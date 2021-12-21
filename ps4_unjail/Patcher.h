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
#define K505_kdlsym__vm_map_lock_addr               0x0019EFF0
#define K505_kdlsym__vm_map_lock_read_addr          0x0019F140
#define K505_kdlsym__vm_map_insert_addr             0x001A0280
#define K505_kdlsym__vm_map_unlock_addr             0x0019F060
#define K505_kdlsym_addr_vm_map_lookup_entry        0x0019F760
#define K505_kdlsym_addr__vm_map_unlock_read        0x0019F190
#define K505_kdlsym_addr_M_TEMP                     0x014B4110

#ifndef kdlsym
#define kdlsym(x) ((void*)((uint8_t *)&gKernelBase[K505_kdlsym_ ## x ##_addr]))
#endif

#define resolve(name) name = ((void *)&gKernelBase[K505_kdlsym_ ## name ##_addr])

#define JOIN_HELPER(x, y) x##y
#define JOIN(x, y) JOIN_HELPER(x, y)

#define TYPE_PAD(size) char JOIN(_pad_, __COUNTER__)[size]
#define TYPE_VARIADIC_BEGIN(name) name { union {
#define TYPE_BEGIN(name, size) name { union { TYPE_PAD(size)
#define TYPE_END(...) }; } __VA_ARGS__
#define TYPE_FIELD(field, offset) struct { TYPE_PAD(offset); field; }

#define TYPE_CHECK_SIZE(name, size) \
	_Static_assert(sizeof(name) == (size), "Size of " #name " != " #size)

#define TYPE_CHECK_FIELD_OFFSET(name, member, offset) \
	_Static_assert(offsetof(name, member) == (offset), "Offset of " #name "." #member " != " #offset)

#define TYPE_CHECK_FIELD_SIZE(name, member, size) \
	_Static_assert(sizeof(((name*)0)->member) == (size), "Size of " #name "." #member " != " #size)



struct lock_object {
	const char* lo_name;
	uint32_t lo_flags;
	uint32_t lo_data;
	void* lo_witness;
};

struct mtx {
	struct lock_object lock_object;
	volatile void* mtx_lock;
};

struct sx {
	struct lock_object lock_object;
	volatile uintptr_t sx_lock;
};

TYPE_BEGIN(struct vm_map_entry, 0xC0);
TYPE_FIELD(struct vm_map_entry *prev, 0);
TYPE_FIELD(struct vm_map_entry *next, 8);
TYPE_FIELD(struct vm_map_entry *left, 0x10);
TYPE_FIELD(struct vm_map_entry *right, 0x18);
TYPE_FIELD(uint64_t start, 0x20);
TYPE_FIELD(uint64_t end, 0x28);
TYPE_FIELD(uint64_t offset, 0x50);
TYPE_FIELD(uint16_t prot, 0x5C);
TYPE_FIELD(char name[32], 0x8D);
TYPE_END();

TYPE_BEGIN(struct vm_map, 0x178);
TYPE_FIELD(struct vm_map_entry header, 0);
TYPE_FIELD(struct sx lock, 0xB8);
TYPE_FIELD(struct mtx system_mtx, 0xD8);
TYPE_FIELD(int nentries, 0x100);
TYPE_END();

TYPE_BEGIN(struct vmspace, 0x250);
TYPE_FIELD(struct vm_map vm_map, 0);
// maybe I will add more later just for documentation purposes
TYPE_END();

TYPE_BEGIN(struct sysent, 0x30);
TYPE_FIELD(uint32_t sy_narg, 0x00);
TYPE_FIELD(void *sy_call, 0x08);
TYPE_FIELD(uint16_t sy_auevent, 0x10);
TYPE_FIELD(uint64_t sy_systrace_args_func, 0x18);
TYPE_FIELD(uint32_t sy_entry, 0x20);
TYPE_FIELD(uint32_t sy_return, 0x24);
TYPE_FIELD(uint32_t sy_flags, 0x28);
TYPE_FIELD(uint32_t sy_thrcnt, 0x2C);
TYPE_END();

TYPE_BEGIN(struct kthread, 0x800); // XXX: random, don't use directly without fixing it
TYPE_FIELD(struct mtx *volatile td_lock, 0);
TYPE_FIELD(struct kproc *td_proc, 8);
TYPE_FIELD(TAILQ_ENTRY(kthread) td_plist, 0x10);
TYPE_FIELD(int tid, 0x88);
TYPE_FIELD(int td_pinned, 0x12C);
TYPE_FIELD(struct ucred *td_ucred, 0x130);
TYPE_FIELD(char td_name[32], 0x284);
TYPE_FIELD(uint64_t td_retval[2], 0x398);
TYPE_FIELD(uint16_t td_priority, 0x380);
TYPE_END();


TYPE_BEGIN(struct kproc, 0x800); // XXX: random, don't use directly without fixing it
TYPE_FIELD(struct kproc *p_forw, 0);
TYPE_FIELD(TAILQ_HEAD(, kthread) p_threads, 0x10);
TYPE_FIELD(struct ucred *p_ucred, 0x40);
TYPE_FIELD(struct filedesc *p_fd, 0x48);
TYPE_FIELD(int pid, 0xB0);
TYPE_FIELD(struct vmspace *p_vmspace, 0x168);
TYPE_FIELD(char p_comm[32], 0x44C);
TYPE_FIELD(char titleid[16], 0x390);
TYPE_FIELD(char contentid[64], 0x3D4);
TYPE_FIELD(char path[64], 0x46C);
TYPE_END();


//#define _KERNEL 0

void InstallPatches(int FW);


