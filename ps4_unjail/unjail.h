#pragma once

#include <_types.h>

struct auditinfo_addr {
	char useless[184];
};

struct ucred {
	uint32_t useless1;
	uint32_t cr_uid;     // effective user id
	uint32_t cr_ruid;    // real user id
	uint32_t useless2;
	uint32_t useless3;
	uint32_t cr_rgid;    // real group id
	uint32_t useless4;
	void *useless5;
	void *useless6;
	void *cr_prison;     // jail(2)
	void *useless7;
	uint32_t useless8;
	void *useless9[2];
	void *useless10;
	struct auditinfo_addr useless11;
	uint32_t *cr_groups; // groups
	uint32_t useless12;
};

struct filedesc {
	void *useless1[3];
	void *fd_rdir;
	void *fd_jdir;
};

struct proc {
	char useless[64];
	struct ucred *p_ucred;
	struct filedesc *p_fd;
};

struct thread {
	void *useless;
	struct proc *td_proc;
};
struct kpayload_args { uint64_t user_arg; };
struct kdump_args { uint64_t argArrayPtr; };

void  *unjail405(struct thread *td);
void  *unjail455(struct thread *td);
void  *unjail474(struct thread *td);
void  *unjail501(struct thread *td);
void  *unjail505(struct thread *td);
void  *unjail672(struct thread *td);
void  *unjail702(struct thread *td);
void  *unjail75X(struct thread *td);
long unsigned int Temp();
void SetTemp(long unsigned int celsius);
int DecryptPup(const char* path);