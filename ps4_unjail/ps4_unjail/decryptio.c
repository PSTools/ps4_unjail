
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
#include "decryptio.h"
#include "defines.h"
#include "debug.h"

#define chunksize 2097152

unsigned long readbytes(const decrypt_state * state, unsigned long offset, unsigned long bytes, void * buffer, unsigned long buffersize) {

  if (bytes > buffersize) {
     printf("ReadBytes failed! - Error: Buffer is too small!\n");
     return -1;
  }

  unsigned long result = -1;

  if (offset != 0) {

      switch (offset) {
      case DIO_RESET:
          result = lseek(state->input_file, 0, SEEK_SET);
          break;
      case DIO_BASEOFFSET:
          result = lseek(state->input_file, state->input_base_offset, SEEK_SET);
          break;
      default:
          result = lseek(state->input_file, state->input_base_offset + offset, SEEK_SET);
          break;
      }

      if (result == -1) {
          int errcode = errno;
          printf("ReadBytes seek_set failed! - Error: %d (%s)\n", errcode, strerror(errcode));
          return -1;
      }

      printf("Seeked to position " SSIZET_FMT " in input file.\n", result);
  }

  unsigned long rchunksize = (bytes >= chunksize) ? chunksize : bytes;

  unsigned long bytesread = 0;
  unsigned long bytesremaining = bytes;

  while (bytesremaining > 0) {
    result = read(state->input_file, buffer+bytesread, (bytesremaining >= rchunksize) ? rchunksize : bytesremaining);

    if (result < 1) {
       break;
    }

    bytesread += result;
    bytesremaining -= result;
  }

  if ((result == -1) || (bytesread != bytes)) {
      int errcode = errno;
      printf("Read failed; Read " SSIZET_FMT " of " SSIZET_FMT "bytes - Result: %d (%s)\n", bytesread, bytes,
												  errcode,
												  strerror(errcode));
      return -1;
  }

//  printf("Read " SSIZET_FMT " of " SSIZET_FMT " bytes to from input file.\n", bytesread, byte);

  return bytesread;

}


unsigned long writebytes(const decrypt_state * state, unsigned long offset, unsigned long bytes, void * buffer, unsigned long buffersize) {

  if (bytes > buffersize) {
     printf("WriteBytes failed! - Error: Buffer is too small!\n");
     return -1;
  }

  unsigned long result = -1;

  if (offset != 0) {

      switch (offset) {
      case DIO_RESET:
          result = lseek(state->output_file, 0, SEEK_SET);
          break;
      case DIO_BASEOFFSET:
          result = lseek(state->output_file, state->output_base_offset, SEEK_SET);
          break;
      default:
          result = lseek(state->output_file, state->output_base_offset + offset, SEEK_SET);
          break;
      }

      if (result == -1) {
          int errcode = errno;
          printf("WriteBytes seek_set failed! - Error: %d (%s)\n", errcode, strerror(errcode));
          return -1;
      }

      printf("Seeked to position " SSIZET_FMT " in output file.\n", result);
  }

  unsigned long wchunksize = (bytes >= chunksize) ? chunksize : bytes;

  unsigned long byteswritten = 0;
  unsigned long bytesremaining = bytes;

  while (bytesremaining > 0) {
    result = write(state->output_file, buffer+byteswritten, (bytesremaining >= wchunksize) ? wchunksize : bytesremaining);

    if (result < 1) {
       break;
    }

    byteswritten += result;
    bytesremaining -= result;
  }

  if ((result == -1) || (byteswritten != bytes)) {
      int errcode = errno;
      printf("Write failed; Write " SSIZET_FMT " of " SSIZET_FMT "bytes - Result: %d (%s)\n", byteswritten,
												    bytes, errcode,
												    strerror(errcode));
      return -1;
  }

//  printf("Write " SSIZET_FMT " of " SSIZET_FMT " bytes to output file.\n", byteswritten, bytes);

  return byteswritten;

}


