#include <errno.h>
#include "Patcher.h"
#include "unjail.h"
#include "syscall.h"
#include <cstring>
#include <stdio.h>
#include <external\freebsd-headers\include\sys\uio.h>
#include <pthread.h>
//#include <external\freebsd-headers\include\sys\proc.h>


extern void cpu_disable_wp();
extern void cpu_enable_wp();


#define CTL_KERN        1               /* "high kernel": proc, limits */
#define KERN_PROC               14      /* struct: process entries */
#define KERN_PS_STRINGS         32      /* int: address of PS_STRINGS */

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




//struct vmspace *(*vmspace_acquire_ref)(struct proc *p);
//void (*vmspace_free)(struct vmspace *vm);
//void (*vm_map_lock_read)(struct vm_map *map);
//void (*vm_map_unlock_read)(struct vm_map *map);
//int (*vm_map_lookup_entry)(struct vm_map *map, uint64_t address, struct vm_map_entry **entries);





#define CR0_WP (1 << 16) // write protect

int (*sysctl)(const int *, u_int, void *, size_t *, const void *, size_t);


//struct kinfo_proc {
//	struct	proc kp_proc;			/* proc structure */
//	struct	eproc {
//		struct	proc *e_paddr;		/* address of proc */
//		struct	session *e_sess;	/* session pointer */
//		struct	ucred e_pcred;		/* process credentials */
//		struct	ucred e_ucred;		/* current credentials */
//		struct	vmspace e_vm;		/* address space */
//		pid_t	e_ppid;			/* parent process id */
//		pid_t	e_pgid;			/* process group id */
//		short	e_jobc;			/* job control counter */
//		dev_t	e_tdev;			/* controlling tty dev */
//		pid_t	e_tpgid;		/* tty process group id */
//		struct	session *e_tsess;	/* tty session pointer */
//#define	WMESGLEN	7
//		char	e_wmesg[WMESGLEN+1];	/* wchan message */
//		segsz_t e_xsize;		/* text size */
//		short	e_xrssize;		/* text rss */
//		short	e_xccount;		/* text references */
//		short	e_xswrss;
//		long	e_flag;
//#define	EPROC_CTTY	0x01	/* controlling tty vnode active */
//#define	EPROC_SLEADER	0x02	/* session leader */
//		char	e_login[MAXLOGNAME];	/* setlogin() name */
//		long	e_spare[4];
//	} kp_eproc;
//};



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


#pragma region << Old Patching doesn't seem to work >>

int find_process(const char* target)
{
	int pid;
	int mib[3] = {1, 14, 0};
	size_t size, count;
	char* data;
	char* proc;

	if (sysctl(mib, 3, NULL, &size, NULL, 0) < 0)
	{
		return -1;
	}

	if (size == 0)
	{
		return -2;
	}

	data = (char*)malloc(size);
	if (data == NULL)
	{
		return -3;
	}

	if (sysctl(mib, 3, data, &size, NULL, 0) < 0)
	{
		free(data);
		return -4;
	}

	count = size / 0x448;
	proc = data;
	pid = -1;
	while (count != 0)
	{

		char* name = &proc[0x1BF];
		if (strncmp(name, target, strlen(target)) == 0)
		{
			pid = *(int*)(&proc[0x48]);
			break;
		}
		proc += 0x448;
		count--;
	}

	free(data);
	return pid;
}


struct kinfo_proc {
	int structSize;
	int layout;
	void *args;
	void *paddr;
	void *addr;
	void *tracep;
	void *textvp;
	void *fd;
	void *vmspace;
	void *wchan;
	int pid;
	char useless[0x173];
	char name[];
};

int (*proc_rw_mem1)(struct kinfo_proc *p, struct uio *uio);


struct kinfo_proc* findProcess(char *procName) {
	int procPID = 0;
	while (!procPID) {
		int mib[3];
		size_t len;
		mib[0] = CTL_KERN;
		mib[1] = KERN_PROC;
		mib[2] = KERN_PROC_ALL;

		if (sysctl(mib, 3, NULL, &len, NULL, 0) != -1) {
			if (len > 0) {
				void *dump = malloc(len);
				if (dump == NULL) {
					return -1;
				}
				if (sysctl(mib, 3, dump, &len, NULL, 0) != -1) {
					int structSize = *(int *)dump;
					for (size_t i = 0; i < (len / structSize); i++) {
						struct kinfo_proc *procInfo = (struct kinfo_proc *)(dump + (i * structSize));
						if (!strcmp(procInfo->name, procName)) {
							procPID = procInfo->pid;
							return procInfo;
							break;
						}
					}
				}
				free(dump);
			}
		}

		sceKernelSleep(1);
	}
	return NULL;
}


int read_rif(const char* path, void* buffer)
{
	FILE* handle = fopen(path, "rb");
	if (handle == NULL)
	{
		return -1;
	}
	fseek(handle, 0x400, SEEK_SET);
	fread(buffer, 1, 0x400, handle);
	fclose(handle);
	return 0;
}

