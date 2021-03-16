
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <kernel.h>
#include <libdbg.h>
#include <sceerror.h>
#include <scetypes.h>
#include <net.h>
#include <libhttp.h>
#include <sys/socket.h>
#include <scetypes.h>
#include <libnet/socket.h>
#include <sys/errno.h>
#include <libnet/socket.h>
#include <libnetctl.h>
#include <_types.h>
#include <unistd.h>
#include "pup.h"
#include "bls.h"
#include "decryptio.h"
#include "encryptsrv.h"
#include "defines.h"
#include "debug.h"

time_t prevtime;

unsigned char GetElapsed(unsigned long ResetInterval) {

	time_t currenttime = time(0);
	unsigned long elapsed = currenttime - prevtime;

	if ((ResetInterval == 0) || (elapsed >= ResetInterval)) {
		prevtime = currenttime;
		return 1;
	}

	return 0;
}

int verify_segment(const decrypt_state* state, int index, pup_segment* segment, int additional)
{
  int result;
  unsigned char* buffer = NULL;

  buffer = memalign(0x4000, segment->compressed_size);
  unsigned long bytesread = readbytes(state, segment->offset, segment->compressed_size, buffer, segment->compressed_size);
  if (bytesread != segment->compressed_size)
  {
     printf("Failed to read segment #%d for verification!\n", index);
     result = -1;
     goto end;
  }

  result = encsrv_verify_segment(state->device_fd, index, buffer, segment->compressed_size, additional);
  if (result != 0)
  {
    printf("Failed to verify segment #%d! %d\n", index, errno);
    goto end;
  }

end:
  if (buffer != NULL)
  {
    free(buffer);
  }

  return result;
}

int verify_segments(const decrypt_state* state, pup_segment* segments, int segment_count)
{
  int result = 0;

  for (int i = 0; i < segment_count; i++)
  {
    pup_segment* segment = &segments[i];
    if ((segment->flags & 0xF0000000) == 0xE0000000)
    {
		printf("Verifying segment #%d (%d)... [1]\n", i, segment->flags >> 20);
		result = verify_segment(state, i, segment, 1);
		if (result < 0)
		{
			goto end;
      }
    }
  }

  for (int i = 0; i < segment_count; i++)
  {
    pup_segment* segment = &segments[i];
    if ((segment->flags & 0xF0000000) == 0xF0000000)
    {
      printf("Verifying segment #%d (%d)... [0]\n", i, segment->flags >> 20);
      result = verify_segment(state, i, segment, 0);
      if (result < 0)
      {
        goto end;
      }
    }
  }

end:
  return result;
}

int decrypt_segment(const decrypt_state* state, unsigned short index, pup_segment* segment)
{
  int result = -1;

  unsigned char* buffer = buffer = memalign(0x4000, segment->compressed_size);

  int is_compressed = (segment->flags & 8) != 0 ? 1 : 0;

  unsigned long remaining_size = segment->compressed_size;
  if (is_compressed == 1)
  {
    remaining_size &= ~0xFull;
  }

  if (remaining_size > 0)
  {
    unsigned long padding_size = segment->compressed_size & 0xF;
    unsigned long encrypted_size = remaining_size;

    if (segment->compressed_size < remaining_size)
    {
      encrypted_size = segment->compressed_size;
    }

    unsigned long bytesread = readbytes(state, segment->offset, encrypted_size, buffer, segment->compressed_size);
    if (bytesread != encrypted_size)
    {
      printf("Failed to read segment #%d!\n", index);
      result = -1;
      goto end;
    }

    result = encsrv_decrypt_segment(state->device_fd, index, buffer, encrypted_size);
    if (result != 0)
    {
      int errcode = errno;
      printf("Failed to decrypt segment #%d! - Error: %d (%s)\n", index, errcode, strerror(errcode));
      goto end;
    }

    int unencrypted_size = remaining_size - padding_size;
    if (is_compressed == 0 || encrypted_size != remaining_size)
    {
      unencrypted_size = encrypted_size;
    }

    unsigned long byteswritten = writebytes(state, segment->offset, unencrypted_size, buffer, segment->compressed_size);
    if (byteswritten != unencrypted_size) {
      printf("Failed to write segment #%d!\n", index);
      result = -1;
      goto end;
    }
  }

end:
  if (buffer != NULL)
  {
    free(buffer);
  }

  return result;
}

