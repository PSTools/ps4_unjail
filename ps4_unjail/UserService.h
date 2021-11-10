#ifndef _USERSERVICE_H_
#define _USERSERVICE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <user_service\user_service_defs.h>

/*Error Defenitions*/
#define USER_SERVICE_ERROR_ALREADY_INITIALIZED -2137653245	/* 0x80960003 */
#define SCE_NP_INVALID_SERVICE_LABEL (0xFFFFFFFF)

typedef uint64_t SceKernelCpumask;
typedef int32_t SceUserServiceUserId;

typedef struct UserServiceInitializeParams {
	int32_t priority;
} UserServiceInitializeParams;


extern SceUserServiceUserId userId;


void initsysUserService(void);
int InitlizieUserService();
int GetInitialUser();
int32_t GetUserName(const SceUserServiceUserId userId, char *userName, const size_t size);

const char* UserServiceGetUserId();

char* UserServiceGetUserName();

#endif