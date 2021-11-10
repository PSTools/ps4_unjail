#pragma region << KernelClasss >>

#include <_kernel.h>
#include "types.h"
#include "syscall.h"

typedef struct Process{
	int process_id;
	char process_name[100];
} Process;

typedef struct {
	uint64_t unk1;
	char version_string[0x1C];
	uint32_t version;
} SceFwInfo;

#ifndef __kernel_fsid_t
typedef struct {
	int val[2];
} __kernel_fsid_t;
#endif


/* note: this corresponds to the kernel's statfs64 type */
typedef struct {
	uint32_t        f_type;
	uint32_t        f_bsize;
	uint64_t        f_blocks;
	uint64_t        f_bfree;
	uint64_t        f_bavail;
	uint64_t        f_files;
	uint64_t        f_ffree;
	__kernel_fsid_t f_fsid;
	uint32_t        f_namelen;
	uint32_t        f_frsize;
	uint32_t        f_spare[5];
} _statfs;

#if __WORDSIZE == 32          /* System word size */
# define __SWORD_TYPE           int
#else /* __WORDSIZE == 64 */
# define __SWORD_TYPE         long int
#endif

typedef  struct  {
	__SWORD_TYPE f_type;    /* type of filesystem (see below) */
	__SWORD_TYPE f_bsize;   /* optimal transfer block size */
	fsblkcnt_t   f_blocks;  /* total data blocks in filesystem */
	fsblkcnt_t   f_bfree;   /* free blocks in fs */
	fsblkcnt_t   f_bavail;  /* free blocks available to
							unprivileged user */
	fsfilcnt_t   f_files;   /* total file nodes in filesystem */
	fsfilcnt_t   f_ffree;   /* free file nodes in fs */
	uint32       f_fsid;    /* filesystem id */
	__SWORD_TYPE f_namelen; /* maximum length of filenames */
	__SWORD_TYPE f_frsize;  /* fragment size (since Linux 2.6) */
	__SWORD_TYPE f_spare[5];
} _fstatfs;



typedef mode_t SceKernelMode;

void loadAndImport();

int loadModule(const char *name, int *idDestination);

void loadKernel();

char* KernelGetIDPS();

const char* KernelGetPSID();

char* Kernel_firmware_version_libc();

uint16_t Kernel_Get_FW_Version();

const char* Kernel_Firmware_Version();

char* Kernel_GetSandboxPath();

const char* KernelGetOpenPSID();



int Kernel_Get_Fw();

#pragma endregion << KernelClass >>