typedef struct _rif_info
{
	uint32_t version;
	uint32_t unknown_4; // &rif[0x6]
	uint64_t psn_account_id;
	uint64_t start_timestamp;
	uint64_t end_timestamp;
	uint64_t unknown_20;
	uint32_t type;
	uint32_t unknown_2C; // &rif[0x54]
	uint32_t sku_flag;
	uint32_t unknown_34;
	uint64_t unknown_38;
	uint64_t unknown_40;
	char content_id[48];
	uint8_t secret_key_iv[16];
}
rif_info;

typedef struct _rif_secret
{
	uint8_t content_key_seed[16];
	uint8_t self_key_seed[16];
	uint8_t entitlement_key[16];
}
rif_secret;

int npdrm_decompose_rif(rif_secret* secret, rif_info* info, void* rif)
{
	int fd = syscall(5,"/dev/npdrm", 0x0002, 0);
	if (fd < 0)
	{
		return 0x800F0A13;
	}
	struct
	{
		void* secret;
		void* info;
		void* rif;
		int error;
	}
	args;
	memset(&args, 0, sizeof(args));
	args.secret = secret;
	args.info = info;
	args.rif = rif;
	int result = syscall(54,fd, 0xC0204E0E, &args);
	syscall(6,fd);
	return result ? 0x800F0A05 : args.error;
}

int sceSblAuthMgrAddEEkc3(const char* content_id, void* keys, int field_2C, int a4)
{
	if (!content_id || !keys)
	{
		return 0x800F0B16;
	}
	int fd = syscall(5,"/dev/authmgr", 0x0002, 0);
	if (fd < 0)
	{
		return 0x800F0B13;
	}
	struct
	{
		char content_id[36];
		uint8_t keys[32];
		int field_44;
		int field_48;
		int error;
	}
	args;
	memcpy(args.content_id, content_id, sizeof(args.content_id));
	memcpy(args.keys, keys, sizeof(args.keys));
	args.field_44 = field_2C;
	args.field_48 = a4;
	int result = syscall(54,fd, 0xC0504104, &args);
	syscall(6,fd);
	return result ? 0x800F0B05 : args.error;
}

int sceSblAuthMgrDeleteEEkc(const char* content_id)
{
	if (!content_id)
	{
		return 0x800F0B16;
	}
	int fd = syscall(5,"/dev/authmgr", 0x0002, 0);
	if (fd < 0)
	{
		return 0x800F0B13;
	}
	struct
	{
		char content_id[36];
		uint8_t unknown[32];
		int error;
	}
	args;
	memcpy(args.content_id, content_id, sizeof(args.content_id));
	int result = syscall(54,fd, 0xC0484102, &args);
	syscall(6,fd);
	return result ? 0x800F0B05 : args.error;
}

uint8_t rif[0x400];


void hexdump(void* input, int length, int relative)
{
	uint8_t* buffer = (uint8_t*)input;
	int i;

	for (i = 0; i < length; i++)
	{
		if ((i % 16) == 0)
		{
			char proc_path1[164];
			sprintf(proc_path1, "%llx :", !relative ? (uint64_t)&buffer[i] : (uint64_t)i);
			notify(proc_path1);
			//printfsocket("%llx :", !relative ? (uint64_t)&buffer[i] : (uint64_t)i);
		}

		char proc_path1[164];
		sprintf(proc_path1, " %02x", buffer[i]);
		notify(proc_path1);

		//printfsocket(" %02x", buffer[i]);

		if (i > 0 && (i % 16) == 15)
		{
			//printfsocket("\n");
		}
	}

	if ((i % 16) != 0)
	{
		//printfsocket("\n");
	}

	//printfsocket("\n");
}

int mount_npdrm()
{
	//int result = syscall(136,"/mnt/npdrm", 0777);
	//if (result < 0)
	//{
	//	notify("Failed to create /mnt/npdrm\n");
	//	return -1;
	//}
	////mount syscall
	//result = syscall(21,"/dev/npdrm", "/mnt/npdrm", 0, NULL);
	//if (result < 0)
	//{
	//	notify("Failed to mount npdrm:");
	//	//notify("Failed to mount procfs: %d\n", result, *__error());
	//	return -2;
	//}
	read_rif("/user/license/freeIP9100-CUSA00001_00.rif", rif);

	rif_secret rif_secret;
	rif_info rif_info;
	memset(&rif_secret, 0xCC, sizeof(rif_secret));
	memset(&rif_info, 0xCC, sizeof(rif_info));
	int result = npdrm_decompose_rif(&rif_secret, &rif_info, rif);
	char proc_path1[164];
	sprintf(proc_path1, "npdrm_parse_rif = %x\n", result);
	notify(proc_path1);
	if (result)
	{
		return -15;
	}

	hexdump(&rif_secret, sizeof(rif_secret), 1);
	//hexdump(&rif_info, sizeof(rif_info), 1);

	struct
	{
		uint8_t content_key_seed[16];
		uint8_t self_key_seed[16];
	}
	keys;
	memcpy(keys.content_key_seed, rif_secret.content_key_seed, 16);
	memcpy(keys.self_key_seed, rif_secret.self_key_seed, 16);

	//int fd = syscall(5,"/dev/npdrm", 0x0002, 0);
	//if (fd < 0)
	//{
	//	notify("Failed to open npdrm");
	//	//printfsocket("Failed to open %s!\n", proc_path);
	//	return -2;
	//}

	return 0;
}


