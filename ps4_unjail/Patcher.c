
#include "Patcher.h"
#include "unjail.h"
#include "syscall.h"

#define nullptr ((void*)0)

// 'sce_sdmemory' patch
#define K505_sce_sdmemory_patch				0xD42843
//verify keystone patch
#define K505_verify_keystone_patch			0x7E4DC0
//transfer mount permission patch eg mount foreign saves with write permission
#define K505_save_mount_permision			0x68BA0
//some other patch not sure 
#define K505_save_mount_patch				0x6A349
//something something patch
#define K505_save_mount_something_patch		0x686AE
//patch unk 
#define K505_save_mount_patch_unk			0x67FCA
//save mount never jump
#define K505_save_mount_never_jump			0x67798
//save data always jump
#define K505_save_mount_always_jump			0x679D5
//psn save patch 
#define K505_save_psn_patch					0xC54F0





#define CR0_WP (1 << 16) // write protect

static inline __attribute__((always_inline)) uint64_t readCr0(void)
{
	uint64_t cr0;
	__asm__ volatile ("movq %0, %%cr0" : "=r" (cr0) : : "memory");
	return cr0;
}

static inline __attribute__((always_inline)) void writeCr0(uint64_t cr0)
{
	__asm__ volatile("movq %%cr0, %0" : : "r" (cr0) : "memory");
}

uint64_t __readmsr1(unsigned long __register)
{
	unsigned long __edx;
	unsigned long __eax;
	__asm__ ("rdmsr" : "=d"(__edx), "=a"(__eax) : "c"(__register));
	return (((uint64_t)__edx) << 32) | (uint64_t)__eax;
}

__asm__("kexec:\nmov $11, %rax\nmov %rcx, %r10\nsyscall\nret");
void kexec(void*, void*);
int (*sysctl)(const int *, u_int, void *, size_t *, const void *, size_t);


void loadAndImport()
{
	int kernel_lib = sceKernelLoadStartModule("/system/common/lib/libkernel.sprx", 0, NULL, 0, NULL, NULL);
	/*if(kernel_lib < 0)
	{
	kernel_lib = sceKernelLoadStartModule("/system/common/lib/libkernel_web.sprx", 0, NULL, 0, NULL, NULL);
	}
	if(kernel_lib < 0)
	{
	kernel_lib = sceKernelLoadStartModule("/system/common/lib/libkernel_sys.sprx", 0, NULL, 0, NULL, NULL);
	}*/
	//sceKernelDlsym(kernel_lib, "sceKernelGetIdPs", (void **)&sceKernelGetIdPs);//this just returns some fed up info
	//sceKernelDlsym(kernel_lib, "sceKernelGetOpenPsIdForSystem", (void **)&sceKernelGetOpenPsIdForSystem);
	//sceKernelDlsym(kernel_lib, "sceKernelGetOpenPsId", (void **)&sceKernelGetOpenPsId);
	//sceKernelDlsym(kernel_lib, "sysctlbyname", (void **)&sysctlbyname);
	sceKernelDlsym(kernel_lib,"sysctl",(void **)&sysctl);
	//sceKernelDlsym(kernel_lib,"_fstat",(void **)&statfs);
	//sceKernelDlsym(kernel_lib,"fstatfs",(void **)&fstatfs);
	/*sceKernelDlsym(kernel_lib, "sceKernelGetSystemSwVersion",(void **)&sceKernelGetSystemSwVersion1);
	sceKernelDlsym(kernel_lib, "sysctlnametomib",(void **)&sysctlnametomib);
	sceKernelDlsym(kernel_lib, "sceKernelGetFsSandboxRandomWord",(void **)&sceKernelGetFsSandboxRandomWord_1);*/
}


int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char* message);
int (*customsceSystemServiceLoadExec)(const char* path, char* const argv[]);


void initSysUtil(void)
{
	int ret = 0;

	int sysUtilHandle = sceKernelLoadStartModule("/system/common/lib/libSceSysUtil.sprx", 0, NULL, 0, 0, 0);
	if(sysUtilHandle > 0)
	{
		ret =sceKernelDlsym(sysUtilHandle, "sceSysUtilSendSystemNotificationWithText", (void **)&sceSysUtilSendSystemNotificationWithText);
		if(ret < 0)
		{
			//we couldn't load libscesysutil
		}
		sceKernelDlsym(sysUtilHandle, "sceSystemServiceLoadExec",(void **)&customsceSystemServiceLoadExec);
	}
	else
	{

	}
}

void notify(char *message)
{
	//int lenghtofmessage = sizeof(message);
	char buffer[5120];
	sprintf(buffer, "%s", message);//dont want all those enters
	//sceSysUtilSendSystemNotificationWithText(0x81, buffer);
	sceSysUtilSendSystemNotificationWithText(222, buffer);
}


