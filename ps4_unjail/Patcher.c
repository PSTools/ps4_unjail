
#include "Patcher.h"

#define nullptr ((void*)0)

#define X86_CR0_WP (1 << 16)
//
// Used in every payload that uses jailbreak();
#define K300_XFAST_SYSCALL         0x0
#define K310_XFAST_SYSCALL         0x0
#define K311_XFAST_SYSCALL         0x0
#define K315_XFAST_SYSCALL         0x0
#define K350_XFAST_SYSCALL         0x003A1AD0
#define K355_XFAST_SYSCALL         0x003A1F10
#define K370_XFAST_SYSCALL         0x003A2000
#define K400_XFAST_SYSCALL         0x0030EA00
#define K401_XFAST_SYSCALL         0x0030EA00
#define K405_XFAST_SYSCALL         0x0030EB30
#define K406_XFAST_SYSCALL         0x0030EB40
#define K407_XFAST_SYSCALL         0x0030EB40
#define K450_XFAST_SYSCALL         0x003095D0
#define K455_XFAST_SYSCALL         0x003095D0
#define K470_XFAST_SYSCALL         0x0030B840
#define K471_XFAST_SYSCALL         0x0030B7D0
#define K472_XFAST_SYSCALL         0x0030B7D0
#define K473_XFAST_SYSCALL         0x0030B7D0
#define K474_XFAST_SYSCALL         0x0030B7D0
#define K500_XFAST_SYSCALL         0x000001C0
#define K501_XFAST_SYSCALL         0x000001C0
#define K503_XFAST_SYSCALL         0x000001C0
#define K505_XFAST_SYSCALL         0x000001C0
#define K507_XFAST_SYSCALL         0x000001C0
#define K550_XFAST_SYSCALL         0x000001C0
#define K553_XFAST_SYSCALL         0x000001C0
#define K555_XFAST_SYSCALL         0x000001C0
#define K556_XFAST_SYSCALL         0x000001C0
#define K600_XFAST_SYSCALL         0x000001C0
#define K602_XFAST_SYSCALL         0x000001C0
#define K620_XFAST_SYSCALL         0x000001C0
#define K650_XFAST_SYSCALL         0x000001C0
#define K651_XFAST_SYSCALL         0x000001C0
#define K670_XFAST_SYSCALL         0x000001C0
#define K671_XFAST_SYSCALL         0x000001C0
#define K672_XFAST_SYSCALL         0x000001C0
#define K700_XFAST_SYSCALL         0x000001C0
#define K701_XFAST_SYSCALL         0x000001C0
#define K702_XFAST_SYSCALL         0x000001C0
#define K750_XFAST_SYSCALL         0x000001C0
#define K751_XFAST_SYSCALL         0x000001C0
#define K755_XFAST_SYSCALL         0x000001C0
#define K800_XFAST_SYSCALL         0x000001C0
#define K801_XFAST_SYSCALL         0x0
#define K803_XFAST_SYSCALL         0x0

// Used in every payload that uses jailbreak();
#define K300_PRISON_0              0x0
#define K310_PRISON_0              0x0
#define K311_PRISON_0              0x0
#define K315_PRISON_0              0x0
#define K350_PRISON_0              0x00EF5A00
#define K355_PRISON_0              0x00EF5A00
#define K370_PRISON_0              0x00EFEF10
#define K400_PRISON_0              0x00F26010
#define K401_PRISON_0              0x00F26010
#define K405_PRISON_0              0x00F26010
#define K406_PRISON_0              0x00F26010
#define K407_PRISON_0              0x00F2A010
#define K450_PRISON_0              0x010399B0
#define K455_PRISON_0              0x010399B0
#define K470_PRISON_0              0x01042AB0
#define K471_PRISON_0              0x01042AB0
#define K472_PRISON_0              0x01042AB0
#define K473_PRISON_0              0x01042AB0
#define K474_PRISON_0              0x01042AB0
#define K500_PRISON_0              0x010986A0
#define K501_PRISON_0              0x010986A0
#define K503_PRISON_0              0x010986A0
#define K505_PRISON_0              0x010986A0
#define K507_PRISON_0              0x010986A0
#define K550_PRISON_0              0x01134180
#define K553_PRISON_0              0x01134180
#define K555_PRISON_0              0x01139180
#define K556_PRISON_0              0x01139180
#define K600_PRISON_0              0x01139458
#define K602_PRISON_0              0x01139458
#define K620_PRISON_0              0x0113D458
#define K650_PRISON_0              0x0113D4F8
#define K651_PRISON_0              0x0113D4F8
#define K670_PRISON_0              0x0113E518
#define K671_PRISON_0              0x0113E518
#define K672_PRISON_0              0x0113E518
#define K700_PRISON_0              0x0113E398
#define K701_PRISON_0              0x0113E398
#define K702_PRISON_0              0x0113E398
#define K750_PRISON_0              0x0113B728
#define K751_PRISON_0              0x0113B728
#define K755_PRISON_0              0x0113B728
#define K800_PRISON_0              0x0111A7D0
#define K801_PRISON_0              0x0
#define K803_PRISON_0              0x0

