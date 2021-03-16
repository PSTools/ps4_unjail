#ifndef __DECRYPTIO_H__
#define __DECRYPTIO_H__



typedef struct _decrypt_state
{
  char * input_path;
  unsigned long input_base_offset;
  int input_file;

  char * output_path;
  unsigned long output_base_offset;
  int output_file;

  int device_fd;

  char * entryname;
  int entryid;
  int totalentries;
  int pup_type;

  char * notifystr;
}
decrypt_state;

#define DIO_NOSEEK 0
#define DIO_RESET -1
#define DIO_BASEOFFSET -2

unsigned long readbytes(const decrypt_state* state, unsigned long offset, unsigned long bytes, void* buffer, unsigned long buffersize);
unsigned long writebytes(const decrypt_state* state, unsigned long offset, unsigned long bytes, void* buffer, unsigned long buffersize);

#endif