int decrypt_segment_blocks(const decrypt_state * state, unsigned short index, pup_segment* segment,
                           unsigned short table_index, pup_segment* table_segment)
{
  int result = -1;
  unsigned char* table_buffer = NULL;
  unsigned char* block_buffer = NULL;

  unsigned long table_length = table_segment->compressed_size;
  table_buffer = memalign(0x4000, table_length);

  unsigned long bytesread = readbytes(state, table_segment->offset, table_length, table_buffer, table_length);
  if (bytesread != table_length)
  {
    printf("  Failed to read table for segment #%d!\n", index);
    result = -1;
    goto end;
  }

  printf("  Decrypting table #%d for segment #%d\n", table_index, index);
  result = encsrv_decrypt_segment(state->device_fd,
                                 table_index, table_buffer, table_length);
  if (result != 0)
  {
    int errcode = errno;
    printf("  Failed to decrypt table for segment #%d! Error: %d (%s)\n", index, errcode, strerror(errcode));
    goto end;
  }

  int is_compressed = (segment->flags & 8) != 0 ? 1 : 0;

  unsigned long block_size = 1 << (((segment->flags & 0xF000) >> 12) + 12);
  unsigned long block_count = (block_size + segment->uncompressed_size - 1) / block_size;

  unsigned long tail_size = segment->uncompressed_size % block_size;
  if (tail_size == 0)
  {
    tail_size = block_size;
  }

  pup_block_info* block_info = NULL;
  if (is_compressed == 1)
  {
    unsigned long valid_table_length = block_count * (32 + sizeof(pup_block_info));
    if (valid_table_length != table_length)
    {
      printf("  Strange segment #%d table: %llu vs %llu\n",
                   index, valid_table_length, table_length);
    }
    block_info = (pup_block_info*)&table_buffer[32 * block_count];
  }

  block_buffer = memalign(0x4000, block_size);

  printf("  Decrypting %d blocks...\n   ", block_count);

  int Seeked = 0;
  //GetElapsed(0);

  unsigned long remaining_size = segment->compressed_size;
  int last_index = block_count - 1;
  for (int i = 0; i < block_count; i++)
  {
    printf("  Decrypting block %d/%d...\n", i, block_count);

    if ((block_count > 50) && (i % 5 == 0) /*&& (GetElapsed(15) == 1)*/) {
       unsigned int percentage = (unsigned int)(((float)i / (float)block_count) * 100.0f);
       sprintf(state->notifystr, "Approximately %d percent complete processing entry %s (%d/%d) from %s", percentage, state->entryname, state->entryid, state->totalentries, state->input_path);
       //printf_notification(state->notifystr);
    }

    unsigned long read_size;
    unsigned long block_offset = 0;

    if (is_compressed == 1)
    {
      pup_block_info* tblock_info = &block_info[i];
      unsigned int unpadded_size = (tblock_info->size & ~0xFu) - (tblock_info->size & 0xFu);

      read_size = block_size;
      if (unpadded_size != block_size)
      {
        read_size = tblock_info->size;
        if (i != last_index || tail_size != tblock_info->size)
        {
          read_size &= ~0xFu;
        }
      }

      if (block_info->offset != 0)
      {
          block_offset = tblock_info->offset;
      }

    }
    else
    {
      read_size = remaining_size;
      if (block_size < read_size)
      {
        read_size = block_size;
      }
    }

    unsigned long SeekTo = DIO_NOSEEK;
    if (Seeked == 0) {
        SeekTo = (block_offset != 0) ? (segment->offset + block_offset) : segment->offset;
    } else {
        SeekTo = (Seeked != 0) ? DIO_NOSEEK : segment->offset;
    }

    unsigned long bytesread = readbytes(state, SeekTo, read_size, block_buffer, block_size);
    if (bytesread != read_size)
    {
      printf("  Failed to read block %d for segment #%d! %d\n", i, index, bytesread);
      goto end;
    }

    result = encsrv_decrypt_segment_block(state->device_fd, index, i, block_buffer,
					 read_size, table_buffer, table_length);
    if (result < 0)
    {
      int errcode = errno;
      printf("  Failed to decrypt block for segment #%d! Error: %d (%s)\n", index, errcode, strerror(errcode));
      goto end;
    }

    unsigned long byteswritten = writebytes(state, SeekTo, read_size, block_buffer, block_size);
    if (byteswritten != read_size)
    {
      printf("  Failed to write block %d for segment #%d!\n", i, index);
      goto end;
    }

    Seeked = 1;
    remaining_size -= read_size;
  }

end:
  if (block_buffer != NULL)
  {
    free(block_buffer);
  }

  if (table_buffer != NULL)
  {
    free(table_buffer);
  }

  return result;
}