int mount_procfs()
{
	int result = syscall(136,"/mnt/proc", 0777);
	if (result < 0)
	{
		notify("Failed to create /mnt/proc\n");
		return -1;
	}
	//mount syscall
	result = syscall(21,"procfs", "/mnt/proc", 0, NULL);
	if (result < 0)
	{
		notify("Failed to mount procfs:");
		//notify("Failed to mount procfs: %d\n", result, *__error());
		return -2;
	}

	return 0;
}

int umount_procfs()
{

	int result = syscall(22,"/mnt/proc",MNT_FORCE);
	if (result < 0)
	{
		notify("Failed to unmount procfs:");
		//notify("Failed to mount procfs: %d\n", result, *__error());
		return -1;
	}


	return 0;
}


int ptrace(int req, int pid, void* addr, int data);
SYSCALL(ptrace,26);

void PTRACE(int req, int pid, void* addr, int data) {
	while (ptrace(req, pid, addr, data)) {
		;
	}
}

void procAttach(int pid) {
	PTRACE(PT_ATTACH, pid, NULL, NULL);
}

void procDetach(int pid) {
	PTRACE(PT_DETACH, pid, NULL, NULL);
}

void procReadBytes(int pid, void* offset, void* buffer, size_t len) {
	struct ptrace_io_desc pt_desc;
	pt_desc.piod_op = PIOD_READ_D;
	pt_desc.piod_addr = buffer;
	pt_desc.piod_offs = offset;
	pt_desc.piod_len = len;
	PTRACE(PT_IO, pid, &pt_desc, NULL);
}

void procWriteBytes(int pid, void* offset, void* buffer, size_t len) {
	struct ptrace_io_desc pt_desc;
	pt_desc.piod_op = PIOD_WRITE_D;
	pt_desc.piod_addr = buffer;
	pt_desc.piod_offs = offset;
	pt_desc.piod_len = len;
	PTRACE(PT_IO, pid, &pt_desc, NULL);
}

//void procReadBytes(int pid, void *offset, void *buffer, size_t len) {
//	struct ptrace_io_desc pt_desc;
//	pt_desc.piod_op = PIOD_READ_D;
//	pt_desc.piod_addr = buffer;
//	pt_desc.piod_offs = offset;
//	pt_desc.piod_len = len;
//	syscall(26,PT_IO, pid, &pt_desc, NULL);
//}
//
//void procWriteBytes(int pid, void *offset, void *buffer, size_t len) {
//	struct ptrace_io_desc pt_desc;
//	pt_desc.piod_op = PIOD_WRITE_D;
//	pt_desc.piod_addr = buffer;
//	pt_desc.piod_offs = offset;
//	pt_desc.piod_len = len;
//	syscall(26,PT_IO, pid, &pt_desc, NULL);
//}


int ReadInt32(int pid, uint64_t offset)
{
	int var;
	procReadBytes(pid, (void*)offset, (void*)&var, sizeof(var));
	return var;
}

uint32_t ReadUint32(int pid, uint64_t offset)
{
	uint32_t var;
	procReadBytes(pid, (void*)offset, (void*)&var, sizeof(var));
	return var;
}

long long ReadInt64(int pid, uint64_t offset)
{
	long long var;
	procReadBytes(pid, (void*)offset, (void*)&var, sizeof(var));
	return var;
}

uint64_t ReadUint64(int pid, uint64_t offset)
{
	uint64_t var;
	procReadBytes(pid, (void*)offset, (void*)&var, sizeof(var));
	return var;
}

char* ReadString(int pid, uint64_t offset, char * buffer)
{
	procReadBytes(pid, (void*)offset, (void*)buffer, 0x40);
	return buffer;
}

void WriteInt32(int pid, uint64_t offset, int var)
{
	procWriteBytes(pid, (void*)offset, (void*)&var, sizeof(var));
}

void WriteUint32(int pid, uint64_t offset, uint32_t var)
{
	procWriteBytes(pid, (void*)offset, (void*)&var, sizeof(var));
}

void WriteInt64(int pid, uint64_t offset, long long var)
{
	procWriteBytes(pid, (void*)offset, (void*)&var, sizeof(var));
}

void WriteUint64(int pid, uint64_t offset, uint64_t var)
{
	procWriteBytes(pid, (void*)offset, (void*)&var, sizeof(var));
}

void WriteString(int pid, uint64_t offset, const char * var)
{
	procWriteBytes(pid, (void*)offset, (void*)var, strlen(var) + 1);
}

void WriteBytes(int pid, uint64_t offset, uint8_t var[])
{
	procWriteBytes(pid, (void*)offset, (void*)var, 3);
}

