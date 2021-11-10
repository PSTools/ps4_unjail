
#pragma once


#include "SysUtil.h"
#include <stdio.h>


int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char* message);
int (*customsceSystemServiceLoadExec)(const char* path, char* const argv[]);

int DEBUGENABlED = 0;

bool IS_ERROR(uint32_t a1)
{
	return a1 & 0x80000000;
}

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


int SysLoadExec(const char *path, char *const *argv)
{
	if (path == nullptr) return false;
	if (path[0] == '\0') return false;
	printf("hello from xdpx");
	int ret = 1;
	ret = customsceSystemServiceLoadExec(path, NULL);
	if (ret == 0) return true;
	return false;
}