int find_table_segment(int index, pup_segment* segments, int segment_count,
                       int* table_index)
{
  if (((index | 0x100) & 0xF00) == 0xF00)
  {
    printf("Can't do table for segment #%d\n", index);
    *table_index = -1;
    return -1;
  }

  for (int i = 0; i < segment_count; i++)
  {
    if (segments[i].flags & 1)
    {
      unsigned int id = segments[i].flags >> 20;
      if (id == index)
      {
        *table_index = i;
        return 0;
      }
    }
  }

  return -2;
}

int decrypt_pup_data(const decrypt_state * state)
{
  int result;
  unsigned long bytesread;
  unsigned char* header_data = NULL;

  pup_file_header file_header;
  bytesread = readbytes(state, DIO_BASEOFFSET, sizeof(file_header), &file_header, sizeof(file_header));
  if (bytesread != sizeof(file_header))
  {
    printf("Failed to read PUP entry header!\n");
    goto end;
  }


  if (file_header.magic != 0x1D3D154F) {
    printf("PUP header magic is invalid!\n");
    goto end;
  }

  int header_size = file_header.unknown_0C + file_header.unknown_0E;

  header_data = memalign(0x4000, header_size);
  memcpy(header_data, &file_header, sizeof(file_header));

  unsigned long tsize = header_size - sizeof(file_header);
  bytesread = readbytes(state, DIO_NOSEEK, tsize, &header_data[sizeof(file_header)], header_size);
  if (bytesread != tsize)
  {
    printf("Failed to read PUP entry header!\n");
    goto end;
  }

  if ((file_header.flags & 1) == 0)
  {
    printf("Decrypting header...\n");
    result = encsrv_decrypt_header(state->device_fd, header_data,
				   header_size, state->pup_type);
    if (result != 0)
    {
      int errcode = errno;
      printf("Failed to decrypt header! Error: %d (%s)\n", errcode, strerror(errcode));
      goto end;
    }
  }
  else
  {
    printf("Can't decrypt network pup!\n");
    goto end;
  }

  pup_header* header = (pup_header*)&header_data[0];
  pup_segment* segments = (pup_segment*)&header_data[0x20];

  unsigned long byteswritten = writebytes(state, DIO_BASEOFFSET, header_size, header_data, header_size);
  if (byteswritten != header_size) {
     printf("Failed to write PUP entry header!\n");
     goto end;
  }

  printf("Verifying segments...\n");
  result = verify_segments(state, segments, header->segment_count);
  if (result < 0)
  {
    printf("Failed to verify segments!\n");
    goto end;
  }


  /*for (int i = 0; i < header->segment_count; i++)
  {
    pup_segment* segment = &segments[i];
    printf("%4d i=%4u b=%u c=%u t=%u r=%05X\n",
                  i, segment->flags >> 20,
                  (segment->flags & 0x800) != 0,
                  (segment->flags & 0x8) != 0,
                  (segment->flags & 0x1) != 0,
                   segment->flags & 0xFF7F6);
  }*/


  printf("Decrypting %d segments...\n", header->segment_count);
  for (int i = 0; i < header->segment_count; i++)
  {
    pup_segment* segment = &segments[i];

    unsigned int special = segment->flags & 0xF0000000;
    if (special == 0xE0000000)
    {
      printf("Skipping additional signature segment #%d!\n", i);
      continue;
    }
    else if (special == 0xF0000000)
    {
      printf("Skipping watermark segment #%d!\n", i);
      continue;
    }

    printf("Decrypting segment %d/%d...\n",
                 1 + i, header->segment_count);

    if ((segment->flags & 0x800) != 0)
    {
      int table_index;
      result = find_table_segment(i, segments, header->segment_count, &table_index);
      if (result < 0)
      {
        printf("Failed to find table for segment #%d!\n", i);
        continue;
      }

      result = decrypt_segment_blocks(state, i, segment, table_index, &segments[table_index]);
    }
    else
    {
      result = decrypt_segment(state, i, segment);
    }

    if (result < 0) {
       goto end;
    }


  }

end:
  if (header_data != NULL)
  {
    free(header_data);
  }

  return 0;
}