void WriteBytes2(int pid, uint64_t offset, uint8_t *var, size_t len)
{
	procWriteBytes(pid, (void*)offset, (void*)var, len);
}

int get_code_info(int pid, uint64_t* paddress, uint64_t* psize, uint64_t known_size)
{
	int mib[4] = {1, 14, 32, pid};
	size_t size, count;
	char* data;
	char* entry;

	if (sysctl(mib, 4, NULL, &size, NULL, 0) < 0)
	{
		notify("sysctl failed");
		return -1;
	}

	if (size == 0)
	{
		notify("size == 0");
		return -2;
	}

	data = (char*)malloc(size);
	if (data == NULL)
	{
		notify("data == NULL");
		return -3;
	}

	if (sysctl(mib, 4, data, &size, NULL, 0) < 0)
	{
		notify("sysctl failed");
		free(data);
		return -4;
	}


	int struct_size = *(int*)data;
	count = size / struct_size;
	entry = data;

	struct kinfo_proc info1;
	size_t length = sizeof(struct kinfo_proc);
	if (sysctl(mib, 4, &info1, &length, NULL, 0) == 0)
	{
		notify("Doing this now please work");
		uint64_t *text_seg_base = NULL;
		//struct proc_vm_map_entry *info = NULL;
		//struct vm_map_entry *entry = NULL;

		////struct vmspace *vm = info1.p_vmspace;
		//if (!info1.p_vmspace)return -15;

		//struct vm_map *map = info1.p_vms.->vm_map;
		////return -7;

		//int num = map->nentries;
		//if (!num) {
		//	//vmspace_free(vm);
		//	return -155;
		//}

		//info = (struct proc_vm_map_entry *)malloc(num * sizeof(struct proc_vm_map_entry));
		//if (!info) {
		//	return -19;
		//}

		//for (int i = 0; i < num; i++) {
		//	info[i].start = entry->start;
		//	info[i].end = entry->end;
		//	info[i].offset = entry->offset;
		//	info[i].prot = entry->prot & (entry->prot >> 8);
		//	memcpy(info[i].name, entry->name, sizeof(info[i].name));

		//	if (!(entry = entry->next))
		//		break;
		//}
		//
		//for (int i = 0; i < num; i++) {
		//	if (info[i].prot == (PROT_READ | PROT_EXEC)) {
		//		text_seg_base = (uint64_t *)info[i].start;
		//		uint64_t start_addr = *(uint64_t*)(info[i].start);
		//		uint64_t end_addr = *(uint64_t*)(info[i].end);
		//		*paddress = *text_seg_base;
		//		*psize = (end_addr - start_addr);
		//		break;
		//	}
		//}
		if (!text_seg_base) {
			return -100;
		}

		return 0;

	}

	int found = 0;
	while (count != 0)
	{

		int type = *(int*)(&entry[0x4]);
		uint64_t start_addr = *(uint64_t*)(&entry[0x8]);
		uint64_t end_addr = *(uint64_t*)(&entry[0x10]);
		uint64_t code_size = end_addr - start_addr;
		uint32_t prot = *(uint32_t*)(&entry[0x38]);
		//char* name = &entry[0x20];
		//notify(name);
		char proc_path1[164];
		sprintf(proc_path1, "%d %llx %llx (%llu) %x\n", type, start_addr, end_addr, code_size, prot);

		//printfsocket("%d %llx %llx (%llu) %x\n", type, start_addr, end_addr, code_size, prot);

		if (prot == 5 && type == 9) //type 9 is excecutable ?
		{
			//notify(name);
			//notify("Found");
			*paddress = start_addr;
			*psize = (end_addr - start_addr);
			found = 1;
			break;
		}

		entry += struct_size;
		count--;
	}

	free(data);
	return !found ? -5 : 0;
}

typedef struct _patch_info
{
	const char* name;
	uint32_t address;
	const char* data;
	uint32_t size;
}
patch_info;

size_t int2size_t(int val) {
	return (val < 0) ? __SIZE_MAX__ : (size_t)((unsigned)val);
}

int AttachToProcess(struct thread1 *td,int PID)
{
	return syscall(26,PT_ATTACH, PID, NULL, NULL);
}


int createThread(void*(func)(void*), void* args)
{
	ScePthread sceThread;
	return scePthreadCreate(&sceThread, NULL, func, args, "PS4 Tools Thread") == 0;
}


//int proc_rw_mem(struct kinfo_proc *p, void *ptr, size_t size, void *data, size_t *n, int write) {
//	struct thread *td = curthread();
//	struct iovec iov;
//	struct uio uio;
//	int r = 0;
//
//	if (!p) {
//		return 1;
//	}
//
//	if (size == 0) {
//		if (n) {
//			*n = 0;
//		}
//
//		return 0;
//	}
//
//	memset(&iov, NULL, sizeof(iov));
//	iov.iov_base = (uint64_t)data;
//	iov.iov_len = size;
//
//	memset(&uio, NULL, sizeof(uio));
//	uio.uio_iov = (uint64_t)&iov;
//	uio.uio_iovcnt = 1;
//	uio.uio_offset = (uint64_t)ptr;
//	uio.uio_resid = (uint64_t)size;
//	uio.uio_segflg = UIO_SYSSPACE;
//	uio.uio_rw = write ? UIO_WRITE : UIO_READ;
//	uio.uio_td = td;
//
//	r = proc_rwmem(p, &uio);
//
//	if (n) {
//		*n = (size_t)((uint64_t)size - uio.uio_resid);
//	}
//
//	return r;
//}

