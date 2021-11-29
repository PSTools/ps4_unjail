
#pragma once
#include <libsysmodule.h>
//#include <message_dialog.h>

#include "SysUtil.h"
#include <stdio.h>
#include "syscall.h"
#include <string>

#include "MsgDialog.h"


int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char* message);
int (*customsceSystemServiceLoadExec)(const char* path, char* const argv[]);
int32_t (*_sceMsgDialogInitialize)(void);
int32_t (*sceMsgDialogTerminate)(void);
int32_t (*sceMsgDialogOpen)(const OrbisMsgDialogParam *param);
OrbisCommonDialogStatus (*sceMsgDialogUpdateStatus)(void);
int32_t (*sceMsgDialogGetResult)(OrbisMsgDialogResult *result);

//error dialog
int32_t (*sceErrorDialogInitialize)(void);
int32_t (*sceErrorDialogTerminate)(void);
int32_t (*sceErrorDialogOpen)(const OrbisErrorMsgDialogParam *param);
int32_t (*sceErrorDialogOpenWithReport)(const OrbisErrorMsgDialogParam *param,char* report);
int32_t (*sceErrorDialogOpenDetail)(const OrbisErrorMsgDetailParam *param,char* report,char* message,char* defaultMessage,char* title);
int32_t (*sceErrorDialogClose)(void);
OrbisCommonDialogStatus (*sceErrorDialogUpdateStatus)(void);
int32_t (*sceErrorDialogGetStatus)(void);
int32_t (*sceErrorDialogParamInitialize)(const OrbisErrorMsgDialogParam *param);
int32_t (*sceErrorDialogDetailParamInitialize)(const OrbisErrorMsgDetailParam *param);

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

int msgYesNo(char* format, ...)
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
	OrbisMsgDialogButtonsParam		buttonsParam;

	memset(&userMsgParam, 0, sizeof(userMsgParam));
	memset( &buttonsParam, 0x00, sizeof(buttonsParam) );
	userMsgParam.msg = buffer;
	userMsgParam.buttonType = ORBIS_MSG_DIALOG_BUTTON_TYPE_2BUTTONS;
	userMsgParam.buttonsParam	= &buttonsParam;
	buttonsParam.msg1			= "YES";
	buttonsParam.msg2			= "NO";
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
			if(result.buttonId == ORBIS_MSG_DIALOG_BUTTON_ID_BUTTON1)
			{
				ret =1;
			}
			else
			{
				ret = 0;
			}
			sceMsgDialogTerminate();
			break;
		}
	}

	return ret;
}


int errorMsg(char* format,...)
{
	int ret = 0;

	int sceSysModuleMessageDialog = sceKernelLoadStartModule("/system/common/lib/libSceErrorDialog.sprx", 0, NULL, 0, 0, 0);
	ret =sceKernelDlsym(sceSysModuleMessageDialog, "sceErrorDialogInitialize", (void **)&sceErrorDialogInitialize);
	ret = sceKernelDlsym(sceSysModuleMessageDialog, "sceErrorDialogTerminate", (void **)&sceErrorDialogTerminate);
	ret = sceKernelDlsym(sceSysModuleMessageDialog, "sceErrorDialogOpen", (void **)&sceErrorDialogOpen);
	ret = sceKernelDlsym(sceSysModuleMessageDialog,"sceErrorDialogOpenWithReport",(void **)&sceErrorDialogOpenWithReport);
	ret = sceKernelDlsym(sceSysModuleMessageDialog,"sceErrorDialogOpenDetail",(void **)&sceErrorDialogOpenDetail);
	ret = sceKernelDlsym(sceSysModuleMessageDialog,"sceErrorDialogClose",(void **)&sceErrorDialogClose);
	ret = sceKernelDlsym(sceSysModuleMessageDialog,"sceErrorDialogUpdateStatus",(void **)&sceErrorDialogUpdateStatus);
	ret = sceKernelDlsym(sceSysModuleMessageDialog,"sceErrorDialogGetStatus",(void **)&sceErrorDialogGetStatus);
	ret = sceKernelDlsym(sceSysModuleMessageDialog,"sceErrorDialogParamInitialize",(void **)&sceErrorDialogParamInitialize);
	ret = sceKernelDlsym(sceSysModuleMessageDialog,"sceErrorDialogDetailParamInitialize",(void **)&sceErrorDialogDetailParamInitialize);
	//sceSysmoduleLoadModule(SCE_SYSMODULE_MESSAGE_DIALOG);
	sceErrorDialogTerminate();


	char buff[1024];
	char buffer[1000];
	memset(buff, 0, 1024);

	va_list args;
	va_start(args, format);
	vsprintf(buff, format, args);
	va_end(args);

	strcpy(buffer, buff);


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


const char* s_menuXml = 
	"<group label='CommonDialog ShowCase'>"
	"	<int id='PadCtrlUserSelect' label='PadCtrlUserSelect' value='0' min='0' max='0' format=''/>"
	"	<int id='MessageDialog' label='MessageDialog' value='0' min='0' max='0' format=''/>"
	"	<int id='InvitationDialog' label='InvitationDialog' value='0' min='0' max='0'  format=''/>"
	"	<int id='SaveDataDialog' label='SaveDataDialog' value='0' min='0' max='0' format=''/>"
	"	<int id='WebBrowserDialog' label='WebBrowserDialog' value='0' min='0' max='0' format=''/>"
	"	<int id='NpFriendlistDialog' label='NpFriendlistDialog' value='0' min='0' max='0' format=''/>"
	"	<int id='NpProfileDialog' label='NpProfileDialog' value='0' min='0' max='0' format=''/>"
	"	<int id='GameCustomDataDialog' label='GameCustomDataDialog' value='0' min='0' max='0'  format=''/>"
	"	<int id='NpNamePrivacyDialog' label='MessageDialog(NpNamePrivacySetting)' value='0' min='0' max='0'  format=''/>"
	"	<int id='VrServiceDialog' label='VrServiceDialog' value='0' min='0' max='0'  format=''/>"
	"</group>";

int ShowCustomMessage()
{
	//Custom Message Dialog Here
	//std::string menu_xml = "<group label='PadCtrlUserSelect'>";
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