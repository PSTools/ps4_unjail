#include "unjail.h"
#include "decrypt.h"
#include <kernel_ex.h>


#define X86_CR0_WP (1 << 16)

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


long unsigned int Temp() {

	int celsius;
	int hi = sceKernelGetCpuTemperature(&celsius);
	return celsius;

}

void SetTemp (long unsigned int celsius){
	char data[10] = {0x00, 0x00, 0x00, 0x00, 0x00, celsius, 0x00, 0x00, 0x00, 0x00};
	int hii = sceKernelSetFanShit(data);
}



int DecryptPup(const char* path){
	decrypt_pups(path,"/mnt/usb0/%s.dec");
	return 666;
}

unsigned int long long __readmsr(unsigned long __register) {
	unsigned long __edx;
	unsigned long __eax;
	__asm__("rdmsr" : "=d"(__edx), "=a"(__eax) : "c"(__register));
	return (((unsigned int long long)__edx) << 32) | (unsigned int long long)__eax;
}

void  *unjail405(struct thread *td){

	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-K405_XFAST_SYSCALL];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 =   (void**)&kernel_ptr[K405_PRISON_0];
	void** got_rootvnode = (void**)&kernel_ptr[K405_ROOTVNODE];

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


	return 0;
}

void  *unjail455(struct thread *td){

	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-K455_XFAST_SYSCALL];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 =   (void**)&kernel_ptr[K455_PRISON_0];
	void** got_rootvnode = (void**)&kernel_ptr[K455_ROOTVNODE];

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


	return 0;
}

void  *unjail474(struct thread *td){

	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-K474_XFAST_SYSCALL];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 =   (void**)&kernel_ptr[K474_PRISON_0];
	void** got_rootvnode = (void**)&kernel_ptr[K474_ROOTVNODE];

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


	return 0;
}

void  *unjail501(struct thread *td){

	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-K501_XFAST_SYSCALL];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 =   (void**)&kernel_ptr[K501_PRISON_0];
	void** got_rootvnode = (void**)&kernel_ptr[K501_ROOTVNODE];

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


	return 0;
}

void  *unjail505(struct thread *td){

	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

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


	return 0;
}

void  *unjail672(struct thread *td){

	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-K672_XFAST_SYSCALL];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 =   (void**)&kernel_ptr[K672_PRISON_0];
	void** got_rootvnode = (void**)&kernel_ptr[K672_ROOTVNODE];

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


	return 0;
}

void  *unjail702(struct thread *td){

	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-K702_XFAST_SYSCALL];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 = (void**)&kernel_ptr[K702_PRISON_0];
	void** got_rootvnode = (void**)&kernel_ptr[K702_ROOTVNODE];

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


	return 0;
}

void  *unjail750(struct thread *td){

	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-K750_XFAST_SYSCALL];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 = (void**)&kernel_ptr[K750_PRISON_0];
	void** got_rootvnode = (void**)&kernel_ptr[K750_ROOTVNODE];

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


	return 0;
}

void  *unjail751(struct thread *td){

	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-K751_XFAST_SYSCALL];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 = (void**)&kernel_ptr[K751_PRISON_0];
	void** got_rootvnode = (void**)&kernel_ptr[K751_ROOTVNODE];

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


	return 0;
}

void  *unjail755(struct thread *td){

	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-K755_XFAST_SYSCALL];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 = (void**)&kernel_ptr[K755_PRISON_0];
	void** got_rootvnode = (void**)&kernel_ptr[K755_ROOTVNODE];

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


	return 0;
}