int apply_pacthes1(struct kinfo_proc* p , patch_info* patches)
{
	uint64_t code_address, code_size;
	int result = get_code_info(p->pid, &code_address, &code_size, 0);
	if (result < 0)
	{
		char proc_path1[164];
		sprintf(proc_path1, "Failed to get code info for %d: %d\n", p->pid, result);
		notify(proc_path1);
		//printfsocket("Failed to get code info for %d: %d\n", pid, result);
		return -1;
	}

	char proc_path[64];
	sprintf(proc_path, "/mnt/proc/%d/mem", p->pid);
	notify(proc_path);
	//open
	int fd = syscall(5,proc_path, 0x0002, 0);
	if (fd < 0)
	{
		notify("Failed to open");
		//printfsocket("Failed to open %s!\n", proc_path);
		return -2;
	}
	//notify("Opened process memory");
	//printfsocket("Opened process memory...\n");

	//disable mprotect first
	//mprotect



	/*resolve(vmspace_acquire_ref);
	resolve(vmspace_free);
	resolve(vm_map_lock_read);
	resolve(vm_map_unlock_read);
	resolve(vm_map_lookup_entry);*/




	//Set protection to all
	//sceKernelMprotect((void*)Address, Length,

	for (int i = 0; patches[i].name != NULL; i++)
	{
		off_t addressinmem = lseek(fd, code_address + patches[i].address, SEEK_SET);
		size_t n;
		char proc_path1[164];
		sprintf(proc_path1, "Address %llx\n", addressinmem);
		notify(proc_path1);
		////mem protect
		//procWriteBytes(p->pid,(void*)(code_address + patches[i].address),(void*)patches[i].data,patches[i].size);
		//result = syscall(74,(void*)addressinmem,patches[i].size,VM_PROT_ALL);
		if(result != 0)
		{
			sprintf(proc_path1, "patch %s: %d\n", patches[i].name, result);
			notify(proc_path1);
			sprintf(proc_path, "Value of errno: %d\n", errno);
			notify(proc_path);
			sprintf(proc_path,  "Error: %s\n", strerror( errno ));
			notify(proc_path);
		}
		addressinmem =lseek(fd, code_address + patches[i].address, SEEK_SET);//go back to the start and read again
		result = proc_rw_mem(p,(void*)(addressinmem),patches[i].size,&patches[i].data ,&n,1);//offset you wanne change
		//result = syscall(476,fd, patches[i].data, sizeof(patches[i].data), patches[i].size);//	write

		//char proc_path1[164];
		if(result != 0)
		{
			sprintf(proc_path1, "patch %s: %d\n", patches[i].name, result);
			notify(proc_path1);
			sprintf(proc_path, "Value of errno: %d\n", errno);
			notify(proc_path);
			sprintf(proc_path,  "Error: %s\n", strerror( errno ));
			notify(proc_path);
		}
		//printfsocket("patch %s: %d %d\n", patches[i].name, result, result < 0 ? errno : 0);
	}
	//result = syscall(26,PT_DETACH, p, NULL, NULL);
	syscall(6,fd);
	//sceKernelClose(fd);
	return 0;
}

int apply_patches(int pid, uint64_t known_size, patch_info* patches)
{
	uint64_t code_address, code_size;
	int result = get_code_info(pid, &code_address, &code_size, known_size);
	if (result < 0)
	{
		char proc_path1[164];
		sprintf(proc_path1, "Failed to get code info for %d: %d\n", pid, result);
		notify(proc_path1);
		//printfsocket("Failed to get code info for %d: %d\n", pid, result);
		return -1;
	}

	char proc_path[64];
	sprintf(proc_path, "/mnt/proc/%d/mem", pid);
	//notify(proc_path);
	//open
	int fd = syscall(5,proc_path, 0x0002, 0);
	if (fd < 0)
	{
		notify("Failed to open");
		//printfsocket("Failed to open %s!\n", proc_path);
		return -2;
	}
	//notify("Opened process memory");
	//printfsocket("Opened process memory...\n");

	//disable mprotect first
	//mprotect

	//Set protection to all
	//sceKernelMprotect((void*)Address, Length,
	notify("Attaching to process");
	//procAttach(pid);
	notify("Attached to process");

	for (int i = 0; patches[i].name != NULL; i++)
	{
		off_t addressinmem = lseek(fd, code_address + patches[i].address, SEEK_SET);
		char proc_path1[164];
		sprintf(proc_path1, "Address %llx\n", addressinmem);
		notify(proc_path1);
		//mem protect
		result = syscall(74,(void*)addressinmem,patches[i].size,VM_PROT_ALL);
		addressinmem =lseek(fd, code_address + patches[i].address, SEEK_SET);//go back to the start and read again
		/*	sprintf(proc_path1, "Address %llx\n", addressinmem);
		notify(proc_path1);*/
		//we first want to read the value
		// ssize_t
		//read(int fd, void *buf, size_t nbytes);



		//int res = syscall(477,0, int2size_t(patches[i].size), VM_PROT_ALL, 0x1000 | 0x2, -1, 0, &OriginalData);
		//if (res < 0)
		//{
		//	//klog("[Patcher] sceKernelMmap Failed: 0x%llX\n", res);
		//	return -190;
		//}

		//memcpy(OriginalData, patches[i].data, int2size_t(patches[i].size));

		//Write Patch.
		//memcpy((void*)addressinmem,  patches[i].data, int2size_t(patches[i].size));
		//procWriteBytes(pid,(void*)(code_address + patches[i].address),&patches[i].data,sizeof(patches[i].data));
		result = syscall(4,fd, patches[i].data, patches[i].size);//	write

		//char proc_path1[164];
		sprintf(proc_path1, "patch %s: %d\n", patches[i].name, result);
		notify(proc_path1);
		//printfsocket("patch %s: %d %d\n", patches[i].name, result, result < 0 ? errno : 0);
	}



	//procDetach(pid);
	sceKernelSleep(5);
	syscall(6,fd);
	//sceKernelClose(fd);
	return 0;
}

