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

#define ORBIS_USER_SERVICE_ERROR_NO_EVENT			-2137653241	/* 0x80960007 */

typedef uint64_t SceKernelCpumask;
typedef int32_t SceUserServiceUserId;

typedef struct UserServiceInitializeParams {
	int32_t priority;
} UserServiceInitializeParams;


typedef struct OrbisUserServiceLoginUserIdList {
    SceUserServiceUserId userId[SCE_USER_SERVICE_MAX_LOGIN_USERS];
} OrbisUserServiceLoginUserIdList;


extern SceUserServiceUserId userId;


typedef enum OrbisUserServiceUserColor {
    ORBIS_USER_SERVICE_USER_COLOR_BLUE = 0,
    ORBIS_USER_SERVICE_USER_COLOR_RED,
    ORBIS_USER_SERVICE_USER_COLOR_GREEN,
    ORBIS_USER_SERVICE_USER_COLOR_PINK
} OrbisUserServiceUserColor;

typedef enum OrbisUserServiceEventType {
	ORBIS_USER_SERVICE_EVENT_TYPE_LOGIN = 0,
	ORBIS_USER_SERVICE_EVENT_TYPE_LOGOUT
} OrbisUserServiceEventType;

typedef struct OrbisUserServiceEvent {
	OrbisUserServiceEventType eventType;
	SceUserServiceUserId userId;
} OrbisUserServiceEvent;


void initsysUserService(void);
int InitlizieUserService();
int GetInitialUser();
int32_t GetUserName(const SceUserServiceUserId userId, char *userName, const size_t size);

const char* UserServiceGetUserId();

char* UserServiceGetUserName();

int32_t orbisUserServiceGetUserColor(const SceUserServiceUserId userId, OrbisUserServiceUserColor *color);

int32_t orbisUserServiceGetEvent(OrbisUserServiceEvent *event);

int32_t orbisUserServiceGetLoginUserIdList(OrbisUserServiceLoginUserIdList *userIdList);
#endif