//
//
////i dont care abo
////void Install_Patch(uint64_t Address, const void* Data, size_t Length)
////{
////
////	//Set protection to all
////	sceKernelMprotect((void*)Address, Length, VM_PROT_ALL);
////
////	//Write Patch.
////	memcpy((void*)Address, Data, Length);
////
////	//klog("[Patcher] Install_Patch: Patch (%llX) Written Successfully!\n", Address);
////}
//
////int get_proc_count() {
////	int count = 0;
////	struct proc *p;
////
////	uint8_t* kernbase = gKernelBase;
////	p = *(struct proc **)(kernbase + K505_addr_allproc);
////
////	do {
////		count++;
////	} while ((p = p->p_forw));
////
////	return count;
////}
////
////struct proc *proc_find_by_name(const char *name) {
////	struct proc *p;
////
////	if (!name) {
////		return NULL;
////	}
////
////	uint8_t* kernbase = gKernelBase;
////	p = *(struct proc **)(kernbase + K505_addr_allproc);
////
////	do {
////		if (!memcmp(p->p_comm, name, strlen(name))) {
////			return p;
////		}
////	} while ((p = p->p_forw));
////
////	return NULL;
////}
//
////struct proc *proc_find_by_pid(int pid) {
////	struct proc *p;
////
////	uint8_t* kernbase = gKernelBase;
////	p = *(struct proc **)(kernbase + K505_addr_allproc);
////
////	do {
////		if (p->pid == pid) {
////			return p;
////		}
////	} while ((p = p->p_forw));
////
////	return NULL;
////}
////
////int proc_get_vm_map(struct proc *p, struct proc_vm_map_entry **entries, size_t *num_entries) {
////	struct proc_vm_map_entry *info = NULL;
////	struct vm_map_entry *entry = NULL;
////
////	struct vmspace *vm = vmspace_acquire_ref(p);
////	if (!vm) {
////		return 1;
////	}
////
////	struct vm_map *map = &vm->vm_map;
////
////	int num = map->nentries;
////	if (!num) {
////		vmspace_free(vm);
////		return 0;
////	}
////
////	vm_map_lock_read(map);
////
////	if (vm_map_lookup_entry(map, NULL, &entry)) {
////		vm_map_unlock_read(map);
////		vmspace_free(vm);
////		return 1;
////	}
////
////	info = (struct proc_vm_map_entry *)alloc(num * sizeof(struct proc_vm_map_entry));
////	if (!info) {
////		vm_map_unlock_read(map);
////		vmspace_free(vm);
////		return 1;
////	}
////
////	for (int i = 0; i < num; i++) {
////		info[i].start = entry->start;
////		info[i].end = entry->end;
////		info[i].offset = entry->offset;
////		info[i].prot = entry->prot & (entry->prot >> 8);
////		memcpy(info[i].name, entry->name, sizeof(info[i].name));
////
////		if (!(entry = entry->next)) {
////			break;
////		}
////	}
////
////	vm_map_unlock_read(map);
////	vmspace_free(vm);
////
////	if (entries) {
////		*entries = info;
////	}
////
////	if (num_entries) {
////		*num_entries = num;
////	}
////
////	return 0;
////}
////
////int proc_rw_mem(struct proc *p, void *ptr, size_t size, void *data, size_t *n, int write) {
////	struct thread *td = curthread();
////	struct iovec iov;
////	struct uio uio;
////	int r = 0;
////
////	if (!p) {
////		return 1;
////	}
////
////	if (size == 0) {
////		if (n) {
////			*n = 0;
////		}
////
////		return 0;
////	}
////
////	memset(&iov, NULL, sizeof(iov));
////	iov.iov_base = (uint64_t)data;
////	iov.iov_len = size;
////
////	memset(&uio, NULL, sizeof(uio));
////	uio.uio_iov = (uint64_t)&iov;
////	uio.uio_iovcnt = 1;
////	uio.uio_offset = (uint64_t)ptr;
////	uio.uio_resid = (uint64_t)size;
////	uio.uio_segflg = UIO_SYSSPACE;
////	uio.uio_rw = write ? UIO_WRITE : UIO_READ;
////	uio.uio_td = td;
////
////	r = proc_rwmem(p, &uio);
////
////	if (n) {
////		*n = (size_t)((uint64_t)size - uio.uio_resid);
////	}
////
////	return r;
////}
////
////inline int proc_read_mem(struct proc *p, void *ptr, size_t size, void *data, size_t *n) {
////	return proc_rw_mem(p, ptr, size, data, n, 0);
////}
////
////inline int proc_write_mem(struct proc *p, void *ptr, size_t size, void *data, size_t *n) {
////	return proc_rw_mem(p, ptr, size, data, n, 1);
////}
////
////int proc_allocate(struct proc *p, void **address, size_t size) {
////	uint64_t addr = NULL;
////	int r = 0;
////
////	if (!address) {
////		r = 1;
////		goto error;
////	}
////
////	struct vmspace *vm = p->p_vmspace;
////	struct vm_map *map = &vm->vm_map;
////
////	vm_map_lock(map);
////
////	r = vm_map_findspace(map, NULL, size, &addr);
////	if (r) {
////		vm_map_unlock(map);
////		goto error;
////	}
////
////	r = vm_map_insert(map, NULL, NULL, addr, addr + size, VM_PROT_ALL, VM_PROT_ALL, 0);
////
////	vm_map_unlock(map);
////
////	if (r) {
////		goto error;
////	}
////
////	if (address) {
////		*address = (void *)addr;
////	}
////
////error:
////	return r;
////}
////
////int proc_deallocate(struct proc *p, void *address, size_t size) {
////	int r = 0;
////
////	struct vmspace *vm = p->p_vmspace;
////	struct vm_map *map = &vm->vm_map;
////
////	vm_map_lock(map);
////
////	r = vm_map_delete(map, (uint64_t)address, (uint64_t)address + size);
////
////	vm_map_unlock(map);
////
////	return r;
////}
////
////int proc_mprotect(struct proc *p, void *address, void *end, int new_prot) {
////	int r = 0;
////
////	uint64_t addr = (uint64_t)address;
////	uint64_t addrend = (uint64_t)end;
////
////	struct vmspace *vm = p->p_vmspace;
////	struct vm_map *map = &vm->vm_map;
////
////	r = vm_map_protect(map, addr, addrend, new_prot, 1);
////	r = vm_map_protect(map, addr, addrend, new_prot, 0);
////
////	return r;
////}
////
////
////uint64_t proc_alloc_size(uint64_t p)
////{
////	uint64_t ldrsize = p;
////	ldrsize += (PAGE_SIZE - (ldrsize % PAGE_SIZE));
////	return ldrsize;
////}
//
//void Patch505()
//{
//	notify("Patch505");
//	int ret = 0;
//
//	/*struct proc *ssc =  proc_find_by_name("SceShellCore");
//
//	if (!ssc) {
//	ret = -1;
//	goto error;
//	}
//	char buffer[180];*/
//	/*buffer[180];
//	sprintf(buffer, "Address \n0x%08x", ssc->);
//	notify(buffer);*/
//	//Install_Patch(ssc->paddr,
//	//Install_Patch();
//
//
//error:
//	/*if (entries)
//	dealloc(entries);*/
//
//	//return ret;
//	return;
//}
//
int Mira_Patch_505(void* td, void* args)
{
	uint8_t* kernel_base = (uint8_t*)(__readmsr1(0xC0000082) - K505_XFAST_SYSCALL);

	uint64_t cr0 = readCr0();
	writeCr0(cr0 & ~X86_CR0_WP);


	//// Use "kmem" for all patches
	//uint8_t *kmem;

	//// Verbose Panics
	//kmem = (uint8_t *)&kernel_base[0x00171627];
	//kmem[0] = 0x90;
	//kmem[1] = 0x90;
	//kmem[2] = 0x90;
	//kmem[3] = 0x90;
	//kmem[4] = 0x90;

	//// sceSblACMgrIsAllowedSystemLevelDebugging
	//kmem = (uint8_t *)&kernel_base[0x00010FC0];
	//kmem[0] = 0xB8;
	//kmem[1] = 0x01;
	//kmem[2] = 0x00;
	//kmem[3] = 0x00;
	//kmem[4] = 0x00;
	//kmem[5] = 0xC3;

	//kmem = (uint8_t *)&kernel_base[0x00011730];
	//kmem[0] = 0xB8;
	//kmem[1] = 0x01;
	//kmem[2] = 0x00;
	//kmem[3] = 0x00;
	//kmem[4] = 0x00;
	//kmem[5] = 0xC3;

	//kmem = (uint8_t *)&kernel_base[0x00011750];
	//kmem[0] = 0xB8;
	//kmem[1] = 0x01;
	//kmem[2] = 0x00;
	//kmem[3] = 0x00;
	//kmem[4] = 0x00;
	//kmem[5] = 0xC3;

	//// Enable mount for unprivileged user
	//kmem = (uint8_t *)&kernel_base[0x001DEBFE];
	//kmem[0] = 0x90;
	//kmem[1] = 0x90;
	//kmem[2] = 0x90;
	//kmem[3] = 0x90;
	//kmem[4] = 0x90;
	//kmem[5] = 0x90;


	//// prtinf hook patches
	//kmem = (uint8_t *)&kernel_base[0x00436136];
	//kmem[0] = 0xEB;
	//kmem[1] = 0x1E;

	//kmem = (uint8_t *)&kernel_base[0x00436154];
	//kmem[0] = 0x90;
	//kmem[1] = 0x90;




	//disable patching
	writeCr0(cr0);

	return 0;

}

void InstallPatches(int FW)
{
	initSysUtil();
	//notify("We got somewhere");
	loadAndImport();
	//notify("loadAndImport");
	if(!gKernelBase)
	{
		notify("Could not get Kernel Base Address");
		return;
	}
	switch(FW)
	{
	case 350:

		break;
	case 355:

		break;
	case 370:

		break;
	case 400:

		break;
	case 401:

		break;
	case 405:

		break;
	case 505:
		{
			notify("Enabling write Mode");
			//notify("505");
			/*kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-K505_XFAST_SYSCALL];
			gKernelBase = (uint8_t*)kernel_base;*/
			struct thread td;
			kexec(&Mira_Patch_505,NULL);
			notify("mira patching completed");
		}

		break;
	default:
		break;
	}

}