void decrypt_pup(decrypt_state * state, const char * OutputPath)
{

  if (OutputPath != NULL) {
      sprintf(state->output_path, OutputPath, state->entryname);
  }
  else
  {
      sprintf(state->output_path, OUTPUTPATH, state->entryname);
  }

  printf("Creating %s...\n", state->output_path);

  state->output_file = open(state->output_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
  if (state->output_file == -1)
  {
    printf("Failed to open %s!\n", state->output_path);
    goto end;
  }


  const char * name = state->entryname;

  if (strcmp(name, "PS4UPDATE1.PUP") == 0 || strcmp(name, "PS4UPDATE2.PUP") == 0)
    state->pup_type = 1;

  if (strcmp(name, "PS4UPDATE3.PUP") == 0 || strcmp(name, "PS4UPDATE4.PUP") == 0)
    state->pup_type = 0;

  if (state->pup_type < 0)
  {
    printf("Don't know the type for %s!\n", state->output_path);
    goto end;
  }

  decrypt_pup_data(state);

end:
  if (state->output_file != -1)
  {
    close(state->output_file);
  }

}


void decrypt_pups(const char * InputPath, const char * OutputPath)
{

  decrypt_state state = { 0 };
  state.device_fd = -1;

  char * strings = (char*)malloc(2048);
  state.input_path = strings; //512
  state.output_path = strings+512; //512
  state.entryname = strings+1024; //512
  state.notifystr = strings+1536; //512

  unsigned char * header_data = NULL;
  unsigned long blsinitial = 0x400;

  sprintf(state.input_path, "%s", (InputPath != NULL) ? InputPath : INPUTPATH);

  printf("Opening %s...\n", state.input_path);
  state.input_file = open(state.input_path, O_RDONLY, 0);
  if (state.input_file == -1)
  {
    printf("Failed to open %s!\n", state.input_path);
    goto end;
  }

  header_data = memalign(0x4000, blsinitial);

  if (header_data == NULL) {
    printf("Failed to allocate memory!\n");
  }

  unsigned long bytesread = readbytes(&state, DIO_RESET, blsinitial, header_data, blsinitial);

  if (bytesread < blsinitial) {
    printf("Failed to read BLS header or BLS header too small!!\n");
    goto end;
  }

  bls_header * header = (bls_header*)header_data;

  if (header->magic != 0x32424C53) {
    printf("Invalid BLS Header!\n");
    goto end;
  }

  if ((header->file_count < 1) || (header->file_count > 10)) {
     printf("Invalid PUP entry count!\n");
     goto end;
  }

  state.totalentries = header->file_count;

  for (unsigned int i = 0; i < header->file_count; i++)
  {
    state.device_fd = open("/dev/pup_update0", O_RDWR, 0);
    if (state.device_fd < 0)
    {
      printf("Failed to open /dev/pup_update0!\n");
      goto end;
    }

    printf("Verifying Bls Header...\n");
    int result = encsrv_verify_blsheader(state.device_fd, header_data, blsinitial, 0);

    if (result != 0) {
        int errcode = errno;
        printf("Failed while verifying Bls Header! Error: %d (%s)\n", errcode, strerror(errcode));
        goto end;
    }

    sprintf(state.entryname, "%s", header->entry_list[i].name);

    state.pup_type = -1;
    state.entryid = i + 1;

    state.input_base_offset = header->entry_list[i].block_offset * 512;

    state.output_file = -1;
    state.output_base_offset = 0;

    sprintf(state.notifystr, "Decrypting \"%s\" (%d/%d) from %s...", state.entryname, state.entryid, state.totalentries, state.input_path);
    //printf_notification(state.notifystr);

    decrypt_pup(&state, OutputPath);

    close(state.device_fd);
    state.device_fd = -1;

  }

end:
  if (header_data != NULL)
  {
    free(header_data);
  }

  if (strings != NULL) {
    free(strings);
  }

  if (state.input_file != -1)
  {
    close(state.input_file);
  }

  if (state.device_fd != -1)
  {
    close(state.device_fd);
  }

}



