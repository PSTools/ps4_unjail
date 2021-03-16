#ifndef __PUP_H__
#define __PUP_H__

typedef struct _pup_file_header
{
  unsigned int magic;
  unsigned int unknown_04;
  unsigned short unknown_08;
  unsigned char flags;
  unsigned char unknown_0B;
  unsigned short unknown_0C;
  unsigned short unknown_0E;
}
pup_file_header;

typedef struct _pup_header
{
  pup_file_header file_header;
  unsigned long file_size;
  unsigned short segment_count;
  unsigned short unknown_1A;
  unsigned int unknown_1C;
}
pup_header;

typedef struct _pup_segment
{
  unsigned int flags;
  unsigned long offset;
  unsigned long compressed_size;
  unsigned long uncompressed_size;
}
pup_segment;

typedef struct _pup_block_info
{
  unsigned int offset;
  unsigned int size;
}
pup_block_info;

#endif
