
#pragma once


#include "SysUtil.h"
#include <stdio.h>
#include "syscall.h"

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


int loadmsg(char* format, ...)
{

	return 0;
	/*sceSysmoduleLoadModule(ORBIS_SYSMODULE_MESSAGE_DIALOG);

	sceMsgDialogInitialize();

	char* buff[1024];
	memset(buff, 0, 1024);

	va_list args;
	va_start(args, format);
	vsprintf(buff, format, args);
	va_end(args);

	OrbisMsgDialogButtonsParam buttonsParam;
	OrbisMsgDialogUserMessageParam messageParam;
	OrbisMsgDialogParam dialogParam;

	OrbisMsgDialogParamInitialize(&dialogParam);

	memset(&buttonsParam, 0x00, sizeof(buttonsParam));
	memset(&messageParam, 0x00, sizeof(messageParam));

	dialogParam.userMsgParam = &messageParam;
	dialogParam.mode = ORBIS_MSG_DIALOG_MODE_USER_MSG;

	messageParam.buttonType = ORBIS_MSG_DIALOG_BUTTON_TYPE_WAIT;

	messageParam.msg = buff;

	sceMsgDialogOpen(&dialogParam);

	return 0;*/
}


int sys_dynlib_dlsym(int loadedModuleID, const char *name, void *destination) {
	return syscall(591, loadedModuleID, name, destination);
}

int sys_dynlib_load_prx(const char *name, int *idDestination) {
	return syscall(594, name, 0, idDestination, 0);
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