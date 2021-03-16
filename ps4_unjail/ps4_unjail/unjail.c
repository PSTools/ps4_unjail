#include "unjail.h"
#include "decrypt.h"


#define X86_CR0_WP (1 << 16)

#define	KERN_405_XFAST_SYSCALL		0x30EB30	// #4
#define	KERN_455_XFAST_SYSCALL		0x3095D0	// #2
#define	KERN_474_XFAST_SYSCALL		0x30B7D0	// #3
#define	KERN_501_XFAST_SYSCALL		0x1C0		// #1
#define	KERN_505_XFAST_SYSCALL		0x1C0		// #1
#define	KERN_672_XFAST_SYSCALL		0x1C0		// #1
#define KERN_702_XFAST_SYSCALL      0x1C0		// #1

#define KERN_405_PRISON_0		0xF26010
#define KERN_455_PRISON_0		0x10399B0
#define KERN_474_PRISON_0		0x1042AB0
#define KERN_501_PRISON_0		0x10986A0
#define KERN_505_PRISON_0		0x10986A0
#define KERN_672_PRISON_0		0x113E518
#define KERN_702_PRISON_0       0x0113E398

#define KERN_405_ROOTVNODE		0x206D250
#define KERN_455_ROOTVNODE		0x21AFA30
#define KERN_474_ROOTVNODE		0x21B89E0
#define KERN_501_ROOTVNODE		0x22C19F0
#define KERN_505_ROOTVNODE		0x22C1A70
#define KERN_672_ROOTVNODE		0x2300320
#define KERN_702_ROOTVNODE      0x022C5750


long unsigned int Temp() {

    int celsius;
    int hi = sceKernelGetCpuTemperature(&celsius);
    return celsius;

}

void SetTemp (long unsigned int celsius){
	
	int hii = sceKernelSetFanShit(&celsius);
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

		void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-KERN_405_XFAST_SYSCALL];
		uint8_t* kernel_ptr = (uint8_t*)kernel_base;
		void** got_prison0 =   (void**)&kernel_ptr[KERN_405_PRISON_0];
		void** got_rootvnode = (void**)&kernel_ptr[KERN_405_ROOTVNODE];

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

		void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-KERN_455_XFAST_SYSCALL];
		uint8_t* kernel_ptr = (uint8_t*)kernel_base;
		void** got_prison0 =   (void**)&kernel_ptr[KERN_455_PRISON_0];
		void** got_rootvnode = (void**)&kernel_ptr[KERN_455_ROOTVNODE];

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

		void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-KERN_474_XFAST_SYSCALL];
		uint8_t* kernel_ptr = (uint8_t*)kernel_base;
		void** got_prison0 =   (void**)&kernel_ptr[KERN_474_PRISON_0];
		void** got_rootvnode = (void**)&kernel_ptr[KERN_474_ROOTVNODE];

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

		void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-KERN_501_XFAST_SYSCALL];
		uint8_t* kernel_ptr = (uint8_t*)kernel_base;
		void** got_prison0 =   (void**)&kernel_ptr[KERN_501_PRISON_0];
		void** got_rootvnode = (void**)&kernel_ptr[KERN_501_ROOTVNODE];

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

		void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-KERN_505_XFAST_SYSCALL];
		uint8_t* kernel_ptr = (uint8_t*)kernel_base;
		void** got_prison0 =   (void**)&kernel_ptr[KERN_505_PRISON_0];
		void** got_rootvnode = (void**)&kernel_ptr[KERN_505_ROOTVNODE];

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

		void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-KERN_672_XFAST_SYSCALL];
		uint8_t* kernel_ptr = (uint8_t*)kernel_base;
		void** got_prison0 =   (void**)&kernel_ptr[KERN_672_PRISON_0];
		void** got_rootvnode = (void**)&kernel_ptr[KERN_672_ROOTVNODE];

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

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-KERN_702_XFAST_SYSCALL];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 = (void**)&kernel_ptr[KERN_702_PRISON_0];
	void** got_rootvnode = (void**)&kernel_ptr[KERN_702_ROOTVNODE];

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