patch_info K505_shellcore_patches[32] =
{
	{"sce_sdmemory_patch",K505_sce_sdmemory_patch,"\x00",1},// 'sce_sdmemory' patch
	{"verify_keystone_patch",K505_verify_keystone_patch,"\x48\x31\xC0\xC3",4},//verify keystone patch
	{"save_mount_permision",K505_save_mount_permision,"\x31\xC0\xC3",3}, //transfer mount permission patch eg mount foreign saves with write permission
	{"save_psn_patch",K505_save_psn_patch,"\x31\xC0\xC3",3},// 0x31, 0xC0, 0xC3//patch psn check to load saves saves foreign to current account
	{"save_mount_patch",K505_save_mount_patch,"\x90\x90",2}, // ^
	{"save_mount_something_patch",K505_save_mount_something_patch,"\x90\x90\x90\x90\x90\x90",6},  // something something patches... 
	{"save_mount_patch_unk",K505_save_mount_patch_unk,"\x90\x90\x90\x90\x90\x90",6},  // don't even remember doing this
	{"save_mount_never_jump",K505_save_mount_never_jump,"\x90\x90",2},  //nevah jump
	{"save_mount_always_jump",K505_save_mount_always_jump,"\x90\xE9",2},  //nevah jump
	{ NULL, 0, NULL, 0 },//required to end forloop for patching
};

#pragma endregion << Old Patching doesn't seem to work >>

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



	uint8_t* kernel_base = (uint8_t*)(__readmsr(0xC0000082) - K505_XFAST_SYSCALL);

	cpu_disable_wp();
	// Patch something



	//// Use "kmem" for all patches
	uint8_t *kmem;

	// Verbose Panics
	kmem = (uint8_t *)&kernel_base[0x00171627];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;

	// sceSblACMgrIsAllowedSystemLevelDebugging
	kmem = (uint8_t *)&kernel_base[0x00010FC0];
	kmem[0] = 0xB8;
	kmem[1] = 0x01;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;
	kmem[5] = 0xC3;

	kmem = (uint8_t *)&kernel_base[0x00011730];
	kmem[0] = 0xB8;
	kmem[1] = 0x01;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;
	kmem[5] = 0xC3;

	kmem = (uint8_t *)&kernel_base[0x00011750];
	kmem[0] = 0xB8;
	kmem[1] = 0x01;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;
	kmem[5] = 0xC3;

	// Enable mount for unprivileged user
	kmem = (uint8_t *)&kernel_base[0x001DEBFE];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;


	// prtinf hook patches
	kmem = (uint8_t *)&kernel_base[0x00436136];
	kmem[0] = 0xEB;
	kmem[1] = 0x1E;

	kmem = (uint8_t *)&kernel_base[0x00436154];
	kmem[0] = 0x90;
	kmem[1] = 0x90;





	// Restore write protection
	cpu_enable_wp();

	return 0;

}
struct uio {
	struct	iovec1 *uio_iov;		/* scatter/gather list */
	int	uio_iovcnt;		/* length of scatter/gather list */
	off_t	uio_offset;		/* offset in target object */
	ssize_t	uio_resid;		/* remaining bytes to process */
	enum	uio_seg uio_segflg;	/* address space */
	enum	uio_rw uio_rw;		/* operation */
	struct	thread1 *uio_td;		/* owner */
};

