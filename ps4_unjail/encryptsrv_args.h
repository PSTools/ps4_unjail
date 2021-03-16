#ifndef __ENCRYPTSERVICEARGS_H__
#define __ENCRYPTSERVICEARGS_H__

typedef struct _verify_blsheader_args
{
  void* buffer; //rbx (rsi)
  unsigned long length; //r14 (rdx)
  unsigned long unknown; //rax
}
verify_blsheader_args;

typedef struct _decrypt_header_args
{
  void* buffer;
  unsigned long length;
  int type;
}
decrypt_header_args;


typedef struct _verify_segment_args
{
  unsigned short index;
  void* buffer;
  unsigned long length;
}
verify_segment_args;

typedef struct _decrypt_segment_args
{
  unsigned short index;
  void* buffer;
  unsigned long length;
}
decrypt_segment_args;

typedef struct _decrypt_segment_block_args
{
  unsigned short entry_index;
  unsigned short block_index;
  void* block_buffer;
  unsigned long block_length;
  void* table_buffer;
  unsigned long table_length;
}
decrypt_segment_block_args;

#endif
