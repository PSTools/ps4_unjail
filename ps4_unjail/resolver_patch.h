//#ifndef _RESOLVER_PATCH_H
//#define _RESOLVER_PATCH_H
//#include <stdint.h>
//
//#define __Xfast_syscall		                    0x1C0 //0x3095D0
//#define __M_TEMP			                    0x14B4110 //0x1993B30
//#define __disable_console_output                0x19ECEB0 //0x1997BC8
//#define __kernel_map                            0x1AC60E0 //0x1B31218
//#define __prison0                               0x10986A0 //0x10399B0
//#define __rootvnode                             0x22C1A70 //0x21AFA30
//#define __allproc						        0x2382FF8 //0x1AD7718
//#define __copyin								0x1EA710 //0x14A890
//#define __copyout								0x1EA630 //0x14A7B0
//
//#define __malloc								0x10E250 //0x3F7750
//#define __free									0x10E460 //0x3F7930
//#define __memcpy								0x1EA530 //0x14A6B0
//#define __memset								0x3205C0 //0x302BD0
//#define __memcmp								0x50AC0 //0x242A60
//#define __kmem_alloc							0xFCC80 //0x16ECD0
//#define __strlen                                0x3B71A0 //0x3514F0
//#define __sprintf                               0x436280
//#define __vsprintf                              0x436310
//
//#define __sys_socket 				            0x318EE0 //0x3EAC20
//#define __sys_connect       		            0x31A310
//#define __sys_bind        			            0x319820 //0x3EB550
//#define __sys_listen       			            0x319A60 //0x3EB760
//#define __sys_accept       			            0x31A170 //0x3EBEC0
//#define __sys_read        			            0x152AB0 //0x5EC50
//#define __sys_send        			            0x152FC0 //0x5F1A0
//#define __sys_setsockopt      		            0x31B750 //0x3ED300
//#define __sys_close        			            0xC0EB0 //0x42AC00
//
//#define __kern_ptrace                           0x30D8E0 //0x17D200
//#define __kern_psignal                          0xD35F0 //0x25690
//#define __kern_wait                             0x35590 //
//
//#define __pause									0x3FB920 //0x261120
//#define __kthread_exit							0x138640 //0x464F60
//#define __kproc_create							0x137DF0 //0x464700
//#define __kthread_add							0x138360 //0x464C90
//#define __sched_prio							0x31EE00 //0x072410
//#define __sched_add								0x31F150 //0x072740
//#define __kern_yield							0x3FBC40 //0x261440
//#define __fill_regs								0x234BA0 //0x2829C0
//#define __set_regs								0x234CD0 //0x282AF0
//#define __create_thread							0x1BE1F0 //0x2ECCD0
//#define __kthread_set_affinity					0x138CC0 //0x4655E0
//#define __kthread_suspend_check					0x138A60 //0x465380
//#define __kproc_kthread_add						0x138B70 //0x465490
//#define __sx_init_flags							0xF5BB0 //0x38F900
//#define __sx_xlock								0xF5E10 //0x38FA30
//#define __sx_xunlock							0xF5FD0 //0x38FBC0
//#define __mtx_init								0x402780 //0x30E0C0
//#define __mtx_lock_spin_flags					0x402100 //0x30DA70
//#define __mtx_unlock_spin_flags					0x4022C0 //0x30DC30
//#define __mtx_lock_sleep						0x401CD0 //0x30D6A0
//#define __mtx_unlock_sleep						0x401FA0 //0x30D940
//#define __kern_reboot							0x10D390 //0x0998A0
//#define __vm_map_lock_read						0x19F140 //0x3920B0
//#define __vm_map_lookup_entry					0x19F760 //0x392C70
//#define __vm_map_unlock_read					0x19F190 //0x392100
//#define __vmspace_free							0x19EDC0 //0x391D10
//#define __vm_map_delete							0x1A19D0 //0x394790
//#define __vm_map_protect						0x1A3A50 //0x396860
//#define __vmspace_acquire_ref					0x19EF90 //0x391EE0
//#define __vm_map_findspace						0x1A1F60 //0x394E90
//#define __vm_map_insert							0x1A0280 //0x392F70
//#define __vm_map_lock							0x19EFF0 //0x391F40
//#define __vm_map_unlock 						0x19F060 //0x391FB0
//#define __proc_rwmem							0x30D150 //0x17CB70
//
//void *M_TEMP;
//const struct sbl_map_list_entry **sbl_driver_mapped_pages;
//const uint8_t *mini_syscore_self_binary;
//struct sbl_key_rbtree_entry **sbl_keymgr_key_rbtree;
//void *fpu_kern_ctx;
//
//// freebsd/common
//int (*sprintf)(char* dst, const char *fmt, ...);
//int (*vsprintf)(char* dst, const char* fmt, va_list ap);
//void (*memcpy)(void *dst, const void *src, size_t len);
//int (*memcmp)(const void *ptr1, const void *ptr2, size_t num);
//size_t (*strlen)(const char *str);
//void (*pause)(const char *wmesg, int timo);
//int (*kthread_add)(void (*func)(void *), void *arg, struct proc *procp, struct thread **newtdpp, int flags, int pages, const char *fmt, ...);
//void (*kthread_exit)(void);
//void (*kthread_suspend_check)(void);
//void (*kthread_set_affinity)(const char *tdname, uint64_t prio, uint64_t cpuset, uint64_t unknown); // custom name
//int (*kproc_create)(void (*func)(void *), void *arg, struct proc **newpp, int flags, int pages, const char *fmt, ...);
//int (*kproc_kthread_add)(void (*func)(void *), void *arg, struct proc **procptr, struct thread **tdptr, int flags, int pages, char * procname, const char *fmt, ...);
//void (*sched_prio)(struct thread *td, uint16_t prio);
//void (*sched_add)(struct thread *td, uint64_t cpuset);
//void (*kern_yield)(uint64_t p);
//int (*create_thread)(struct thread * td, uint64_t ctx, void (*start_func)(void *), void *arg, char *stack_base, size_t stack_size, char *tls_base, long * child_tid, long * parent_tid, uint64_t flags, uint64_t rtp);
//int (*proc_rwmem)(struct proc *p, struct uio *uio);
//void (*sx_init_flags)(struct sx *sx, const char *description, int opts);
//void (*sx_xlock)(struct sx *sx);
//void (*sx_xunlock)(struct sx *sx);
//void (*mtx_init)(struct mtx *mutex, const char *name, const char *type, int opts);
//void (*mtx_lock_spin_flags)(struct mtx *mutex, int flags);
//void (*mtx_unlock_spin_flags)(struct mtx *mutex, int flags);
//void (*mtx_lock_sleep)(struct mtx *mutex, int flags);
//void (*mtx_unlock_sleep)(struct mtx *mutex, int flags);
//int (*fpu_kern_enter)(struct thread *td, void *ctx, unsigned int flags);
//int (*fpu_kern_leave)(struct thread *td, void *ctx);
//void (*kern_reboot)(int magic);
//int	(*fill_regs)(struct thread *td, struct reg *rg);
//int	(*set_regs)(struct thread *td, struct reg *rg);
//
////ptrace
//int(*kern_ptrace)(struct thread * td, int req, int pid, void * addr, int data);
//int(*kern_psignal)(struct proc* proc, int sig);
//int(*kern_wait)(struct thread *td, int wpid, int *status, int options, void *rusage);
//
//// virtual memory
//struct vmspace *(*vmspace_acquire_ref)(struct proc *p);
//void (*vmspace_free)(struct vmspace *vm);
//void (*vm_map_lock_read)(struct vm_map *map);
//void (*vm_map_unlock_read)(struct vm_map *map);
//int (*vm_map_lookup_entry)(struct vm_map *map, uint64_t address, struct vm_map_entry **entries);
//int (*vm_map_findspace)(struct vm_map * map, uint64_t start, uint64_t length, uint64_t *addr);
//int (*vm_map_insert)(struct vm_map * map, uint64_t object, uint64_t offset, uint64_t start, uint64_t end, int prot, int max, int cow);
//void (*vm_map_lock)(struct vm_map * map);
//void (*vm_map_unlock)(struct vm_map * map);
//int (*vm_map_delete)(struct vm_map * map, uint64_t start, uint64_t end);
//int (*vm_map_protect)(struct vm_map * map, uint64_t start, uint64_t end, int new_prot, uint64_t set_max);
//
//void resolve(uint64_t kernbase);
//
//#endif