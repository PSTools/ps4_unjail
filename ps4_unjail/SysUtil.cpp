
#pragma once
#include <libsysmodule.h>
//#include <message_dialog.h>

#include "SysUtil.h"
#include <stdio.h>
#include "syscall.h"

#include "MsgDialog.h"


int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char* message);
int (*customsceSystemServiceLoadExec)(const char* path, char* const argv[]);
int32_t (*_sceMsgDialogInitialize)(void);
int32_t (*sceMsgDialogTerminate)(void);
int32_t (*sceMsgDialogOpen)(const OrbisMsgDialogParam *param);
OrbisCommonDialogStatus (*sceMsgDialogUpdateStatus)(void);
int32_t (*sceMsgDialogGetResult)(OrbisMsgDialogResult *result);

//void sceMsgDialogParamInitialize(OrbisMsgDialogParam *param)
//{
//	memset( param, 0x0, sizeof(OrbisMsgDialogParam) );
//
//	_sceCommonDialogBaseParamInit( &param->baseParam );
//	param->size = sizeof(OrbisMsgDialogParam);
//}



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

/*Show Messages Thanks to LM's Store Source Code*/
int loadmsg(char* format, ...)
{

	//return 0;
	int sceSysModuleMessageDialog = sceKernelLoadStartModule("/system/common/lib/libSceMsgDialog.sprx", 0, NULL, 0, 0, 0);
	int ret =sceKernelDlsym(sceSysModuleMessageDialog, "sceMsgDialogInitialize", (void **)&_sceMsgDialogInitialize);
	ret = sceKernelDlsym(sceSysModuleMessageDialog, "sceMsgDialogOpen", (void **)&sceMsgDialogOpen);

	_sceMsgDialogInitialize();

	char buff[1024];
	memset(buff, 0, sizeof(1024));

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

	return 0;
}

int msgok(char* format, ...)
{
	int ret = 0;

	int sceSysModuleMessageDialog = sceKernelLoadStartModule("/system/common/lib/libSceMsgDialog.sprx", 0, NULL, 0, 0, 0);
	ret =sceKernelDlsym(sceSysModuleMessageDialog, "sceMsgDialogInitialize", (void **)&_sceMsgDialogInitialize);
	ret = sceKernelDlsym(sceSysModuleMessageDialog, "sceMsgDialogOpen", (void **)&sceMsgDialogOpen);
	ret = sceKernelDlsym(sceSysModuleMessageDialog, "sceMsgDialogTerminate", (void **)&sceMsgDialogTerminate);
	ret = sceKernelDlsym(sceSysModuleMessageDialog,"sceMsgDialogUpdateStatus",(void **)&sceMsgDialogUpdateStatus);
	ret = sceKernelDlsym(sceSysModuleMessageDialog,"sceMsgDialogGetResult",(void **)&sceMsgDialogGetResult);
	//sceSysmoduleLoadModule(SCE_SYSMODULE_MESSAGE_DIALOG);

	sceMsgDialogTerminate();

	char buff[1024];
	char buffer[1000];
	memset(buff, 0, 1024);

	va_list args;
	va_start(args, format);
	vsprintf(buff, format, args);
	va_end(args);

	strcpy(buffer, buff);

	//logshit(buff);

	_sceMsgDialogInitialize();
	OrbisMsgDialogParam param;
	OrbisMsgDialogParamInitialize(&param);
	param.mode = ORBIS_MSG_DIALOG_MODE_USER_MSG;

	OrbisMsgDialogUserMessageParam userMsgParam;
	memset(&userMsgParam, 0, sizeof(userMsgParam));
	userMsgParam.msg = buffer;
	userMsgParam.buttonType = ORBIS_MSG_DIALOG_BUTTON_TYPE_OK;
	param.userMsgParam = &userMsgParam;

	if (sceMsgDialogOpen(&param) < 0)
		ret = -1;	


	OrbisCommonDialogStatus stat;

	while (1)
	{
		stat = sceMsgDialogUpdateStatus();
		if (stat == ORBIS_COMMON_DIALOG_STATUS_FINISHED)
		{
			OrbisMsgDialogResult result;
			memset(&result, 0, sizeof(result));

			if (0 > sceMsgDialogGetResult(&result))
				ret = -2;

			sceMsgDialogTerminate();
			break;
		}
	}

	return ret;
}


int hidemsg()
{
	int ret = 0;

	int sceSysModuleMessageDialog = sceKernelLoadStartModule("/system/common/lib/libSceMsgDialog.sprx", 0, NULL, 0, 0, 0);
	ret = sceKernelDlsym(sceSysModuleMessageDialog, "sceMsgDialogTerminate", (void **)&sceMsgDialogTerminate);
	//sceSysmoduleLoadModule(SCE_SYSMODULE_MESSAGE_DIALOG);

	sceMsgDialogTerminate();

	return ret;
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