//int proc_write_mem(struct kinfo_proc *p, void *ptr, size_t size, void *data, size_t *n) 
//{
//	return proc_rw_mem(p, ptr, size, data, n, 1);
//}
//int proc_rw_mem(struct kinfo_proc *p, void *ptr, size_t size, void *data, size_t *n, int write)
//{
//
//	struct iovec iov;
//	struct uio uio;
//	int r = 0;
//
//	if (!p)
//		return -1;
//
//	if (size == 0) {
//		if (n)
//			*n = 0;
//
//		return 0;
//	}
//
//	memset(&iov, 0, sizeof(iov));
//	iov.iov_base = data;
//	iov.iov_len = size;
//
//	memset(&uio, 0, sizeof(uio));
//	uio.uio_iov = &iov;
//	uio.uio_iovcnt = 1;
//	uio.uio_offset = (uint64_t)ptr;
//	uio.uio_resid = (uint64_t)size;
//	uio.uio_segflg = UIO_SYSSPACE;
//	uio.uio_rw = write ? UIO_WRITE : UIO_READ;
//	//uio.uio_td = td;
//
//	//r = proc_rwmem(p, &uio);
//
//	if (n)
//		*n = (size_t)((uint64_t)size - uio.uio_resid);
//
//	return r;
//}

int patcher(struct thread1 *td){

	struct ucred* cred;
	struct filedesc1* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;
	//notify("We made it this far");
	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-K505_XFAST_SYSCALL];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 =   (void**)&kernel_ptr[K505_PRISON_0];
	void** got_rootvnode = (void**)&kernel_ptr[K505_ROOTVNODE];

	cred->cr_uid = 0;
	cred->cr_ruid = 0;
	cred->cr_rgid = 0;
	cred->cr_groups[0] = 0;

	cred->cr_prison = *got_prison0;
	fd->fd_rdir = fd->fd_jdir = *got_rootvnode;

	// escalate ucred privs, needed for access to the filesystem ie* mounting & decrypting files
	void *td_ucred = *(void **)(((char *)td) + 304); // p_ucred == td_ucred

	// sceSblACMgrIsSystemUcred
	uint64_t *sonyCred = (uint64_t *)(((char *)td_ucred) + 96);
	*sonyCred = 0xffffffffffffffff;

	// sceSblACMgrGetDeviceAccessType
	uint64_t *sceProcType = (uint64_t *)(((char *)td_ucred) + 88);
	*sceProcType = 0x3801000000000013; // Max access

	// sceSblACMgrHasSceProcessCapability
	uint64_t *sceProcCap = (uint64_t *)(((char *)td_ucred) + 104);
	*sceProcCap = 0xffffffffffffffff; // Sce Process

	// Disable write protection
	cpu_disable_wp();


	// Patch something


	//Use "kmem" for all patches
	uint8_t *kmem;

	//// Verbose Panics
	//kmem = (uint8_t *)&kernel_ptr[0x00171627];
	//kmem[0] = 0x90;
	//kmem[1] = 0x90;
	//kmem[2] = 0x90;
	//kmem[3] = 0x90;
	//kmem[4] = 0x90;

	//// sceSblACMgrIsAllowedSystemLevelDebugging
	//kmem = (uint8_t *)&kernel_ptr[0x00010FC0];
	//kmem[0] = 0xB8;
	//kmem[1] = 0x01;
	//kmem[2] = 0x00;
	//kmem[3] = 0x00;
	//kmem[4] = 0x00;
	//kmem[5] = 0xC3;

	//kmem = (uint8_t *)&kernel_ptr[0x00011730];
	//kmem[0] = 0xB8;
	//kmem[1] = 0x01;
	//kmem[2] = 0x00;
	//kmem[3] = 0x00;
	//kmem[4] = 0x00;
	//kmem[5] = 0xC3;

	//kmem = (uint8_t *)&kernel_ptr[0x00011750];
	//kmem[0] = 0xB8;
	//kmem[1] = 0x01;
	//kmem[2] = 0x00;
	//kmem[3] = 0x00;
	//kmem[4] = 0x00;
	//kmem[5] = 0xC3;

	//// Enable mount for unprivileged user
	//kmem = (uint8_t *)&kernel_ptr[0x001DEBFE];
	//kmem[0] = 0x90;
	//kmem[1] = 0x90;
	//kmem[2] = 0x90;
	//kmem[3] = 0x90;
	//kmem[4] = 0x90;
	//kmem[5] = 0x90;


	//// prtinf hook patches
	//kmem = (uint8_t *)&kernel_ptr[0x00436136];
	//kmem[0] = 0xEB;
	//kmem[1] = 0x1E;

	//kmem = (uint8_t *)&kernel_ptr[0x00436154];
	//kmem[0] = 0x90;
	//kmem[1] = 0x90;

	//(uint8_t *)&kernel_ptr[KERN_PTRACE_CHECK] = 0xEB;

	/*kmem = (uint8_t *)&kernel_ptr[0x30D9AA];
	kmem[0] = 0xEB;*/


	kmem = (uint8_t *)&kernel_ptr[K505_TARGET_ID];
	//kmem = (uint8_t *)tid_patch;
	kmem[0] = 0x82;


	kmem = (uint8_t *)&kernel_ptr[K505_NPDRM_OPEN];
	kmem[0] = 0x31;
	kmem[1] = 0xC0;
	kmem[2] = 0xC3;

	kmem = (uint8_t *)&kernel_ptr[K505_NPDRM_CLOSE];;
	kmem[0] = 0x31;
	kmem[1] = 0xC0;
	kmem[2] = 0xC3;


	// This may change depending on new firmware's function structure
	kmem = (uint8_t *)&kernel_ptr[K505_NPDRM_IOCTL];
	kmem[0] = 0xEB;
	//if (fw_version < 500) {
	//	// 3.50-4.74
	//	kmem[1] = 0x04;
	//} else {
	// 5.00-9.00
	kmem[1] = 0x00;
	//}


	// Restore write protection
	cpu_enable_wp();







	return 0;
}
static inline struct thread1 *curthread(void) {
	struct thread1* td;
	__asm__ volatile (
		"mov %0, %%gs:0"
		: "=r"(td)
		);

	return td;
}

