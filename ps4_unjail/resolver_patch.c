#include "resolver_patch.h"

#define RESOLVE(name, base, offset) \
	name = (void*)(base + offset); \

void resolve(uint64_t kernbase) {
	RESOLVE(M_TEMP, kernbase, __M_TEMP);
	RESOLVE(sprintf, kernbase, __sprintf);
	RESOLVE(vsprintf, kernbase, __vsprintf);
	RESOLVE(malloc, kernbase, __malloc);
	RESOLVE(free, kernbase, __free);
	RESOLVE(memcpy, kernbase, __memcpy);
	RESOLVE(memset, kernbase, __memset);
	RESOLVE(memcmp, kernbase, __memcmp);
	RESOLVE(strlen, kernbase, __strlen);
	RESOLVE(pause, kernbase, __pause);
	RESOLVE(kthread_add, kernbase, __kthread_add);
	RESOLVE(kthread_exit, kernbase, __kthread_exit);
	RESOLVE(sched_prio, kernbase, __sched_prio);
	RESOLVE(sched_add, kernbase, __sched_add);
	RESOLVE(kern_yield, kernbase, __kern_yield);
	RESOLVE(create_thread, kernbase, __create_thread);
	RESOLVE(kthread_suspend_check, kernbase, __kthread_suspend_check);
	RESOLVE(kthread_set_affinity, kernbase, __kthread_set_affinity);
	RESOLVE(kproc_create, kernbase, __kproc_create);
	RESOLVE(kproc_kthread_add, kernbase, __kproc_kthread_add);
	RESOLVE(proc_rwmem, kernbase, __proc_rwmem);
	RESOLVE(sx_init_flags, kernbase, __sx_init_flags);
	RESOLVE(sx_xlock, kernbase, __sx_xlock);
	RESOLVE(sx_xunlock, kernbase, __sx_xunlock);
	RESOLVE(mtx_init, kernbase, __mtx_init);
	RESOLVE(mtx_lock_spin_flags, kernbase, __mtx_lock_spin_flags);
	RESOLVE(mtx_unlock_spin_flags, kernbase, __mtx_unlock_spin_flags);
	RESOLVE(mtx_lock_sleep, kernbase, __mtx_lock_sleep);
	RESOLVE(mtx_unlock_sleep, kernbase, __mtx_unlock_sleep);
	RESOLVE(kern_reboot, kernbase, __kern_reboot);
	RESOLVE(vm_map_lock_read, kernbase, __vm_map_lock_read);
	RESOLVE(vm_map_lookup_entry, kernbase, __vm_map_lookup_entry);
	RESOLVE(vm_map_unlock_read, kernbase, __vm_map_unlock_read);
	RESOLVE(vm_map_findspace, kernbase, __vm_map_findspace);
	RESOLVE(vm_map_insert, kernbase, __vm_map_insert);
	RESOLVE(vm_map_lock, kernbase, __vm_map_lock);
	RESOLVE(vm_map_unlock, kernbase, __vm_map_unlock);
	RESOLVE(vm_map_delete, kernbase, __vm_map_delete);
	RESOLVE(vm_map_protect, kernbase, __vm_map_protect);
	RESOLVE(vmspace_free, kernbase, __vmspace_free);
	RESOLVE(vmspace_acquire_ref, kernbase, __vmspace_acquire_ref);
	RESOLVE(fill_regs, kernbase, __fill_regs);
	RESOLVE(set_regs, kernbase, __set_regs);
	RESOLVE(kern_ptrace, kernbase, __kern_ptrace);
	RESOLVE(kern_psignal, kernbase, __kern_psignal);
	RESOLVE(kern_wait, kernbase, __kern_wait);
}