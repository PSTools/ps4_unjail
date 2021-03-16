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
#include "encryptsrv_args.h"


int translate_type(int type)
{
  switch (type)
  {
    case 0:
    case 3: return 0;
    case 1:
    case 4: return 1;
    case 2:
    case 5: return 2;
  }
  return 0;
}


int encsrv_verify_blsheader(int fd, void* buffer, unsigned long length, unsigned long unknown) {
  verify_blsheader_args args;
  memset(&args, 0, sizeof(args));
  args.buffer = buffer,
  args.length = length;
  args.unknown = unknown;
  return syscall(54,fd, 0xC010440D, &args);
}


int encsrv_decrypt_header(int fd, void* buffer, unsigned long length, int type)
{
  decrypt_header_args args;
  memset(&args, 0, sizeof(args));
  args.buffer = buffer,
  args.length = length;
  args.type = translate_type(type);
  return syscall(54,fd, 0xC0184401, &args);
}

int encsrv_verify_segment(int fd, unsigned short index, void* buffer, unsigned long length, int additional)
{
  verify_segment_args args;
  memset(&args, 0, sizeof(args));
  args.index = index;
  args.buffer = buffer,
  args.length = length;
  //VerifyPupAdditionalSign / VerifyPupWatermark
  int op = additional != 0 ? 0xC0184402 : 0xC0184403;
  return syscall(54,fd, op, &args);
}

int encsrv_decrypt_segment(int fd, unsigned short index, void* buffer, unsigned long length)
{
  decrypt_segment_args args;
  memset(&args, 0, sizeof(args));
  args.index = index;
  args.buffer = buffer,
  args.length = length;
  return syscall(54,fd, 0xC0184404, &args);
}

int encsrv_decrypt_segment_block(int fd, unsigned short entry_index, unsigned short block_index, void* block_buffer,
                                unsigned long block_length, void* table_buffer, unsigned long table_length)
{
  decrypt_segment_block_args args;
  memset(&args, 0, sizeof(args));
  args.entry_index = entry_index;
  args.block_index = block_index;
  args.block_buffer = block_buffer,
  args.block_length = block_length;
  args.table_buffer = table_buffer;
  args.table_length = table_length;
  return syscall(54,fd, 0xC0284405, &args);
}
