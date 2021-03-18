#ifndef _USERSERVICE_H_
#define _USERSERVICE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*Error Defenitions*/
#define USER_SERVICE_ERROR_ALREADY_INITIALIZED -2137653245	/* 0x80960003 */
#define SCE_NP_INVALID_SERVICE_LABEL (0xFFFFFFFF)

typedef uint64_t SceKernelCpumask;
typedef int32_t SceUserServiceUserId;

typedef struct UserServiceInitializeParams {
	int32_t priority;
} UserServiceInitializeParams;


int32_t (*sceUserServiceGetInitialUser)(int *userId);
//int32_t (*sceUserServiceGetUserName)(int32_t, char *, const size_t);
int32_t (*sceUserServiceInitialize)(UserServiceInitializeParams *initParams);
int32_t (*sceUserServiceInitialize2)(int threadPriority,SceKernelCpumask cpuAffinityMask);
int32_t (*sceUserServiceInitializeForShellCore)(int32_t *initParams);
int32_t (*sceUserServiceGetUserName)(const SceUserServiceUserId userId, char *userName, const size_t size);
int32_t (*sceUserServiceGetHomeDirectory)(char* userfolder, size_t size,SceUserServiceUserId userId);

#endif