struct iovec1 {
	uint64_t iov_base;
	size_t iov_len;
};

int proc_rw_mem(struct kinfo_proc *p, void *ptr, size_t size, void *data, size_t *n, int write)
{
	notify("proc_rw_mem");
	struct thread1 *td = curthread();
	struct iovec1 iov;
	struct uio uio;
	int r = 0;

	if (!p)
		return -1;

	if (size == 0) {
		if (n)
			*n = 0;

		return 0;
	}
	notify("mem set iov");
	memset(&iov, 0, sizeof(iov));
	iov.iov_base = (uint64_t)data;
	iov.iov_len = size;
	notify("mem set uio");
	
	memset(&uio, 0, sizeof(uio));
	uio.uio_iov = &iov;
	uio.uio_iovcnt = 1;
	uio.uio_offset = (uint64_t)ptr;
	uio.uio_resid = (uint64_t)size;
	uio.uio_segflg = UIO_SYSSPACE;
	uio.uio_rw = write ? UIO_WRITE : UIO_READ;
	uio.uio_td = td;
	
	notify("mem proc_rw_mem1 uio");
	r = proc_rw_mem1(p, &uio);

	if (n)
		*n = (size_t)((uint64_t)size - uio.uio_resid);

	return r;
}
void Resolve()
{
	resolve(proc_rw_mem1);
}

void InstallPatches(int FW)
{
	//kdlsym(proc_rwmem,int (*proc_rwmem)(struct proc *p, struct uio *uio););
	//kdlsym(proc_rwmem,int (*proc_rwmem)(struct kinfo_proc *p, struct uio *uio););


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
			notify("resolving");
			Resolve();
			//notify("resolving");
			bool EnableOldPathcer = true;
			notify("Enabling write Mode 505");
			//notify("505");
			/*kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-K505_XFAST_SYSCALL];
			gKernelBase = (uint8_t*)kernel_base;*/
			struct thread1 td;
			syscall(11,patcher,td);
			//save data patches

			if(EnableOldPathcer == true)
			{
				int result;

				printf("Hello From Klog");
				struct kinfo_proc* shell_pid = findProcess("SceShellCore");
				if (shell_pid < 0)
				{
					notify("shell PID issue");
					return;
					//return -15;
				}
				//new test
				//struct proc* pid = findProcess("SceShellCore");
				//if (pid->p_pid > 0)
				//{
				//	//int result = ptrace(PT_ATTACH, pid->pid, NULL, NULL);
				//	//char proc_path1[100];
				//	//sprintf(proc_path1, "Ptrace %d\n", result);
				//	//notify(proc_path1);
				//	///*	char buffer[100];
				//	//procReadBytes(pid,(void*)patches[i].data,buffer,patches[i].size);
				//	//printf(proc_path1, "buffer %s\n", buffer);
				//	//notify(proc_path1);*/
				//	//uint8_t bytes[] = { 0x00};
				//	//uint8_t keystone_patch[] = { 0x48,0x31,0xC0,0xC3};
				//	//uint64_t code_address, code_size;
				//	//int result = get_code_info(pid->pid, &code_address, &code_size,0);
				//	//size_t n;
				//	//

				//	//procWriteBytes(pid->pid, (void*)K505_sce_sdmemory_patch,bytes, sizeof(bytes));//offset you wanne change
				//	//procWriteBytes(pid->pid, (void*)K505_verify_keystone_patch,keystone_patch, sizeof(keystone_patch));//offset you wanne change

				//	//int result = ptrace(PT_DETACH, pid->pid, NULL, NULL);
				//}

				result = mount_procfs();
				if (result)
				{
					notify("mount_procfs failed");
					return;
				}


				if(apply_pacthes1(shell_pid,K505_shellcore_patches) != 0)
				{
					notify("Could not patch SceShellCore");
				}
				result = umount_procfs();
				if(result)
				{
					notify("unmount_procfs failed");
					return;
				}

			}
			else
			{
				int result = mount_npdrm();
				if (result)
				{
					notify("mount_npdrm failed");
					return;
				}
			}

			//new patcher something like ps4debug



		}
		break;
	default:
		break;
	}

}



//ptrace
//syscall(26