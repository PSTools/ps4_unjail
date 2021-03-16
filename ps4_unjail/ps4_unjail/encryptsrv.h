#ifndef __ENCRYPTSERVICE_H__
#define __ENCRYPTSERVICE_H__

int encsrv_verify_blsheader(int fd, void* buffer, unsigned long length, int type);

int encsrv_decrypt_header(int fd, void* buffer, unsigned long length, int type);

int encsrv_verify_segment(int fd, unsigned short index, void* buffer, unsigned long length, int additional);

int encsrv_decrypt_segment(int fd, unsigned short index, void* buffer, unsigned long length);

int encsrv_decrypt_segment_block(int fd, unsigned short entry_index, unsigned short block_index, void* block_buffer,
                                unsigned long block_length, void* table_buffer, unsigned long table_length);

#endif
