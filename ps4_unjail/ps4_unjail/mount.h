#include <_types.h>

#define	MNT_UPDATE	0x0000000000010000ULL

void build_iovec(struct iovec** iov, int* iovlen, const char* name, const void* val, size_t len);
int mount_large_fs(const char* device, const char* mountpoint, const char* fstype, const char* mode, unsigned int flags);