// Used in every payload that uses jailbreak();
#define K300_ROOTVNODE             0x0
#define K310_ROOTVNODE             0x0
#define K311_ROOTVNODE             0x0
#define K315_ROOTVNODE             0x0
#define K350_ROOTVNODE             0x01963000
#define K355_ROOTVNODE             0x01963040
#define K370_ROOTVNODE             0x0196F040
#define K400_ROOTVNODE             0x0206D250
#define K401_ROOTVNODE             0x0206D250
#define K405_ROOTVNODE             0x0206D250
#define K406_ROOTVNODE             0x0206D250
#define K407_ROOTVNODE             0x02071250
#define K450_ROOTVNODE             0x021AFA30
#define K455_ROOTVNODE             0x021AFA30
#define K470_ROOTVNODE             0x021B89E0
#define K471_ROOTVNODE             0x021B89E0
#define K472_ROOTVNODE             0x021B89E0
#define K473_ROOTVNODE             0x021B89E0
#define K474_ROOTVNODE             0x021B89E0
#define K500_ROOTVNODE             0x022C19F0
#define K501_ROOTVNODE             0x022C19F0
#define K503_ROOTVNODE             0x022C1A70
#define K505_ROOTVNODE             0x022C1A70
#define K507_ROOTVNODE             0x022C1A70
#define K550_ROOTVNODE             0x022EF570
#define K553_ROOTVNODE             0x022EF570
#define K555_ROOTVNODE             0x022F3570
#define K556_ROOTVNODE             0x022F3570
#define K600_ROOTVNODE             0x021BFAC0
#define K602_ROOTVNODE             0x021BFAC0
#define K620_ROOTVNODE             0x021C3AC0
#define K650_ROOTVNODE             0x02300320
#define K651_ROOTVNODE             0x02300320
#define K670_ROOTVNODE             0x02300320
#define K671_ROOTVNODE             0x02300320
#define K672_ROOTVNODE             0x02300320
#define K700_ROOTVNODE             0x022C5750
#define K701_ROOTVNODE             0x022C5750
#define K702_ROOTVNODE             0x022C5750
#define K750_ROOTVNODE             0x01B463E0
#define K751_ROOTVNODE             0x01B463E0
#define K755_ROOTVNODE             0x01B463E0
#define K800_ROOTVNODE             0x01B8C730
#define K801_ROOTVNODE             0x0
#define K803_ROOTVNODE             0x0


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



unsigned int long long __readmsr1(unsigned long __register) {
	unsigned long __edx;
	unsigned long __eax;
	__asm__("rdmsr" : "=d"(__edx), "=a"(__eax) : "c"(__register));
	return (((unsigned int long long)__edx) << 32) | (unsigned int long long)__eax;
}
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

uint8_t* gKernelBase = NULL;
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
void Mira_Patch_505()
{
	notify("mira patching");
	// Use "kmem" for all patches
	uint8_t *kmem;

	// Verbose Panics
	kmem = (uint8_t *)&gKernelBase[0x00171627];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;

	// sceSblACMgrIsAllowedSystemLevelDebugging
	kmem = (uint8_t *)&gKernelBase[0x00010FC0];
	kmem[0] = 0xB8;
	kmem[1] = 0x01;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;
	kmem[5] = 0xC3;

	kmem = (uint8_t *)&gKernelBase[0x00011730];
	kmem[0] = 0xB8;
	kmem[1] = 0x01;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;
	kmem[5] = 0xC3;

	kmem = (uint8_t *)&gKernelBase[0x00011750];
	kmem[0] = 0xB8;
	kmem[1] = 0x01;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;
	kmem[5] = 0xC3;

	// Enable mount for unprivileged user
	kmem = (uint8_t *)&gKernelBase[0x001DEBFE];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;


	// prtinf hook patches
	kmem = (uint8_t *)&gKernelBase[0x00436136];
	kmem[0] = 0xEB;
	kmem[1] = 0x1E;

	kmem = (uint8_t *)&gKernelBase[0x00436154];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	notify("mira patching completed");
}

void InstallPatches(int FW)
{
	initSysUtil();
	//notify("We got somewhere");
	loadAndImport();
	//notify("loadAndImport");
	void* kernel_base;

	switch(FW)
	{
	case 350:
		kernel_base = &((uint8_t*)__readmsr1(0xC0000082))[-K350_XFAST_SYSCALL];
		gKernelBase = (uint8_t*)kernel_base;
		break;
	case 355:
		gKernelBase = (uint8_t*)__readmsr1(0xC0000082) - K355_XFAST_SYSCALL;
		break;
	case 370:
		gKernelBase = (uint8_t*)__readmsr1(0xC0000082) - K370_XFAST_SYSCALL;
		break;
	case 400:
		gKernelBase = (uint8_t*)__readmsr1(0xC0000082) - K400_XFAST_SYSCALL;
		break;
	case 401:
		gKernelBase = (uint8_t*)__readmsr1(0xC0000082) - K401_XFAST_SYSCALL;
		break;
	case 405:
		gKernelBase = (uint8_t*)__readmsr1(0xC0000082) - K405_XFAST_SYSCALL;
		break;
	case 505:
		//notify("505");
		kernel_base = &((uint8_t*)__readmsr1(0xC0000082))[-K505_XFAST_SYSCALL];
		notify("kernel_base");
		gKernelBase = (uint8_t*)kernel_base;
		notify("gbase");
		//I will be using this as a test 
		Mira_Patch_505();
		break;
	default:
		break;
	}

}


