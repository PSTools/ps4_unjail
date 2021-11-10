#include "SysUtil.h"
#include "UserService.h"
#include <_kernel.h>


int32_t (*sceUserServiceGetInitialUser)(int *userId);
//int32_t (*sceUserServiceGetUserName)(int32_t, char *, const size_t);
int32_t (*sceUserServiceInitialize)(UserServiceInitializeParams *initParams);
int32_t (*sceUserServiceInitialize2)(int threadPriority,SceKernelCpumask cpuAffinityMask);
int32_t (*sceUserServiceInitializeForShellCore)(int32_t *initParams);
int32_t (*sceUserServiceGetUserName)(const SceUserServiceUserId userId, char *userName, const size_t size);
int32_t (*sceUserServiceGetHomeDirectory)(char* userfolder, size_t size,SceUserServiceUserId userId);

SceUserServiceUserId userId = SCE_USER_SERVICE_USER_ID_INVALID;

//Init SCE User Service Funcitons
void initsysUserService(void)
{
	int sysUserService = sceKernelLoadStartModule("libSceUserService.sprx", 0, NULL, 0, NULL, NULL);
	sceKernelDlsym(sysUserService, "sceUserServiceInitialize", (void **)&sceUserServiceInitialize);
	sceKernelDlsym(sysUserService, "sceUserServiceInitialize2", (void **)&sceUserServiceInitialize2);
	sceKernelDlsym(sysUserService, "sceUserServiceInitializeForShellCore", (void **)&sceUserServiceInitializeForShellCore);
	sceKernelDlsym(sysUserService, "sceUserServiceGetInitialUser", (void **)&sceUserServiceGetInitialUser);
	sceKernelDlsym(sysUserService, "sceUserServiceGetUserName", (void **)&sceUserServiceGetUserName);
	sceKernelDlsym(sysUserService, "sceUserServiceGetHomeDirectory", (void **)&sceUserServiceGetHomeDirectory);
}

int InitlizieUserService()
{
	int ret=0;
	(void)ret;
	initSysUtil();
	//Initilize sce functions from lib
	initsysUserService();

	//E Initialize UserService.
	//J UserService の初期化。
	UserServiceInitializeParams params;
	//set it in memory
	memset(&params, 0, sizeof(params));
	params.priority = 256;
	//do this
	ret = sceUserServiceInitialize(NULL);
	if(ret < 0)
	{
		//__int32_t errordisplay ;
		//memcpy(&errordisplay,(void*)ret,sizeof(int32_t));

		if (ret == USER_SERVICE_ERROR_ALREADY_INITIALIZED)
		{
			//notify("USER SERVICE ERROR ALREADY INITIALIZED");
			ret =0;

		}
		else
		{
			notify("Coult not Initilize User Service");
			return ret;
		}

		//error has come up on itilize
		return ret;
	}	
	return ret;
}


int GetInitialUser()
{
	notify("Get Initial User");
	int ret =0;
	//if(userId == SCE_USER_SERVICE_USER_ID_INVALID)

	ret = sceUserServiceGetInitialUser(&userId);
	if(ret < 0)
	{
		notify("sceUserServiceGetInitialUser failed");
		return ret;
	}

	notify((char*)userId);
	return ret;

}

const char* UserServiceGetUserId()
{

	char rtnchar[32];
	int ret = -1;
	initSysUtil();
	char buffer[520];
	memset(buffer,0,sizeof(buffer));
	if(DEBUGENABlED == 1)
	{
		notify("Getting UserID");
	}

	ret =InitlizieUserService();
	if(ret < 0)
	{
		sprintf(buffer, "UserServiceError\n\n0x%08x", ret);
		notify(buffer);
		//notify("InitlizieUserService error");
		return buffer;
	}
	if(userId == SCE_USER_SERVICE_USER_ID_INVALID)
	{
		ret = GetInitialUser();
		if(ret < 0)
		{
			notify("sceUserServiceGetInitialUser failed");
			return buffer;
		}
	}
	//notify("Got Userid");
	sprintf(buffer, "%d", userId);
	char * a = new char[1024];
	strcpy(a, buffer);
	return (a);

}

char* UserServiceGetUserName()
{
	char* rtnchar;
	//memset(rtnchar,0,sizeof(rtnchar));
	int ret = -1;

	//initilize util
	initSysUtil();
	char username[32];
	//memset(username, 0, 32);
	//memset(username, 0, 32);
	char buffer[520];
	if(DEBUGENABlED == 1)
	{
		notify("Initlizing UserService");
	}
	ret =InitlizieUserService();
	if(ret < 0)
	{
		sprintf(buffer, "UserServiceError\n%d", ret);
		notify(buffer);
		//notify("InitlizieUserService error");
		return buffer;
	}
	if(DEBUGENABlED == 1)
	{
		notify("Getting username");
	}
	if(userId == SCE_USER_SERVICE_USER_ID_INVALID)
	{
		ret = sceUserServiceGetInitialUser(&userId);
		if(ret < 0)
		{
			notify("sceUserServiceGetInitialUser failed");
			return buffer;
		}
	}

	notify("Getting Username");

	(void)memset(username, 0, sizeof(username));

	if (sceUserServiceGetUserName(userId, username, sizeof(username) - 1) < 0)
	{
		notify("[DEBUG] [ERROR] Failed to get username!\n");
		return buffer;
	}
	//memcpy(rtnchar,username,sizeof(username));
	//strcpy( rtnchar,username);
	if(DEBUGENABlED == 1)
	{
		notify(username);
	}
	/*sprintf(buffer,"Username is %s",username);
	notify(buffer);
	rtnchar = username;
	*/
	char * a = new char[1024];
	strcpy(a, username);
	return (a);
}

int32_t GetUserName(const SceUserServiceUserId userId, char *userName, const size_t size)
{
	return sceUserServiceGetUserName(userId,userName,size);
}