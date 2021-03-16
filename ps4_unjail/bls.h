#ifndef __BLSHEADER_H__
#define __BLSHEADER_H__

typedef struct _bls_entry
{
  unsigned int block_offset;
  unsigned int size;
  unsigned char reserved[8];
  char name[32];
}
bls_entry;

typedef struct _bls_header
{
  unsigned int magic;
  unsigned int version;
  unsigned int flags;
  unsigned int file_count;
  unsigned int block_count;
  unsigned char unknown[12];
  struct _bls_entry entry_list[0];
}
bls_header;

#endif
