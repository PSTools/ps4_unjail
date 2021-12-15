//#pragma once
//#include <cstdarg>
//#include <sys/stat.h>
//#include <stddef.h>
//#include <stdint.h>
//#include <stdio.h>
//#include <iostream>
//
//extern uint8_t* gKernelBase;

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <_kernel.h>
#include <stdlib.h>

#define CTL_KERN 1
#define KERN_PROC 14
#define KERN_PROC_ALL 0     /* everything */
#define KERN_PROC_PID 1     /* by process id */
#define KERN_PROC_PGRP 2    /* by process group id */
#define KERN_PROC_SESSION 3 /* by session of pid */
#define KERN_PROC_TTY 4     /* by controlling tty */
#define KERN_PROC_UID 5     /* by effective uid */
#define KERN_PROC_RUID 6    /* by real uid */
#define KERN_PROC_VMMAP         32      /* VM map entries for process */

#define PT_ATTACH       10      /* trace some running process */
#define PT_DETACH       11      /* stop tracing a process */
#define PT_IO           12      /* do I/O to/from stopped process. */

#define PIOD_READ_D     1       /* Read from D space */
#define PIOD_WRITE_D    2       /* Write to D space */
#define PIOD_READ_I     3       /* Read from I space */
#define PIOD_WRITE_I    4       /* Write to I space */

#define PROT_READ        0x1                /* Page can be read.  */
#define PROT_WRITE        0x2                /* Page can be written.  */
#define PROT_EXEC        0x4                /* Page can be executed.  */
#define PROT_NONE        0x0                /* Page can not be accessed.  */
#define PROT_GROWSDOWN        0x01000000        /* Extend change to start of
growsdown vma (mprotect only).  */
#define PROT_GROWSUP        0x02000000        /* Extend change to start of
growsup vma (mprotect only).  */

#define MNT_UPDATE      0x0000000000010000ULL /* not real mount, just update */
#define MNT_DELEXPORT   0x0000000000020000ULL /* delete export host lists */
#define MNT_RELOAD      0x0000000000040000ULL /* reload filesystem data */
#define MNT_FORCE       0x0000000000080000ULL /* force unmount or readonly */
#define MNT_SNAPSHOT    0x0000000001000000ULL /* snapshot the filesystem */
#define MNT_BYFSID      0x0000000008000000ULL /* specify filesystem by ID. */
#define MNT_CMDFLAGS   (MNT_UPDATE      | MNT_DELEXPORT | MNT_RELOAD    | \
	MNT_FORCE       | MNT_SNAPSHOT  | MNT_BYFSID)

typedef unsigned char vm_prot_t;	/* protection codes */
#define	VM_PROT_NONE		((vm_prot_t) 0x00)
#define	VM_PROT_READ		((vm_prot_t) 0x01)
#define	VM_PROT_WRITE		((vm_prot_t) 0x02)
#define	VM_PROT_EXECUTE		((vm_prot_t) 0x04)
#define	VM_PROT_COPY		((vm_prot_t) 0x08)	/* copy-on-read */

#define	VM_PROT_ALL		(VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE)
#define VM_PROT_RW		(VM_PROT_READ|VM_PROT_WRITE)
#define	VM_PROT_DEFAULT		VM_PROT_ALL


struct ptrace_io_desc {
	int piod_op;     /* I/O operation */
	void* piod_offs; /* child offset */
	void* piod_addr; /* parent offset */
	size_t piod_len; /* request length */
};

#define KERN_PMAP_PROTECT	0x2E3090
#define	KERN_PROCESS_ASLR 0x194875
#define	KERN_PTRACE_CHECK 0x30D9AA

#define	KERN_PMAP_PROTECT	0x2E3090
#define	KERN_PMAP_PROTECT_P	0x2E30D4
#define	KERN_PMAP_STORE		0x22CB570


//mira patches here might need to get a way to determen on loadup which is to be used

#define K505_kdlsym_vmspace_acquire_ref_addr        0x19EF90
#define K505_kdlsym_vmspace_free_addr               0x19EDC0
#define K505_kdlsym_vm_map_lock_read_addr           0x19F140
#define K505_kdlsym_vm_map_unlock_read_addr         0x19F190
#define K505_kdlsym_vm_map_lookup_entry_addr        0x19F760
#define K505_kdlsym_proc_rw_mem1_addr               0x30D150

#ifndef kdlsym
#define kdlsym(x) ((void*)((uint8_t *)&gKernelBase[K505_kdlsym_ ## x ##_addr]))
#endif

#define resolve(name) name = ((void *)&gKernelBase[K505_kdlsym_ ## name ##_addr])


//#define _KERNEL 0

void InstallPatches(int FW);


