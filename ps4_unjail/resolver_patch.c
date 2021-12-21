extern "C"
{
#include <external\freebsd-headers\include\sys\sysent.h>
#include <external\freebsd-headers\include\sys/sysent.h>
#include <external\freebsd-headers\include\sys/proc.h>
#include <external\freebsd-headers\include\sys/syscall.h>
#include <external\freebsd-headers\include\sys/ptrace.h>
#include <sys/uio.h>

#include <external\freebsd-headers\include\sys/mman.h>
#include <external\freebsd-headers\include\sys/pcpu.h>
#include <external\freebsd-headers\include\vm/vm.h>
#include <external\freebsd-headers\include\vm/pmap.h>
#include <external\freebsd-headers\include\vm/vm_map.h>
}
/* Util */
sysentvec* sysvec = nullptr;
prison* prison0 = nullptr;
vnode* rootvnode = nullptr;
void (*pause)(const char *wmesg, int timo) = nullptr;

/* STD Lib */
void* M_TEMP = 0;
void* M_MOUNT = 0;
void *(*malloc)(unsigned long size, void *type, int flags) = 0;
void (*free)(void *addr, void *type) = 0;
void (*memcpy)(void *dst, const void *src, size_t len) = 0;
void *(*memset)(void *ptr, int value, size_t num) = 0;
int (*memcmp)(const void *ptr1, const void *ptr2, size_t num) = 0;
size_t (*strlen)(const char *str) = 0;
int (*strcpy)(char * str1, char * str2) = 0;
char* (*strncpy)(char *destination, const char *source, size_t num) = 0;
int (*strcmp)(const char * str1, const char * str2) = 0;
char* (*strstr)(const char * str1, const char * str2) = 0;
int (*sprintf)(char* dst, const char *fmt, ...) = 0;
int (*snprintf)(char *str, size_t size, const char *format, ...) = 0;
int (*vsprintf)(char* dst, const char* fmt, va_list ap);
int (*vprintf)(const char *fmt, va_list arg) = 0;
int(*sscanf)(const char *str, const char *format, ...) = 0;
char *(*strdup)(const char *s, void* type) = 0;
char *(*realloc)(void *addr, unsigned long size, void* mtp, int flags) = 0;

/* Virtual memory */
vmspace *(*vmspace_acquire_ref)(proc *p) = nullptr;
void (*vmspace_free)(vmspace* vm) = nullptr;
void (*vm_map_lock_read)(vm_map* map) = nullptr;
void (*vm_map_unlock_read)(vm_map* map) = nullptr;
int (*vm_map_lookup_entry)(vm_map* map, uint64_t address, vm_map_entry **entries) = nullptr;
int (*vm_map_findspace)(vm_map* map, uint64_t start, uint64_t length, uint64_t *addr) = nullptr;
int (*vm_map_insert)(vm_map* map, uint64_t object, uint64_t offset, uint64_t start, uint64_t end, int prot, int max, int cow) = nullptr;
void (*vm_map_lock)(vm_map* map) = nullptr;
void (*vm_map_unlock)(vm_map* map) = nullptr;
int (*vm_map_delete)(vm_map* map, uint64_t start, uint64_t end) = nullptr;
int (*vm_map_protect)(vm_map* map, uint64_t start, uint64_t end, int new_prot, uint64_t set_max) = nullptr;

/* Proc */
proc *allproc = 0;
int (*proc_rwmem)(proc* p, uio* uio) = 0;
int (*create_thread)(thread* td, uint64_t ctx, void* start_func, void *arg, char *stack_base, size_t stack_size, char *tls_base, long * child_tid, long * parent_tid, uint64_t flags, uint64_t rtp) = 0;

#define NATIVE_RESOLVE(kernbase, _Ty) _Ty = (decltype(_Ty))(void*)(kernbase + addr_ ## _Ty);

void Resolve(uint64_t kernbase)
{
	if(!kernbase)
		return;

	/* Util */
	NATIVE_RESOLVE(kernbase, sysvec);
	NATIVE_RESOLVE(kernbase, prison0);
	NATIVE_RESOLVE(kernbase, rootvnode);
	NATIVE_RESOLVE(kernbase, pause);

	/* STD Lib */
	NATIVE_RESOLVE(kernbase, M_TEMP);
	NATIVE_RESOLVE(kernbase, M_MOUNT);
	NATIVE_RESOLVE(kernbase, malloc);
	NATIVE_RESOLVE(kernbase, free);
	NATIVE_RESOLVE(kernbase, memcpy);
	NATIVE_RESOLVE(kernbase, memset);
	NATIVE_RESOLVE(kernbase, memcmp);
	NATIVE_RESOLVE(kernbase, strlen);
	NATIVE_RESOLVE(kernbase, strcpy);
	NATIVE_RESOLVE(kernbase, strncpy);
	NATIVE_RESOLVE(kernbase, strcmp);
	NATIVE_RESOLVE(kernbase, strstr);
	NATIVE_RESOLVE(kernbase, sprintf);
	NATIVE_RESOLVE(kernbase, snprintf);
	NATIVE_RESOLVE(kernbase, vsprintf);
	NATIVE_RESOLVE(kernbase, vprintf);
	NATIVE_RESOLVE(kernbase, sscanf);
	NATIVE_RESOLVE(kernbase, strdup);
	NATIVE_RESOLVE(kernbase, realloc);   

	/* Virtual Memory */
	NATIVE_RESOLVE(kernbase, vmspace_acquire_ref);
	NATIVE_RESOLVE(kernbase, vmspace_free);
	NATIVE_RESOLVE(kernbase, vm_map_lock_read);
	NATIVE_RESOLVE(kernbase, vm_map_unlock_read);
	NATIVE_RESOLVE(kernbase, vm_map_lookup_entry);
	NATIVE_RESOLVE(kernbase, vm_map_findspace);
	NATIVE_RESOLVE(kernbase, vm_map_insert);
	NATIVE_RESOLVE(kernbase, vm_map_lock);
	NATIVE_RESOLVE(kernbase, vm_map_unlock);
	NATIVE_RESOLVE(kernbase, vm_map_delete);
	NATIVE_RESOLVE(kernbase, vm_map_protect);

	/* Proc */
	NATIVE_RESOLVE(kernbase, allproc);
	NATIVE_RESOLVE(kernbase, proc_rwmem);
	NATIVE_RESOLVE(kernbase, create_thread);
}