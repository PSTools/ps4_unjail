#define PRX_EXPORT extern "C" __declspec (dllexport)

#include "pluginunjail.h"

extern "C" {
#include "unjail.h"
#include "ftps4.h"
#include "mount.h"
}

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <kernel.h>
#include <libdbg.h>
#include <sceerror.h>
#include <scetypes.h>
#include <net.h>
#include <libhttp.h>
#include <sys/socket.h>
#include <scetypes.h>
#include <libnet/socket.h>
#include <sys/errno.h>
#include <libnet/socket.h>
#include <libnetctl.h>
#include <_types.h>
#include <unistd.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <net.h>
#include <libhttp.h>
#include <sys/socket.h>
#include <scetypes.h>
#include <libnet/socket.h>
#include <sys/errno.h>
#include <libnet/socket.h>
#include <libnetctl.h>
#include <libsysmodule.h>
#include <usb_storage.h>
#include <string>
#include <sys/time.h>
#include <kernel_ex.h>
#include "types.h"


#include <fcntl.h>



#include "UserService.h"
#include "Saves.h"
#include "SysUtil.h"
#include "SystemService.h"
#include "Kernel.h"
#include "SoundUtil.h"
#include "SaveDataDialog.h"

extern int run;


#include <assert.h>
#define Assert assert

#include "PlayerInterface/UnityPrxPlugin.h"
#include "PlayerInterface/UnityEventQueue.h"
#include <system_service.h>

extern int get_ip_address(char *ip_address);

// ============================================================
// This event is triggered for SCE_SYSTEM_SERVICE_EVENT_ENTITLEMENT_UPDATE.
struct OnEntitlementUpdate { SceSystemServiceEvent params; };
REGISTER_EVENT_ID(0xDE76F015C0DE4BE8ULL, 0x9046B1153C877E39ULL, OnEntitlementUpdate)

	void OnEntitlementUpdateHandler(const OnEntitlementUpdate &eventData)
{
	Assert(eventData.params.eventType == SCE_SYSTEM_SERVICE_EVENT_ENTITLEMENT_UPDATE);

	printf("SCE_SYSTEM_SERVICE_EVENT_ENTITLEMENT_UPDATE\n");
}

UnityEventQueue::StaticFunctionEventHandler<OnEntitlementUpdate>	g_OnEntitlementUpdateHandler(OnEntitlementUpdateHandler);


// ============================================================
// This event is triggered for SCE_SYSTEM_SERVICE_EVENT_APP_LAUNCH_LINK.
struct OnLaunchLink { SceSystemServiceEvent params; };
REGISTER_EVENT_ID(0x477AFB5C1CA045D6ULL, 0x95E9C61B8365A66AULL, OnLaunchLink)

	void OnLaunchLinkHandler(const OnLaunchLink &eventData)
{
	Assert(eventData.params.eventType == SCE_SYSTEM_SERVICE_EVENT_APP_LAUNCH_LINK);

	uint32_t size = eventData.params.data.appLaunchLink.size;
	const uint8_t *  data = eventData.params.data.appLaunchLink.arg;

	printf("SCE_SYSTEM_SERVICE_EVENT_APP_LAUNCH_LINK\n");
}

UnityEventQueue::StaticFunctionEventHandler<OnLaunchLink>	g_OnLaunchLinkHandler(OnLaunchLinkHandler);




extern "C" int module_start(size_t sz, const void* arg)
{
	if (!ProcessPrxPluginArgs(sz, arg, "libPS4Unjail"))
	{
		// Failed.
		return -1;
	}

	// An example of handling system service events
	UnityEventQueue::IEventQueue* eventQueue = GetRuntimeInterface<UnityEventQueue::IEventQueue>(PRX_PLUGIN_IFACE_ID_GLOBAL_EVENT_QUEUE);
	eventQueue->AddHandler(&g_OnEntitlementUpdateHandler);
	eventQueue->AddHandler(&g_OnLaunchLinkHandler);

	return 0;
}

//int (*KLOG)(const char *_Restrict, ...);
//
//void loadAndImportLibC()
//{
//	int kernel_lib = sceKernelLoadStartModule("/system/common/lib/libc.sprx", 0, NULL, 0, NULL, NULL);
//	sceKernelDlsym(kernel_lib, "printf", (void **)&KLOG);//this just returns some fed up info
//}



//Call this funciton if you want to show debug messages everywhere
PRX_EXPORT void SetDebuggerTrue()
{
	DEBUGENABlED =1;
	//loadAndImportLibC();
	//if(KLOG != nullptr)
	//{
	//	//initSysUtil();
	//	//notify("Klog found");
	//	//SendMessageToPS4("");
	//	//KLOG("test");
	//}
}

#pragma region << Original Functions >>

PRX_EXPORT int GetInteger()
{
	return 6;
}

PRX_EXPORT const char* GetString()
{
	return "Hello";
}

PRX_EXPORT int AddTwoIntegers(int i1, int i2)
{
	return i1 + i2;
}

PRX_EXPORT float AddTwoFloats(float f1, float f2)
{
	return f1 + f2;
}

PRX_EXPORT int FreeFTP()
{
	char ip_address[SCE_NET_CTL_IPV4_ADDR_STR_LEN];
	get_ip_address(ip_address);
	ftps4_init(ip_address, 21);
	return 0;
}

PRX_EXPORT struct tm * netGetTimeWide(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	time_t timedate = tv.tv_sec;
	struct tm * timestring = localtime(&timedate);
	return timestring;
}

PRX_EXPORT int FreeUnjail(int FWVersion)
{
	struct thread td;
	if(FWVersion == 405){
		return Sys::kexec((void *)&unjail405, &td);
	}else if(FWVersion == 455){
		return Sys::kexec((void *)&unjail455, &td);
	}else if(FWVersion == 474){
		return Sys::kexec((void *)&unjail474, &td);
	}else if(FWVersion == 501){
		return Sys::kexec((void *)&unjail501, &td);
	}else if(FWVersion == 505){
		return Sys::kexec((void *)&unjail505, &td);
	}else if(FWVersion == 672){
		return Sys::kexec((void *)&unjail672, &td);
	}else if (FWVersion == 702){
		return Sys::kexec((void *)&unjail702, &td);
	}else if (FWVersion == 751){
		return Sys::kexec((void *)&unjail751, &td);
	}else if (FWVersion == 755){
		return Sys::kexec((void *)&unjail755, &td);
	}

	return 0;
}



#define SCE_SYSMODULE_USB_STORAGE 0xD5

PRX_EXPORT int FreeMountUsb(int offset)
{
	int ret = sceSysmoduleLoadModule(SCE_SYSMODULE_USB_STORAGE);
	if (ret != SCE_OK) {
		printf("Error: sceSysmoduleLoadModule(SCE_SYSMODULE_USB_STORAGE), ret 0x%08x\n", ret);
		return ret;
	}
	typedef int32_t(*__sceUsbStorageRequestMap2)(uint32_t id, const char * directory, const char * mountpt);
	ret = sceUsbStorageInit(0);
	int32_t device_count = 0;
	uint32_t device_ids[10] = { 0 };
	sceUsbStorageGetDeviceList(device_ids, &device_count);
	__sceUsbStorageRequestMap2 _sceUsbStorageRequestMap2 = (__sceUsbStorageRequestMap2)(((uint64_t)&(*sceUsbStorageRequestMap)) - (offset)); // offset on 5.05

	for (int i = 0; i < device_count; ++i)
	{
		std::string mount_pt = std::string("/usb") + std::to_string(i);
		ret = _sceUsbStorageRequestMap2(device_ids[i], "/", mount_pt.c_str());
	}
	return 0;
}



PRX_EXPORT uint32_t FW()
{
	SceKernelFwInfo fwinfo;
	char* info;

	fwinfo.size = sizeof(fwinfo);
	int kernVerRet = sceKernelGetSystemSwVersion(&fwinfo);
	if (kernVerRet < 0)
	{
		printf("[ERROR] 0x%08X Failed to get firmware info\n");
		return 0;
	}
	else
	{
		printf("[INFO] sceKernelGetSystemSwVersion: 0x%08X\n", fwinfo.version);
		return fwinfo.version;
	}

}


//long unsigned int Temp();

PRX_EXPORT long unsigned int  Temperature() {
	return Temp();
}

PRX_EXPORT void  SetTemperature(long unsigned int celsius) {
	SetTemp(celsius);
}

PRX_EXPORT int PupDecrypt(const char* path){
	int res = DecryptPup(path);
	return res;
}


PRX_EXPORT int FreeMountUsbMuilti(){
	//char version[50];
	uint32_t fw = FW();
	//firmware_version_libc(version);


	if (fw >= 0x672)
	{
		FreeMountUsb(0x90);
	}
	else
	{
		FreeMountUsb(0xA0);
	}
	return 0;
}

PRX_EXPORT int FreeMount()
{
	mount_large_fs("/dev/da0x0.crypt", "/preinst", "exfatfs", "511", MNT_UPDATE);
	mount_large_fs("/dev/da0x1.crypt", "/preinst2", "exfatfs", "511", MNT_UPDATE);
	mount_large_fs("/dev/da0x4.crypt", "/system", "exfatfs", "511", MNT_UPDATE);
	mount_large_fs("/dev/da0x5.crypt", "/system_ex", "exfatfs", "511", MNT_UPDATE);
	mount_large_fs("/dev/sbram0", "/system_sam", "exfatfs", "511", MNT_UPDATE);
	mount_large_fs("/dev/sflah0.crypt", "/flash0", "exfatfs", "511", MNT_UPDATE);
	mount_large_fs("/dev/sflah1.crypt", "/flash1", "exfatfs", "511", MNT_UPDATE);
	return 0;
}



PRX_EXPORT int GetPid(void)
{
	return Sys::getpid();

}

PRX_EXPORT int GetUid(void)
{
	return Sys::getuid();

}


struct ReturnedStructure
{
	int number;
	const char* text;
};

PRX_EXPORT bool ReturnAStructure(ReturnedStructure* data)
{
	static char sText[] = "Hello";
	data->number = 23;
	data->text = sText;

	return true;
}

# pragma endregion << Original Functions >>


#pragma region << Type's Pointers and Mounts >>

enum{ ModulesSize = 1024 };

#pragma region  <<Notification Class >>

/*Has been moved*/

//int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char* message);
//int (*customsceSystemServiceLoadExec)(const char* path, char* const argv[]);
//
//void initSysUtil(void)
//{
//	int ret = 0;
//
//	int sysUtilHandle = sceKernelLoadStartModule("/system/common/lib/libSceSysUtil.sprx", 0, NULL, 0, 0, 0);
//	if(sysUtilHandle > 0)
//	{
//		ret =sceKernelDlsym(sysUtilHandle, "sceSysUtilSendSystemNotificationWithText", (void **)&sceSysUtilSendSystemNotificationWithText);
//		if(ret < 0)
//		{
//			//we couldn't load libscesysutil
//		}
//		sceKernelDlsym(sysUtilHandle, "sceSystemServiceLoadExec",(void **)&customsceSystemServiceLoadExec);
//	}
//	else
//	{
//
//	}
//}
//
//void notify(char *message)
//{
//	//int lenghtofmessage = sizeof(message);
//	char buffer[5120];
//	sprintf(buffer, "%s", message);//dont want all those enters
//	//sceSysUtilSendSystemNotificationWithText(0x81, buffer);
//	sceSysUtilSendSystemNotificationWithText(222, buffer);
//}

#pragma endregion <<Notification Class >>

//#define STORE_LOG "/user/app/NPXS39041/logs/log.txt"

//void klog(const char *format, ...)  
//{
//	if(strlen(format) < 300)
//	{
//		char buff[300];
//		memset(&buff[0], 0, sizeof(buff));
//
//		va_list args;
//		va_start(args, format);
//		vsprintf(&buff[0], format, args);
//		va_end(args);
//
//		printf("%s", buff);
//
//		/*int fd = sceKernelOpen(STORE_LOG, O_WRONLY | O_CREAT | O_APPEND, 0777);
//		if (fd >= 0)
//		{
//		if(Lastlogcheck == 10) {
//		CheckLogSize(); Lastlogcheck = 0; }
//		else
//		Lastlogcheck++;
//
//		sceKernelWrite(fd, buff, strlen(buff));
//		sceKernelClose(fd);
//		}*/
//
//
//	}
//	else
//		printf("DEBUG: input is too large!\n");
//
//}



#pragma region << Pad Functions >>


typedef struct OrbisPadColor{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} OrbisPadColor;

typedef struct vec_float3
{
	float x;
	float y;
	float z;
} vec_float3;

typedef struct vec_float4
{
	float x;
	float y;
	float z;
	float w;
} vec_float4;

// The ScePadData Structure contains data polled from the DS4 controller. This includes button states, analogue
// positional data, and touchpad related data.
typedef struct OrbisPadData {
	unsigned int buttons;	
	uint8_t lx;
	uint8_t ly;
	uint8_t rx;
	uint8_t ry;
	uint8_t l2;
	uint8_t r2;
	uint16_t padding;
	vec_float4 quat;
	vec_float3 vel;
	vec_float3 acell;
	uint8_t touch[24];
	uint8_t connected;
	uint64_t timestamp;
	uint8_t ext[16];
	uint8_t count;
	uint8_t unknown[15];
} OrbisPadData;

typedef struct ScePadStickInformation {
	uint8_t deadZoneLeft;
	uint8_t deadZoneRight;
} ScePadStickInformation;

typedef enum {
	SCE_PAD_DEVICE_CLASS_INVALID       	= -1,
	SCE_PAD_DEVICE_CLASS_STANDARD      	= 0,
	SCE_PAD_DEVICE_CLASS_GUITAR        	= 1,
	SCE_PAD_DEVICE_CLASS_DRUM          	= 2,
	SCE_PAD_DEVICE_CLASS_DJ_TURNTABLE  	= 3,
	SCE_PAD_DEVICE_CLASS_DANCEMAT      	= 4,
	SCE_PAD_DEVICE_CLASS_NAVIGATION    	= 5,
	SCE_PAD_DEVICE_CLASS_STEERING_WHEEL	= 6,
	SCE_PAD_DEVICE_CLASS_STICK         	= 7,
	SCE_PAD_DEVICE_CLASS_FLIGHT_STICK  	= 8,
	SCE_PAD_DEVICE_CLASS_GUN           	= 9,
} ScePadDeviceClass;
typedef struct ScePadTouchPadInformation {
	float pixelDensity;
	struct {
		uint16_t x;
		uint16_t y;
	} resolution;
} ScePadTouchPadInformation;
typedef struct ScePadControllerInformation {
	ScePadTouchPadInformation touchPadInfo;
	ScePadStickInformation stickInfo;
	uint8_t connectionType;
	uint8_t connectedCount;
	bool connected;
	ScePadDeviceClass deviceClass;
	uint8_t reserve[8];
} ScePadControllerInformation;
typedef struct ScePadOpenParam {
	uint8_t reserve[8];
} ScePadOpenParam;


int (*scePadGetHandle)(SceUserServiceUserId userID, uint32_t controller_type, uint32_t controller_index);
int (*scePadInit)(void);
int (*scePadSetLightBar)(int handle, OrbisPadColor *inputColor);
int(*scePadSetLightBarBlinking)(int a1, char *a2);
int (*scePadClose)(int handle);
int (*scePadRead)(int handle, OrbisPadData *data, int count);
int (*scePadReadState)(int handle, void *data);
int (*scePadResetLightBar)(int handle);
int (*scePadOpen)(SceUserServiceUserId userId,int32_t type,int32_t index,const ScePadOpenParam* pParam);
int (*scePadGetControllerInformation)(int32_t handle,ScePadControllerInformation *pInfo);
int (*scePadIsSupportedAudioFunction)(int32_t handle);

void initScePad(void)
{
	int scePadHandle = sceKernelLoadStartModule("/system/common/lib/libScePad.sprx", 0, NULL, 0, 0, 0);
	sceKernelDlsym(scePadHandle,"scePadInit",(void **)&scePadInit);
	sceKernelDlsym(scePadHandle, "scePadSetLightBar", (void **)&scePadSetLightBar);
	sceKernelDlsym(scePadHandle, "scePadSetLightBarBlinking",(void **)&scePadSetLightBarBlinking);
	sceKernelDlsym(scePadHandle, "scePadOpen",(void **)&scePadOpen);
	sceKernelDlsym(scePadHandle, "scePadClose",(void **)&scePadClose);
	sceKernelDlsym(scePadHandle, "scePadRead",(void **)&scePadRead);
	sceKernelDlsym(scePadHandle, "scePadReadState",(void **)&scePadReadState);
	sceKernelDlsym(scePadHandle, "scePadResetLightBar",(void **)&scePadResetLightBar);
	sceKernelDlsym(scePadHandle, "scePadGetHandle",(void**)&scePadGetHandle);
	sceKernelDlsym(scePadHandle, "scePadGetControllerInformation",(void**)&scePadGetControllerInformation);
	sceKernelDlsym(scePadHandle, "scePadIsSupportedAudioFunction",(void**)&scePadIsSupportedAudioFunction);
}

#pragma endregion << Pad Functions >>

#pragma region << Screenshot Util >>

typedef struct SceScreenShotParam {
	uint32_t thisSize;
	const char *photoTitle;
	const char *gameTitle;
	const char *gameComment;
	void *reserved;
} SceScreenShotParam;
int32_t (*sceScreenShotSetParam)(const SceScreenShotParam param);

void initSceScreenshot(void)
{
	int sceScreenshotHandle = sceKernelLoadStartModule("/system/common/lib/libSceScreenShot.sprx", 0, NULL, 0, 0, 0);
	sceKernelDlsym(sceScreenshotHandle,"sceScreenShotSetParam",(void **)&sceScreenShotSetParam);  
}


#pragma endregion << Screenshot Util >>

#pragma region << Trophy Class >>

#define SCE_NP_TROPHY_NAME_MAX_SIZE 128
#define SCE_NP_TROPHY_DESCR_MAX_SIZE 1024
#define SCE_NP_TROPHY_INVALID_CONTEXT (-1)
#define SCE_NP_TROPHY_ERROR_INVALID_NP_SERVICE_LABEL -2141907423 /*0x80551621*/
#define SCE_NP_TROPHY_ERROR_INVALID_ARGUMENT	-2141907452
#define SCE_NP_TROPHY_ERROR_INVALID_HANDLE	-2141907448
#define SCE_NP_TROPHY_ERROR_INVALID_NP_SERVICE_LABEL	-2141907423
#define SCE_NP_TROPHY_ERROR_INVALID_NP_TITLE_ID	-2141907424
#define SCE_NP_TROPHY_ERROR_INVALID_NPCOMMID	-2141907324
#define SCE_NP_TROPHY_ERROR_INVALID_NPCOMMSIGN	-2141907323

typedef struct SceNpTrophyGameDetails {
	size_t size;
	uint32_t numGroups;
	uint32_t numTrophies;
	uint32_t numPlatinum;
	uint32_t numGold;
	uint32_t numSilver;
	uint32_t numBronze;
	char title[128];
	char description[1024];
} SceNpTrophyGameDetails;

typedef struct SceNpTrophyGameData {
	size_t size;
	uint32_t unlockedTrophies;
	uint32_t unlockedPlatinum;
	uint32_t unlockedGold;
	uint32_t unlockedSilver;
	uint32_t unlockedBronze;
	uint32_t progressPercentage;
} SceNpTrophyGameData;

typedef int32_t SceNpTrophyHandle;
typedef int32_t SceNpTrophyContext;
typedef int32_t SceNpTrophyId;
typedef int32_t SceUserServiceUserId;
typedef uint32_t SceNpServiceLabel;


int (*NpTrophyUnlockTrophy)(SceNpTrophyContext context, SceNpTrophyHandle handle, SceNpTrophyId trophyId, SceNpTrophyId *platinumId);
int (*NpTrophyCreateHandle)(SceNpTrophyHandle *handle);
int (*sceNpTrophySystemCreateHandle)(SceNpTrophyHandle *handle);
int (*sceNpTrophyCreateContext)(SceNpTrophyContext *context,SceUserServiceUserId userId,SceNpServiceLabel serviceLabel,uint64_t options);
int (*sceNpTrophySystemCreateContext)(SceNpTrophyContext *context,SceUserServiceUserId userId,SceNpTrophyHandle *handle,uint64_t options);
int(*sceNpTrophyGetGameInfo)(SceNpTrophyContext context,SceNpTrophyHandle handle, SceNpTrophyGameDetails *details,SceNpTrophyGameData *GameData);
int (*sceNpTrophyRegisterContext)(SceNpTrophyContext context,SceNpTrophyHandle handle,uint64_t options);


//__int64 __fastcall sceNpTrophyCreateContext(unsigned int *a1, unsigned int a2, unsigned int a3, __int64 a4)

//__int64 __fastcall sceNpTrophyUnlockTrophy(unsigned int a1, unsigned int a2, unsigned int a3, _DWORD *a4)
SceNpTrophyContext context = 1;
SceNpTrophyHandle handle = 1;

//Init SCE Trophy Funcitons
void initsysNpTrophy(void)
{
	int sysNpTrophy = sceKernelLoadStartModule("/system/common/lib/libSceNpTrophy.sprx", 0, NULL, 0, 0, 0);
	sceKernelDlsym(sysNpTrophy, "sceNpTrophyUnlockTrophy", (void **)&NpTrophyUnlockTrophy);
	sceKernelDlsym(sysNpTrophy, "sceNpTrophyCreateHandle", (void **)&NpTrophyCreateHandle);
	sceKernelDlsym(sysNpTrophy, "sceNpTrophyCreateContext", (void **)&sceNpTrophyCreateContext);
	sceKernelDlsym(sysNpTrophy,"sceNpTrophySystemCreateHandle",(void **)&sceNpTrophySystemCreateHandle);
	sceKernelDlsym(sysNpTrophy, "sceNpTrophySystemCreateContext",(void **)&sceNpTrophySystemCreateContext);
	sceKernelDlsym(sysNpTrophy,"sceNpTrophyGetGameInfo",(void **)&sceNpTrophyGetGameInfo);
	sceKernelDlsym(sysNpTrophy,"sceNpTrophyRegisterContext",(void **) &sceNpTrophyRegisterContext);
}


#pragma endregion << Trophy Class >>

#pragma region << NP Manager >>
#define SCE_NP_ONLINEID_MIN_LENGTH        3
#define SCE_NP_ONLINEID_MAX_LENGTH        16
typedef struct SceNpOnlineId {
	char data[SCE_NP_ONLINEID_MAX_LENGTH];
	char term;
	char dummy[3];
} SceNpOnlineId;
typedef struct SceNpId {
	SceNpOnlineId handle;
	uint8_t opt[8];
	uint8_t reserved[8];
} SceNpId;
int32_t (*sceNpIntGetNpTitleId)(SceUserServiceUserId* a1, SceNpId* _YMM0);

/* Np title ID */
#define SCE_NP_TITLE_ID_LEN				(12)
typedef struct SceNpTitleId {
	char id[SCE_NP_TITLE_ID_LEN + 1];
	uint8_t padding[3];
} SceNpTitleId;

/* NP title secret */
#define SCE_NP_TITLE_SECRET_SIZE		(128)
typedef struct SceNpTitleSecret {
	uint8_t data[SCE_NP_TITLE_SECRET_SIZE];
} SceNpTitleSecret;


int	(*sceNpSetNpTitleIdVsh)(const SceNpTitleId *titleId,const SceNpTitleSecret *titleSecret);
int (*sceNpSetNpTitleId)(const SceNpTitleId *titleId,const SceNpTitleSecret *titleSecret);
void initsysNpManager(void)
{
	int sysNpManager = sceKernelLoadStartModule("/system/common/lib/libSceNpManager.sprx", 0, NULL, 0, 0, 0);
	sceKernelDlsym(sysNpManager, "sceNpSetNpTitleId", (void **)&sceNpSetNpTitleId);
	sceKernelDlsym(sysNpManager, "sceNpIntGetNpTitleId", (void **)&sceNpIntGetNpTitleId);
	sceKernelDlsym(sysNpManager, "sceNpSetNpTitleIdVsh", (void **)&sceNpSetNpTitleIdVsh);
}

#pragma endregion << NP Manager >>

#pragma endregion << Type's Pointers and Mounts >>

PRX_EXPORT const char* DevelopedBy()
{
	static char strar[] = "The Darkprogramer";
	char buffer[1024];
	sprintf(buffer, "%s", strar);

	return buffer;
}

#pragma region << Send notification on the ps4 >>

PRX_EXPORT int SendMessageToPS4(char* Message)
{	
	int result;
	//simple test case here
	//all i want to do is run a small sample and see if we can get access to sysctl
	initSysUtil();
	notify(Message);
	return SCE_OK;
}

PRX_EXPORT int HideDialog()
{
	return hidemsg();
}

PRX_EXPORT int ShowLoadingDialog(char* Message)
{
	return loadmsg(Message);
}

PRX_EXPORT int ShowMessageDialog(char* Message)
{
	return msgok(Message);
}

#pragma endregion << Send notification on the ps4 >>

#pragma region << Save Data >>

void PatchShellCore()
{
	//we need to patch a few extra values in shellcore 
	//these values are from ChapChendo's PS4 Save Mounter all thanks go to him

	//shellcore_savedata_patch();

	//I have added this to mira itself

}




PRX_EXPORT int MountSaveData(char* TITLEID,char* fingerprint)
{
	int ret = 0;
	initSysUtil();

	//SetDebuggerTrue();

	//this is no longer required as it seems to mount perfectly fine


	char buffer[1024];
	//notify("");//not sure why i had a blank notification here anyway
	if(DEBUGENABlED ==1)
	{
		sprintf(buffer,"Info %s %s",TITLEID,fingerprint);
		notify(buffer);
	}

	if ((fingerprint != NULL) && (fingerprint[0] == '\0')) 
	{
		printf("Fingerprint is empty assigning defualt\n");
		fingerprint = "00000000000000000000000000000000000000000";
		//if you dont want to do the keystone verification process use the below
		//294a5ed06db170618f2eed8c424b9d828879c080cc66fbc4864f69e974deb856 ////tis will only work for debug games
	}

	/*This structure sets the fingerprint of the passcode to use when mounting a save data directory.

	A fingerprint is a key for protecting save data from access by other applications. When a passcode (character string comprised of "a" to "z", "A" to "Z", and "0" to "9") is specified during application package creation, the fingerprint can be obtained as a hash value of the passcode (for details, refer to the "Package Generator User's Guide" document. When another application mounts this application's save data, this fingerprint must be specified and then read only mode must be specified.

	On the other hand, there is no need for a fingerprint to be specified when an application mounts its own save data.

	*/

	ret =SaveDataTest(TITLEID,fingerprint);//not sure if this is the sealed key or not 
	if(ret < 0)
	{
		if(ret = SCE_SAVE_DATA_ERROR_MOUNT_FULL)
		{
			notify("Save Mount Full");
			return ret;
		}
		else if(ret == SCE_SAVE_DATA_ERROR_PARAMETER)
		{
			notify("Save Mounted");
		}
	}
	return ret;
}

PRX_EXPORT int MountSaveData_Path(char* TITLEID,char* SaveToMount,char* fingerprint)
{
	int ret = 0;
	initSysUtil();

	//SetDebuggerTrue();

	//this is no longer required as it seems to mount perfectly fine


	char buffer[1024];
	//notify("");//not sure why i had a blank notification here anyway
	if(DEBUGENABlED ==1)
	{
		sprintf(buffer,"Info %s %s",TITLEID,fingerprint);
		notify(buffer);
	}

	if ((fingerprint != NULL) && (fingerprint[0] == '\0')) 
	{
		printf("Fingerprint is empty assigning defualt\n");
		fingerprint = "00000000000000000000000000000000000000000";
		//if you dont want to do the keystone verification process use the below
		//294a5ed06db170618f2eed8c424b9d828879c080cc66fbc4864f69e974deb856 ////tis will only work for debug games
	}

	/*This structure sets the fingerprint of the passcode to use when mounting a save data directory.

	A fingerprint is a key for protecting save data from access by other applications. When a passcode (character string comprised of "a" to "z", "A" to "Z", and "0" to "9") is specified during application package creation, the fingerprint can be obtained as a hash value of the passcode (for details, refer to the "Package Generator User's Guide" document. When another application mounts this application's save data, this fingerprint must be specified and then read only mode must be specified.

	On the other hand, there is no need for a fingerprint to be specified when an application mounts its own save data.

	*/

	ret =SaveDataTest(TITLEID,SaveToMount,fingerprint);//not sure if this is the sealed key or not 
	if(ret < 0)
	{
		if(ret = SCE_SAVE_DATA_ERROR_MOUNT_FULL)
		{
			notify("Save Mount Full");
			return ret;
		}
		else if(ret == SCE_SAVE_DATA_ERROR_PARAMETER)
		{
			notify("Save Mounted");
		}
	}
	return ret;
}

PRX_EXPORT int MountSaveData2(char* TITLEID,char* SaveToMount)
{
	//SaveDataTestMount2

	int ret = 0;
	initSysUtil();

	//SetDebuggerTrue();

	//this is no longer required as it seems to mount perfectly fine


	char buffer[1024];
	//notify("");//not sure why i had a blank notification here anyway
	//if(DEBUGENABlED ==1)
	//{
	//	sprintf(buffer,"Info %s %s",TITLEID,fingerprint);
	//	notify(buffer);
	//}

	//if ((fingerprint != NULL) && (fingerprint[0] == '\0')) 
	//{
	//	printf("Fingerprint is empty assigning defualt\n");
	//	fingerprint = "00000000000000000000000000000000000000000";
	//	//if you dont want to do the keystone verification process use the below
	//	//294a5ed06db170618f2eed8c424b9d828879c080cc66fbc4864f69e974deb856 ////tis will only work for debug games
	//}

	/*This structure sets the fingerprint of the passcode to use when mounting a save data directory.

	A fingerprint is a key for protecting save data from access by other applications. When a passcode (character string comprised of "a" to "z", "A" to "Z", and "0" to "9") is specified during application package creation, the fingerprint can be obtained as a hash value of the passcode (for details, refer to the "Package Generator User's Guide" document. When another application mounts this application's save data, this fingerprint must be specified and then read only mode must be specified.

	On the other hand, there is no need for a fingerprint to be specified when an application mounts its own save data.

	*/

	ret =SaveDataTestMount2(TITLEID,SaveToMount);//not sure if this is the sealed key or not 
	if(ret < 0)
	{
		if(ret = SCE_SAVE_DATA_ERROR_MOUNT_FULL)
		{
			notify("Save Mount Full");
			return ret;
		}
		else if(ret == SCE_SAVE_DATA_ERROR_PARAMETER)
		{
			notify("Save Mounted");
		}
	}
	return ret;

}

PRX_EXPORT int UnMountSaveData()
{
	int ret = 0;
	initSysUtil();
	initSaveData();//save data
	for (int i = 0; i < TotalSavesMounted; i++)
	{
		SceSaveDataMountPoint mountPoint ;
		mountPoint = SaveDataMountPoints[i];

		if (SaveDataUnmount(&mountPoint) < 0 ) {
			// Error handling
			notify("Error Unmounting Save Data");
		}
	}
	return ret;
}


PRX_EXPORT bool DeleteSaveData(char* SaveToDelete)
{
	if(SaveDataDelete(SaveToDelete) ==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

PRX_EXPORT bool DeleteSaveDataGame(char* TitleId)
{
	if(SaveDataDeleteGame(TitleId) ==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

PRX_EXPORT bool DeletAllSavesForUser()
{
	if(SaveDataDeleteAllForUser() == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


PRX_EXPORT bool ShowSaveDataDialog()
{
	int ret =0;
	//things to initilize
	initSysUtil();
	InitlizieUserService();
	InitilizeSaveDataDialog();
	initSaveData();
	ret = orbisSaveDataInitialize3(NULL);//call it with a null


	if ( orbisSaveDataDialogInitialize() < 0 ) {
		// Error handling
		notify("Could not Initialize save Data Dialog");
	}


	return true;

}


#pragma endregion << Save Data >>

#pragma region << Kernel Calls >>


#pragma region << Patch Stuff >>

//the idea here is to enable the system to allow patching to kernel via our 
//static inline int proc_write_mem(struct proc *p, void *ptr, size_t size, void *data, size_t *n) 
//{
//	return proc_rw_mem(p, ptr, size, data, n, 1);
//}
//
//
//static int proc_rw_mem(struct proc *p, void *ptr, size_t size, void *data, size_t *n, int write)
//{
//	struct thread *td = curthread();
//	struct iovec iov;
//	struct uio uio;
//	int r = 0;
//
//	if (!p)
//		return -1;
//
//	if (size == 0) {
//		if (n)
//			*n = 0;
//
//		return 0;
//	}
//
//	memset(&iov, 0, sizeof(iov));
//	iov.iov_base = (uint64_t)data;
//	iov.iov_len = size;
//
//	memset(&uio, 0, sizeof(uio));
//	uio.uio_iov = (uint64_t)&iov;
//	uio.uio_iovcnt = 1;
//	uio.uio_offset = (uint64_t)ptr;
//	uio.uio_resid = (uint64_t)size;
//	uio.uio_segflg = UIO_SYSSPACE;
//	uio.uio_rw = write ? UIO_WRITE : UIO_READ;
//	uio.uio_td = td;
//
//	r = proc_rwmem(p, &uio);
//
//	if (n)
//		*n = (size_t)((uint64_t)size - uio.uio_resid);
//
//	return r;
//}


#pragma endregion << Patch Stuff >>

//#define RESOLVE(module, name) getFunctionAddressByName(module, #name, &name)
//
//int getFunctionAddressByName(int loadedModuleID, char *name, void *destination);


PRX_EXPORT bool LaunchApp(char * titleId)
{
	SystemServiceLaunchApp(titleId);
}

PRX_EXPORT bool LoadExec(const char *path, char *const *argv)
{
	try
	{
		initSysUtil();

		SysLoadExec(path,argv);
	}
	catch(std::exception e)
	{
		notify((char*)e.what());

	}
	return true;
}

int _sceKernelGetModuleInfoEx(SceKernelModule handle, SceKernelModuleInfoEx* info) {
	int ret;

	if (!info) {
		ret = SCE_KERNEL_ERROR_EFAULT;
		goto err;
	}

	memset(info, 0, sizeof(*info));
	{
		info->size = sizeof(*info);
	}

	ret = syscall(SYS_dynlib_get_info_ex, handle, info); /* TODO: make proper error code */

err:
	return ret;
}

int(*sceKernelDebugOutText)(int dbg_channel, const char* text);
void(*kprintf)(const char* fmt, ...) = 0;
//Finally we should now have the KLOG funciton
void klog(const char* fmt, ...)
{

	try

	{
		if(sceKernelDebugOutText)
		{
			char buffer[0x400] = { 0 };
			va_list args;
			va_start(args, fmt);
			vsprintf(buffer, fmt, args);
			va_end(args);

			char buffer2[0x400] = { 0 };
			sprintf(buffer2, "[universal] %s\n", buffer);

			sceKernelDebugOutText(0, buffer2);
		}
		else
		{
			int Libkernel_library = 0;
			if (sys_dynlib_load_prx("/system/common/lib/libkernel.sprx", &Libkernel_library))
				if (sys_dynlib_load_prx("libkernel_web.sprx", &Libkernel_library))
					sys_dynlib_load_prx("libkernel_sys.sprx", &Libkernel_library);
			/*From now on I want to do it this way*/
			//int kernel_lib = sceKernelLoadStartModule("/system/common/lib/libkernel.sprx", 0, NULL, 0, NULL, NULL);
			//sceKernelDlsym(kernel_lib, "sceKernelDebugOutText", (void **)&sceKernelDebugOutText);//this will hook into the libkernel and get the call for us

			sys_dynlib_dlsym(Libkernel_library, "sceKernelDebugOutText", &sceKernelDebugOutText);

			char buffer[0x400] = { 0 };
			va_list args;
			va_start(args, fmt);
			vsprintf(buffer, fmt, args);
			va_end(args);

			char buffer2[0x400] = { 0 };
			sprintf(buffer2, "[universal] %s\n", buffer);

			sceKernelDebugOutText(0, buffer2);
		}
	}
	catch(std::exception e)
	{
		notify((char*)e.what());
	}
}

void logshit(char* format, ...)
{
	char buffer[0x400] = { 0 };

	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	//sceKernelDebugOutText(DGB_CHANNEL_TTYL, buff);


	int fd = sceKernelOpen("/user/app/XDPX20004/logs/loader.log", O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd >= 0)
	{
		//read first
		char buffer1[4096];
		int bytes_read = sceKernelRead(fd, buffer1, sizeof(buffer1));
		strcat(buffer1,buffer); // append string two to string one.
		sceKernelWrite(fd, buffer1, strlen(buffer1));
		sceKernelClose(fd);
	}
}

//void logshit(char* format, ...)
//{
//	char* buff[1024];
//	memset(buff, 0, sizeof(buff));
//
//	va_list args;
//	va_start(args, format);
//	vsprintf(buff, format, args);
//	va_end(args);
//
//	sceKernelDebugOutText(DGB_CHANNEL_TTYL, buff);
//
//	int fd = sceKernelOpen("/user/app/NPXS39041/logs/loader.log", O_WRONLY | O_CREAT | O_APPEND, 0777);
//	if (fd >= 0)
//	{
//		sceKernelWrite(fd, buff, strlen(buff));
//		sceKernelClose(fd);
//	}
//}

PRX_EXPORT const char* GetCallableList()
{
	mkdir("/user/app/XDPX20004/logs/", 0777);
	unlink("/user/app/XDPX20004/logs/loader.log");
	mkdir("/user/app/XDPX20004/", 0777);
	initSysUtil();

	//int sceKernelGetModuleList(SceKernelModule *array, size_t numArray, size_t *actualNum);
	int kernel_lib = sceKernelLoadStartModule("/system/common/lib/libkernel.sprx", 0, NULL, 0, NULL, NULL);
	size_t moduleCount;
	SceKernelModule modules [1];
	modules[0] = kernel_lib;

	//ofstream outfile("myfile.txt", ofstream::binary);

	sceKernelGetModuleList(modules,ModulesSize,&moduleCount);
	//now our code should give us the total modules

	char mess [180];
	sprintf(mess,"<Module List (%d modules)>\n", moduleCount);
	notify(mess);
	logshit(mess);
	for(int ix = 0; ix < moduleCount; ix++)
	{
		sprintf(mess,"<Module %d Loaded>\n", ix);
		notify(mess);
		int ret, opt = sizeof(opt), reload_mod;
		SceKernelModuleInfoEx info = {0};
		info.size = sizeof(info);
		if ((ret = _sceKernelGetModuleInfoEx(kernel_lib, &info)) < 0) {
			sprintf(mess,"Failed to get module info for UID %08x, ret %08x\n", kernel_lib, ret);
			notify(mess);

			logshit(mess);
			return mess;
		}
		else
		{

			sprintf(mess,"Got module info for UID %08x, ret %08x\n",info.name, ret);
			notify(mess);
			//logshit(mess);

			sprintf(mess,"Module %d info \n *Module Name %s\n *Module Refs %s",ix,info.name,info.numRefs);
			notify(mess);
			logshit(mess);
		}
		//else we have info lets display them
		for (int i = 0; i < info.numSegments; i++)
		{
			sprintf(mess,"Module Info 0x%08x_%s_%d",info.segmentInfo[i].baseAddr,info.name,i );
			notify(mess);
			logshit(mess);
		}

		//print_modinfo(ids[i], verbose);
	}
	return mess;
}

PRX_EXPORT const char* GetListOfServices()
{
	char RTNCHAR[1500];
	int ret =-1;
	loadAndImport();

	initSysUtil();

	void* idpsholder = (void *)malloc(1500);
	memset(idpsholder,0,1500);
	size_t bufferlen  = 1500;
	//ret = sysctl(0,idpsholder,bufferlen, NULL, NULL);//just to bypass the ps4 control namne
	//ret = -1;
	if(ret < 0)
	{
		notify("sysctl List of services Failed");
		return RTNCHAR;//all wne to hell 
	}	
	sprintf(RTNCHAR, "%s", idpsholder);
	return RTNCHAR;
}

PRX_EXPORT const char* GetIDPS()
{
	return KernelGetIDPS();
}

PRX_EXPORT const char* GetPSID()
{
	return KernelGetPSID();
}

//this calls sceKernelGetSystemSwVersion
//PRX_EXPORT const char* GetKernelVersion()
//{
//	char* PSID;
//	loadAndImport();	
//	initSysUtil();
//	int ret = -1;
//    char* psid = (char *)malloc(1024*2);
//    memset(psid, 0, 1024*2);
//	
//	ret =sceKernelGetSystemSwVersion(psid);
//	if(ret < 0)
//	{
//		notify("sceKernelGetSystemSwVersion Failed");
//		return PSID;
//	}
//	/*char psid_buf[255];
//	for (int i = 0; i<250; i++) {
//		sprintf(psid_buf+i, "%x", psid[i]);
//	}*/
//	char buffer[1024*2];
//	 sprintf(buffer, "%s", psid);
//	 PSID = buffer;
//	 notify(buffer);
//	return PSID;
//}

PRX_EXPORT const char* KernelGetOpenPsId()
{
	return KernelGetOpenPSID();
}


//gets the version as an int
PRX_EXPORT int get_fw()
{
	return Kernel_Get_Fw();
}

//uint16_t g_firmware;
//PRX_EXPORT uint16_t get_firmware() {
//  if (g_firmware) {
//    return g_firmware;
//  }
//
//  uint16_t ret;                   // Numerical representation of the firmware version. ex: 505 for 5.05, 702 for 7.02, etc
//  uint32_t offset;                // Offset for firmware's version location
//  uint16_t elf_header_size;       // ELF header size
//  uint16_t elf_header_entry_size; // ELF header entry size
//  uint16_t num_of_elf_entries;    // Number of entries in the ELF header
//  char binary_fw[2] = {0};        // 0x0000
//  char string_fw[5] = {0};        // "0000\0"
//  char sandbox_path[33];          // `/XXXXXXXXXX/common/lib/libc.sprx` [Char count of 32 + nullterm]
//
//  snprintf_s(sandbox_path, sizeof(sandbox_path), "/%s/common/lib/libc.sprx", sceKernelGetFsSandboxRandomWord1());
//  int fd = open(sandbox_path, O_RDONLY, 0);
//  if (fd < 0) {
//    // Assume it's currently jailbroken
//    fd = open("/system/common/lib/libc.sprx", O_RDONLY, 0);
//    if (fd < 0) {
//      // It's really broken
//      return -1;
//    }
//  }
//}

// Get the firmware version on the kernel (but can be spoofed !)
PRX_EXPORT const char* firmware_version_kernel() {
	return Kernel_Firmware_Version();
}


PRX_EXPORT uint16_t get_firmware() {
	return Kernel_Get_FW_Version();
}

//Get the firmware version by libc (for prevent from kernel spoof)
//Should no longer be required thanks to LM
PRX_EXPORT char* firmware_version_libc()
{
	return Kernel_firmware_version_libc();
}

//this is needed for mono uses the same argumnents it seems

/*
You have got to be kidding me 
_int64 __fastcall sceKernelLoadStartModule(__int64 a1, __int64 a2, __int64 a3, int a4, __int64 a5, _DWORD *a6)
{
__int64 result; // rax

if ( a4 )
result = 2147614742LL;
else
//starts unity when a4 = 0
result = sceKernelLoadStartModuleInternalForMono_0(a1, a2, a3, 0, a5, a6);
return result;
}
*/
//SceKernelModule sceKernelLoadStartModuleInternalForMono(const char *moduleFileName,
//        size_t args, const void *argp, uint32_t flags,
//        const SceKernelLoadModuleOpt *pOpt, int *pRes);



PRX_EXPORT char* GetSandboxPath()
{
	return Kernel_GetSandboxPath();
}




#pragma endregion << Kernel Calls >>

#pragma region << User Service Functions >>

PRX_EXPORT char* GetUsername()
{
	try
	{
		return  UserServiceGetUserName();
	}
	catch(std::exception e)
	{
		notify((char*)e.what());
		return "";
	}
}

PRX_EXPORT const char* GetUserId()
{
	try
	{	
		return UserServiceGetUserId();
	}
	catch(std::exception e)
	{
		notify((char*)e.what());
		return "";
	}

}


#pragma endregion << User Service Functions >>

#pragma region << App 2 USB >>

#define SYS_link        9
#define SYS_unlink      10
#define SYS_symlink     57
#define SYS_readlink    58
//big thanks to celestblue for helping me find the syscall numbers
//the idea behind this is to link the files needed to unlock trophies
//once linked we can change the actual trophy file (license file) then allow the console to unlock it via homebrew
void LinkFile()
{
	//syscall(SYS_symlink,
}

#pragma endregion << App 2 USB >>

#pragma region << Trophies >>


//struct _tai_hook_user {
//  uintptr_t next;
//  void *func;
//  void *old;
//};
//typedef uintptr_t tai_hook_ref_t;
//
//#define TAI_CONTINUE(type, hook, ...) ({ \
//  struct _tai_hook_user *cur, *next; \
//  cur = (struct _tai_hook_user *)(hook); \
//  next = (struct _tai_hook_user *)cur->next; \
//  (next == NULL) ? \
//    ((type(*)())cur->old)(__VA_ARGS__) \
//  : \
//    ((type(*)())next->func)(__VA_ARGS__) \
//  ; \
//})
//
//
//
//static tai_hook_ref_t trophy_cxt_hook;
//char display[128];
//int isInit = 0,isCxt = 0;
//SceUID sceNpTrophyCreateContext_patched(SceNpTrophyContext *c, uint32_t commId,  uint32_t commSign, uint64_t options) {
//	notify("\nContext called");
//	int ret = TAI_CONTINUE(SceUID, trophy_cxt_hook, c, commId, commSign, options);
//	context = c;
//	notify("\nContext found: %x %x", *context, *c);
//	if(ret<0) {
//		snprintf(display,128, "Error: SceNpTrophy cxt has ret: %x", ret);
//	} else {
//		isCxt = 1;
//	}	
//
//	return ret;
//}
//static tai_hook_ref_t trophy_init_hook;
//
//SceUID sceNpTrophyInit_patched(uint32_t r1, uint32_t r2, uint32_t r3, uint64_t r4) {
//	notify("\nSceNpTrophy has init");
//	int ret = TAI_CONTINUE(SceUID, trophy_init_hook, r1, r2, r3, r4);
//	if(ret<0) {
//		snprintf(display,128, "Error: SceNpTrophy init has ret: %x", ret);
//	} else {
//		isInit = 1;
//	}
//
//	return ret;
//}
//
//static tai_hook_ref_t fame_hook;

int GetTrophyCount(int32_t contxt,int32_t hndl)
{
	SceNpTrophyGameDetails details;
	int ret =sceNpTrophyGetGameInfo(contxt, hndl, &details, NULL);
	char mess [180];
	sprintf(mess, "Title\n%s", details.title);
	notify(mess);
	return details.numTrophies;
}

/*Not sure why but this does not work at all*/
PRX_EXPORT int MakeCusaAppReadWrite()
{
	char* path = "/mnt/sandbox/XDPX20004_000/app0/sce_sys/";
	if (chmod(path,777) <= 0) 
	{
		return -1;
		//We can redo these permissions nice !
	}
	path = "/mnt/sandbox/pfsmnt/XDPX20004-app0/";
	if (chmod(path,777) <= 0) 
	{
		return -1;
		//We can redo these permissions nice !
	}
	return 1;
}

/*Example code provided*/
PRX_EXPORT int UnlockTrophies(char* TitleId,char * Titleidsecret)
{
	//NPWR04914_00 playroom
	//CUSA01656
	int ret = -1;
	initSysUtil();
	initsysNpTrophy();
	initsysNpManager();
	notify("NpTrophy Loaded");
	char buffer[180];
	ret =InitlizieUserService();
	if(ret < 0)
	{
		sprintf(buffer, "UserServiceError\n%d", ret);
		notify(buffer);
		//notify("InitlizieUserService error");
		return ret;
	}
	SceNpTrophyContext testcontext = SCE_NP_TROPHY_INVALID_CONTEXT;

	SceNpServiceLabel servicelabel;
	memset(&servicelabel,0x00,sizeof(servicelabel));
	memcpy(&servicelabel,"CUSA10866_00",sizeof(int32_t));
	notify("Starting Unlock");
	//extern SceNpServiceLabel serviceLabel;

	SceNpTitleId titleId;
	memset(&titleId, 0x00, sizeof(titleId));
	strcpy(titleId.id, TitleId);//title id all of this can be found in the NPTitle.Dat file
	//memset(&titleId, 0x00, sizeof(titleId));
	//strlcpy(titleId.data, TITLEID, sizeof(titleId.data));
	SceNpTitleId npTitleId = {
		{'C','U','X','S','2','9','0','1','5','_','0','0','\0'},
	};
	strcpy(npTitleId.id, TitleId);//title id all of this can be found in the NPTitle.Dat file
	//memset(&titleId, 0x00, sizeof(titleId));
	//strlcpy(titleId.data, TITLEID, sizeof(titleId.data));
	//strcpy(&titleid,"CUSA01656",sizeof(SceNpTitleId));
	SceNpTitleSecret titleSecret ;
	//memset(&titleSecret, 0x00, sizeof(titleSecret));
	//memcpy(titleSecret.data,"D20246C55F1C7A780D78F650AD9775351E203B9E56489FFA2CD57E9B49C69B1828DCB964833C9254208EE57E0950AC6D5EBD6423C3B078A0E75BB76419FA99541CB1C100ECEED5083A2D5099F574EFC98511265A9F36E5151141789B52440E4ACB8340BAA62CB4D27A884FF1F34821C7E0121BADBD3639E404C6D3D07CA6140E", sizeof(titleSecret.data));
	memcpy(titleSecret.data,Titleidsecret,sizeof(titleSecret.data));
	//notify("SettingNpTitleIDAndSecret");
	const SceNpTitleSecret s_npTitleSecret = {
		{
			0xDA , 0xC4 , 0x40 , 0x1F , 0x6C , 0x3B , 0x9D , 0xE9 , 0xD4 , 0x7C , 0xA2 , 0xBF , 0x90 , 0xD7 , 0x49 , 0xA0 , 0xCE , 0x1C , 0x70 , 0xA4 , 0x4E , 0xAA , 0x00 , 0x6A , 0xDC , 0x64 , 0x57 , 0xEB , 0x10 , 0xD3 , 0x0E , 0xE9 , 0x38 , 0x22 , 0xA2 , 0xD7 , 0x2E , 0x62 , 0x39 , 0x3B , 0xAC , 0xDB , 0x97 , 0x75 , 0x2E , 0x7F , 0x72 , 0x5B , 0x10 , 0xB3 , 0x19 , 0xFE , 0x85 , 0x1D , 0x16 , 0xBC , 0x7C , 0x82 , 0xB2 , 0x2F , 0x81 , 0xFD , 0x3C , 0xCB , 0x91 , 0x6F , 0x56 , 0xB9 , 0x7D , 0xF9 , 0xD3 , 0xE7 , 0x15 , 0x33 , 0xCD , 0x92 , 0x8B , 0x58 , 0xB5 , 0xA3 , 0x1A , 0xB7 , 0x4D , 0x56 , 0xBF , 0xC7 , 0xF5 , 0x02 , 0x53 , 0xCB , 0x95 , 0x38 , 0x54 , 0x1B , 0x04 , 0xF3 , 0x11 , 0xF4 , 0xD8 , 0x70 , 0x92 , 0xCD , 0x74 , 0x2E , 0x85 , 0x63 , 0x8B , 0x7B , 0x03 , 0x62 , 0x24 , 0xA4 , 0x54 , 0xEF , 0xA8 , 0xC8 , 0x6D , 0x25 , 0x5D , 0xC1 , 0x40 , 0x3B , 0xE6 , 0x38 , 0xFA , 0xAE , 0x0D , 0x7C
		}
	};

	char username[32];

	if(userId == SCE_USER_SERVICE_USER_ID_INVALID)
	{
		ret = GetInitialUser();
		if(ret < 0)
		{
			notify("sceUserServiceGetInitialUser failed");
			return ret;
		}
	}
	(void)memset(username, 0, sizeof(username));

	if (GetUserName(userId, username, sizeof(username) - 1) < 0)
	{
		notify("[DEBUG] [ERROR] Failed to get username!\n");
		return -1;
	}
	//first create the handle
	notify("Trying to set NpTitleId");

	//this will be ignored if an application ps4 is not in release mode = developer mode //soooooooo thus us usless then
	ret = sceNpSetNpTitleId(&npTitleId, &titleSecret);
	if (ret < 0) {
		buffer[180];
		sprintf(buffer, "sceNpSetNpTitleId Error\n0x%08x", ret);
		notify(buffer);
		return ret;
	}

	//well would you look at that this break .... great 

	/*ret =sceNpSetNpTitleId(&titleId,&titleSecret);
	{
	buffer[180];
	sprintf(buffer, "sceNpSetNpTitleId Error\n0x%08x", ret);
	notify(buffer);
	return ret;
	}*/


	buffer[180];
	sprintf(buffer, "Unlocking Trophies For\n%s", npTitleId.id);
	notify(buffer);

	return 0;//just for now

	SceNpTrophyContext localcontext = 1;
	SceNpTrophyHandle localhandle = 1;


	ret = sceNpTrophySystemCreateHandle(&localhandle);
	if(ret <0)
	{
		buffer[500];
		sprintf(buffer, "sceNpTrophySystemCreateHandle Error\n0x%08x", ret);
		notify(buffer);
		return ret;
	}
	buffer[500];
	sprintf(buffer, "Handle is \n0x%08x", localhandle);
	notify(buffer);



	ret = sceNpTrophySystemCreateContext(&localcontext,userId,&localhandle,0);
	if(ret <0)
	{
		buffer[500];
		sprintf(buffer, "sceNpTrophyCreateContext Error\n0x%08x", ret);
		notify(buffer);
		return ret;
	}
	buffer[500];
	sprintf(buffer, "Context is \n0x%08x", localcontext);
	notify(buffer);


	ret = sceNpTrophyRegisterContext(localcontext,localhandle,0);

	if(ret <0)
	{
		buffer[500];
		sprintf(buffer, "sceNpTrophyRegisterContext Error\n0x%08x", ret);
		notify(buffer);
		return ret;
	}

	notify("doing a new thing");


	SceNpTrophyId trophyId = 0;
	do
	{	

		for (int32_t trophyId = 0; trophyId < GetTrophyCount(localcontext++,localhandle); trophyId++)
		{
			SceNpTrophyId plat;
			buffer[500];
			sprintf(buffer, "info trophy id \n %d", trophyId);
			notify(buffer);
			ret = NpTrophyUnlockTrophy(localcontext, localhandle, trophyId++, &plat);
			if(ret <0)
			{
				buffer[500];
				sprintf(buffer, "sceNpTrophyUnlockTrophy Error\n0x%08x", ret);
				notify(buffer);
				//return ret;
			}
			if(ret == 0x8055160e || ret == 0x8055160f)
			{
				return ret;
			}
		}
	}
	while(trophyId == 1 || ret >= 0 || ret == 0x8055160f );

	return 0;
	/*notify("string copy");
	char test [12];
	memset(test,0,sizeof(test));
	strcpy(test,"CUSA10866_00");*/

	//we need to create the context for each game somehow maybe its saved in one of the tables ?
	ret = sceNpTrophyCreateContext(&testcontext,userId,0,0);//no options
	if(ret < 0)
	{
		//0x%08x
		buffer[180];
		sprintf(buffer, "sceNpTrophySystemCreateContext Error\n0x%08x", ret);
		notify(buffer);
		return ret;
	}

	notify("Starting Unlock xdpx test");



	//sceNpTrophyCreateHandle(&handle);
	SceNpTrophyId plat;
	SceNpTrophyId trophyId1 = 0;
	ret = NpTrophyUnlockTrophy(1,1,2,&plat);
	if(ret < 0)
	{
		buffer[180];
		sprintf(buffer, "NpTrophyUnlockTrophy Error\n0x%08x", ret);
		notify(buffer);
		return ret;
	}


	//SceNpTrophyId plat;
	//SceNpTrophyId trophyId = 0;
	//do
	//{
	//	ret = NpTrophyUnlockTrophy(*context, handle, trophyId++, &plat);
	//}
	//while(trophyId == 1 || ret >= 0 || ret == 0x8055160f );


}

#pragma endregion << Trophies >>

#pragma region << Mountstuff >>


int (*sceFsMountTrophyData)(int a1, char path, int a3, int a4, int a5);

/*Research this more in depth*/
//SceFsMountGameDataOpt
////pfsImgFile

//Mount Path
//typedef struct _SceFsMountGameDataOpt {
//	char *pfsImgFile;//*(long *)*param_1 pcVar11 = "%s(%d)\t***ERR*** pfsImgFile not specified.\n";
//	long field_0x8;//can be 0 or something else 
//	long field_0x10;//use 1 check bitmap ?
//	long field_0x18;//use DDR3 ? //          == 0    pcVar11 = "%s(%d)\t***ERR*** ddr3 cache should be enabled for outer pfs image!\n";
//					
//
//	int field_0x20;	// == 0               pcVar11 =  "%s(%d)\t***ERR*** gddr5 cache should not be enabled to mount outer pfs image!\n";
//					// "%s(%d)\t***ERR*** gddr5 cache should be enabled to mount inner pfs image!\n";
//					// "%s(%d)\t***ERR*** gddr5 cache should not be used for non-game pfsimage!\n"
//	char field_0x24[16];//0 for now
//	long field_0x38;//0 for now
//	char field_0x40;//0 for now//key revision 4 ?
//	uint field_0x44;//Budget not specified
//	char * field_0x48;
//} SceFsMountGameDataOpt;
//int (*sceFsMountGamePkg)(const _SceFsMountGameDataOpt *pfsImgFile, char *mountpath, int a3, int a4);

typedef struct SceFsMountGameDataOpt
{
	const char* pfsImgFile; 		//0x00

};

typedef struct pConfig
{
	uint64_t VolumeSize;	//0x18
};

int (*sceFsMountGamePkg)(
	SceFsMountGameDataOpt* Opt, 
	char* MountPath, 
	int32_t unk1, // Gets stored on the stack is passed into pfsMountData
	pConfig* i_pConfig, //structure of some sort it seems
	int32_t unk3, 
	int32_t unk4
	);

//errors 
/*    if (mountpath == 0) {
pcVar11 = "%s(%d)\t***ERR*** Mount path not specified.\n";*/

//now read it to another location (Dump to use)
#define READ_SIZE	(256)
#define NB_PAGES	(256)


/* UTILS **********************************************************************/
void hexprint(const uint8_t *input, const int input_length)
{
	int i = 0;

	for (i = 0; i < input_length; i++) {
		if (i%16 == 0) {
			printf("\n");
		}
		printf("%02x ", input[i]);
	}
	printf("\n");
}

void dump_to_file(const char *output_file_path,
				  const char *buffer, const int buffer_length)
{
	int output_file = open(output_file_path, O_RDWR|O_APPEND|O_CREAT);
	if (output_file < 0) {
		printf("Failed opening output file %s\n", output_file_path);
		return;
	}

	write(output_file, buffer, buffer_length);
}




PRX_EXPORT int MountandLoad()
{

	loadAndImport();
	initSysUtil();
	notify("Loading authmgr");



	if(DEBUGENABlED == 1)
	{
		notify("Loading libSceFsInternalForVsh");
	}
	int sys = sceKernelLoadStartModule("/system/priv/lib/libSceFsInternalForVsh.sprx", 0, 0,0, 0, 0);
	int ret = sceKernelDlsym(sys, "sceFsMountGamePkg", (void**)&sceFsMountGamePkg);	
	char buffer[180];
	if(ret < 0)
	{
		sprintf(buffer, "sceFsMountGamePkg\n%d", ret);
		notify(buffer);
		return ret;
	}
	SceFsMountGameDataOpt dataopt;
	memset(&dataopt,0x00,sizeof(SceFsMountGameDataOpt));
	memcpy(&dataopt.pfsImgFile,"/user/app/CUSA00327/app.pkg",sizeof(char));

	pConfig config;
	memset(&config,0x00,sizeof(pConfig));
	uint64_t volumesize = 348258304;
	memcpy(&config.VolumeSize,&volumesize,sizeof(uint64_t));
	//dataopt.pfsImgFile
	notify("Trying to mount");
	ret =  sceFsMountGamePkg(&dataopt,"/mnt/pfs/",0,&config,0,0);
	if(ret < 0)
	{
		sprintf(buffer, "sceFsMountGamePkg\n%d", ret);
		notify(buffer);
		if(ret == SCE_KERNEL_ERROR_EINVAL)
		{
			sprintf(buffer, "Invalid Arguments\n%d", ret);
			notify(buffer);
		}
		//we have some errors cool

		return ret;
	}
	//we have mounted the sceFsMountGamePkg
	notify("profit ?");


	return 0;
}





//I can't even remember how i got to this .... basically you can open up pkg's(trophies savedata games) using iovec .... u just need the ekpfs key
PRX_EXPORT int MountTrophy()
{



	struct iovec* iov = NULL;
	int iovlen = 0;
	int ret = 0 ;
	build_iovec(&iov, &iovlen, "fstype", "pfs", -1);
	build_iovec(&iov, &iovlen, "from", "/dev/lvd5", -1);
	build_iovec(&iov, &iovlen, "fspath", "/mnt/pfs/savedata_1a42e7c2_CUSA00677_CUSA00677JDSAVE0", -1);
	build_iovec(&iov, &iovlen, "sigverify", "0", -1);
	build_iovec(&iov, &iovlen, "ekpfs", "3e74e6a13bb8a84c46f280b609ae1f2ec1c21a42edd038d760b49a65827b4c1b", -1);
	build_iovec(&iov, &iovlen, "mkeymode", "SD", -1);
	build_iovec(&iov, &iovlen, "budgetid", "game", -1);
	build_iovec(&iov, &iovlen, "playgo", "0", -1);
	build_iovec(&iov, &iovlen, "disc", "0", -1);
	build_iovec(&iov, &iovlen, "errmsg",NULL,-1);
	ret = syscall(378,iov, iovlen, MNT_UPDATE);
	//ret = nmount(iov, iovlen, flags);
}
//sceFsMountGamePkg



#pragma endregion << Mount Stuff >>

#pragma region << Control >>

PRX_EXPORT int Change_Controller_Color(int r,int g,int b)
{
	int ret =0;
	initSysUtil();
	//SetDebuggerTrue();/*Toggles debug notifications*/
	if(DEBUGENABlED == 1)
	{
		notify("Initlizing pad");
	}
	initScePad();
	initsysUserService();

	char buffer[1024];
	ret = scePadInit();
	if(ret < 0)
	{
		sprintf(buffer, "initlizes pad Error\n\n0x%08x", ret);
		notify(buffer);
		return ret;
	}

	ret = InitlizieUserService();
	if(ret < 0)
	{
		sprintf(buffer, "InitlizieUserService Error\n\n0x%08x", ret);
		notify(buffer);
		return ret;
	}

	if(userId == SCE_USER_SERVICE_USER_ID_INVALID)
	{

		ret = GetInitialUser();
		if(ret < 0)
		{
			notify("sceUserServiceGetInitialUser failed");
			return ret;
		}
	}
	if(DEBUGENABlED == 1)
	{
		notify("Pad is opening");
	}
	int pad = scePadOpen(userId,0,0,NULL);
	if(pad < 0)
	{
		sprintf(buffer, "scePadOpenError\n\n0x%08x", ret);
		notify(buffer);
		return pad;
	}

	if(DEBUGENABlED == 1)
	{
		sprintf(buffer, "Pad is open\n\n0x%08x", pad);
		notify(buffer);
	}	

	ret = scePadGetHandle(userId,0,0);
	if(ret < 0)
	{
		sprintf(buffer, "scePadGetHandle Error\n\n0x%08x", ret);
		notify(buffer);
		return ret;
	}

	if(DEBUGENABlED == 1)
	{
		sprintf(buffer, "scePadGetHandle is \n\n0x%08x", ret);
		notify(buffer);
	}	

	int isSupportedAudioFunction = 0;
	// Check if a controller is connected or not
	//ScePadControllerInformation information;
	//ret = scePadGetControllerInformation(ret, &information);
	//if(ret == 0){
	//	// Controller information was successfully obtained
	//	if(information.connected){
	//	  // Check if the audio features of the connected controller are supported or not
	//	  isSupportedAudioFunction = scePadIsSupportedAudioFunction(handle);
	//	  if(isSupportedAudioFunction < 0){
	//		  // Error handling
	//		  /*sprintf(buffer, "scePadIsSupportedAudioFunction Error\n\n0x%08x", ret);
	//		  notify(buffer);*/
	//		  //return ret;
	//	  }
	//	}
	//}else if(ret < 0){
	//	// Error handling 
	//	 sprintf(buffer, "scePadGetControllerInformation Error\n\n0x%08x", ret);
	//	 notify(buffer);
	//	 ret;
	//}
	if(DEBUGENABlED == 1)
	{
		sprintf(buffer, "ColorPad is \n\nR: %d G: %d B: %d A: 1",r,g,b);
		notify(buffer);
	}	
	OrbisPadColor params;
	params.b = b;
	params.r = r;
	params.g = g;
	params.a = 1;
	if(DEBUGENABlED == 1)
	{
		sprintf(buffer, "ColorPad is \n\nR: %d G: %d B: %d A: 1",r,g,b);
		notify(buffer);
	}	
	ret = scePadSetLightBar(pad,&params);
	if(ret < 0)
	{
		notify("Set Pad Color failed");
		return ret;
	}
	ret =scePadClose(pad);
	if(ret < 0)
	{
		notify("Closing pad failed");
		return ret;
	}

	return ret;
}

#pragma endregion << Control >>

#pragma region << Screenshot class >>

PRX_EXPORT int TakeScreenShot()
{
	int ret =0;

	initScePad();
	initSceScreenshot();

	SceScreenShotParam screenshot_param = {0};

	screenshot_param.thisSize = sizeof(SceScreenShotParam);
	screenshot_param.photoTitle = "screenshot custom title";
	screenshot_param.gameTitle = "screenshot custom album";
	screenshot_param.gameComment = "screenshot custom comment";

	ret = sceScreenShotSetParam(screenshot_param);
	if(ret < 0)
	{
		notify("sceScreenShotSetParam failed");
		return ret;
	}
	/*ret = sceScreenShotEnable()
	{
	notify("sceScreenShotEnable failed");
	return ret;
	}*/
	return ret;
}

#pragma endregion << Sceenshot class >>

#pragma region << BGFT >>

//Install PKG From Anywhere //use this to install pkg from homebrew

enum bgft_task_option_t {
	BGFT_TASK_OPTION_NONE = 0x0,
	BGFT_TASK_OPTION_DELETE_AFTER_UPLOAD = 0x1,
	BGFT_TASK_OPTION_INVISIBLE = 0x2,
	BGFT_TASK_OPTION_ENABLE_PLAYGO = 0x4,
	BGFT_TASK_OPTION_FORCE_UPDATE = 0x8,
	BGFT_TASK_OPTION_REMOTE = 0x10,
	BGFT_TASK_OPTION_COPY_CRASH_REPORT_FILES = 0x20,
	BGFT_TASK_OPTION_DISABLE_INSERT_POPUP = 0x40,
	BGFT_TASK_OPTION_DISABLE_CDN_QUERY_PARAM = 0x10000,
};

struct bgft_download_param {
	int user_id;
	int entitlement_type;
	const char* id;
	const char* content_url;
	const char* content_ex_url;
	const char* content_name;
	const char* icon_path;
	const char* sku_id;
	enum bgft_task_option_t option;
	const char* playgo_scenario_id;
	const char* release_date;
	const char* package_type;
	const char* package_sub_type;
	unsigned long package_size;
};

struct OrbisBgftDownloadParam {
	int userId;
	int entitlementType;
	const char* id; /* max size = 0x30 */
	const char* contentUrl; /* max size = 0x800 */
	const char* contentExUrl;
	const char* contentName; /* max size = 0x259 */
	const char* iconPath; /* max size = 0x800 */
	const char* skuId;
	bgft_task_option_t option;
	const char* playgoScenarioId;
	const char* releaseDate;
	const char* packageType;
	const char* packageSubType;
	unsigned long packageSize;
};

struct OrbisBgftDownloadParamEx {
	OrbisBgftDownloadParam params;
	unsigned int slot;
};

struct bgft_download_param_ex {
	struct bgft_download_param param;
	unsigned int slot;
};

struct bgft_task_progress_internal {
	unsigned int bits;
	int error_result;
	unsigned long length;
	unsigned long transferred;
	unsigned long length_total;
	unsigned long transferred_total;
	unsigned int num_index;
	unsigned int num_total;
	unsigned int rest_sec;
	unsigned int rest_sec_total;
	int preparing_percent;
	int local_copy_percent;
};

#define BGFT_INVALID_TASK_ID (-1)

struct bgft_init_params {
	void* mem;
	unsigned long size;
};
struct OrbisBgftInitParams {
	void* heap;
	size_t heapSize;
};
int(*sceBgftInitialize)(struct bgft_init_params*);
int(*sceBgftServiceIntInit)(struct OrbisBgftInitParams*);
//int sceBgftServiceIntDownloadRegisterTaskByStorageEx(OrbisBgftDownloadParamEx* params, OrbisBgftTaskId* taskId);
int(*sceBgftServiceIntDownloadRegisterTaskByStorageEx)(OrbisBgftDownloadParamEx* params, int* taskId);
int(*sceBgftDownloadStartTask)(int);
int bgft;

int(*sceAppInstUtilInitialize)(void);
int (*sceAppInstUtilAppInstallPkg)(const char* file_path, int reserved);
int (*sceAppInstUtilGetTitleIdFromPkg)(const char* pkg_path, char* title_id, int* is_app);
int (*sceAppInstUtilCheckAppSystemVer)(const char* title_id, uint64_t buf, uint64_t bufs);
int (*sceAppInstUtilAppPrepareOverwritePkg)(const char* pkg_path);
int (*sceAppInstUtilGetPrimaryAppSlot)(const char* title_id, unsigned int* slot);
int (*sceAppInstUtilAppUnInstall)(const char* title_id);
int (*sceAppInstUtilAppGetSize)(const char* title_id, uint64_t buf);
int (*sceAppInstUtilAppExists)(const char* title_id,unsigned int flag);

bool AppInstUtil_Initialized = false;
bool AppInstUtil_Init()
{
	int ret = 0;

	if (AppInstUtil_Initialized)
		return true;
	//"/system/common/lib/libSceAppInstUtil.sprx"
	ret = sceKernelLoadStartModule("/system/common/lib/libSceAppInstUtil.sprx", 0,NULL,0,0,0);
	sceKernelDlsym(ret, "sceAppInstUtilInitialize",(void **)&sceAppInstUtilInitialize);
	sceKernelDlsym(ret, "sceAppInstUtilAppInstallPkg",(void **)&sceAppInstUtilAppInstallPkg);
	sceKernelDlsym(ret, "sceAppInstUtilGetTitleIdFromPkg",(void **)&sceAppInstUtilGetTitleIdFromPkg);
	sceKernelDlsym(ret, "sceAppInstUtilCheckAppSystemVer",(void **)&sceAppInstUtilCheckAppSystemVer);
	sceKernelDlsym(ret, "sceAppInstUtilAppPrepareOverwritePkg",(void **)&sceAppInstUtilAppPrepareOverwritePkg);
	sceKernelDlsym(ret, "sceAppInstUtilGetPrimaryAppSlot",(void **)&sceAppInstUtilGetPrimaryAppSlot);
	sceKernelDlsym(ret, "sceAppInstUtilAppUnInstall",(void **)&sceAppInstUtilAppUnInstall);
	sceKernelDlsym(ret, "sceAppInstUtilAppGetSize",(void **)&sceAppInstUtilAppGetSize);
	sceKernelDlsym(ret, "sceAppInstUtilAppExists",(void **)&sceAppInstUtilAppExists);

	ret = sceAppInstUtilInitialize();
	if (ret)
	{
		SendMessageToPS4("sceAppInstUtilInitialize failed:");
		//klog("sceAppInstUtilInitialize failed: 0x%08X", ret);

		AppInstUtil_Initialized = false;

		return false;
	}

	AppInstUtil_Initialized = true;

	return true;
}


static OrbisBgftInitParams s_bgft_init_params;

#define BGFT_HEAP_SIZE (1 * 1024 * 1024)

static bool s_bgft_initialized = false;

int bgft_init(void) {
	int ret;

	if (s_bgft_initialized) {
		goto done;
	}

	memset(&s_bgft_init_params, 0, sizeof(s_bgft_init_params));
	{
		s_bgft_init_params.heapSize = BGFT_HEAP_SIZE;
		s_bgft_init_params.heap = (uint8_t*)malloc(s_bgft_init_params.heapSize);
		if (!s_bgft_init_params.heap) {
			printf("No memory for BGFT heap.\n");
			goto err;
		}
		memset(s_bgft_init_params.heap, 0, s_bgft_init_params.heapSize);
	}

	ret = sceBgftServiceIntInit(&s_bgft_init_params);
	if (ret) {
		printf("sceBgftServiceIntInit failed: 0x%08X\n", ret);
		goto err_bgft_heap_free;
	}

	s_bgft_initialized = true;

done:
	return 0;

err_bgft_heap_free:
	if (s_bgft_init_params.heap) {
		free(s_bgft_init_params.heap);
		s_bgft_init_params.heap = NULL;
	}

	memset(&s_bgft_init_params, 0, sizeof(s_bgft_init_params));

err:
	s_bgft_initialized = false;

	return -1;
}
PRX_EXPORT int InstallPKG(char * path,char * name,char * imgpath)
{
	int is_app,rv;

	char title_id[16];//placeholder for titleid


	bgft = sceKernelLoadStartModule("/system/common/lib/libSceBgft.sprx", 0,NULL,0,0,0);

	sceKernelDlsym(bgft, "sceBgftServiceIntInit",(void **)&sceBgftServiceIntInit);

	sceKernelDlsym(bgft,"sceBgftServiceIntInit",(void **)&sceBgftInitialize);

	sceKernelDlsym(bgft, "sceBgftServiceIntDownloadRegisterTaskByStorageEx",(void **)&sceBgftServiceIntDownloadRegisterTaskByStorageEx);

	sceKernelDlsym(bgft, "sceBgftServiceIntDownloadStartTask",(void **)&sceBgftDownloadStartTask);

	AppInstUtil_Init();

	rv = sceAppInstUtilGetTitleIdFromPkg(path, title_id, &is_app);
	struct bgft_init_params ip = {
		.mem = mmap(NULL, 0x100000, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0),
		.size = 0x100000,
	};
	//rv = sceBgftInitialize(&ip);
	//trying what LM's did in his store open the path then close the path
	int fd = sceKernelOpen(path, 0000, 0x0000);
	if (fd >= 0)
	{

		if(DEBUGENABlED == 1)
			SendMessageToPS4("opening path and closing");
		sceKernelClose(fd);
		//initilize the bf stuff
		if(DEBUGENABlED == 1)
			SendMessageToPS4("bgft_init");
		rv = bgft_init();
		struct bgft_download_param params = {
			.entitlement_type = 5,
			.id = "",
			.content_url = path,
			.content_name = name,
			.icon_path = imgpath,
			.playgo_scenario_id = "0",
			.option = BGFT_TASK_OPTION_DISABLE_CDN_QUERY_PARAM,
		};
		int task = BGFT_INVALID_TASK_ID;
		//rv = sceBgftServiceIntDownloadRegisterTaskByStorageEx(&params, &task);
		OrbisBgftDownloadParamEx download_params;
		memset(&download_params, 0, sizeof(download_params));
		{
			download_params.params.entitlementType = 5;
			download_params.params.id = "";
			download_params.params.contentUrl = path;
			download_params.params.contentName = name;
			download_params.params.iconPath = imgpath;
			download_params.params.playgoScenarioId = "0";
			download_params.params.option = BGFT_TASK_OPTION_DISABLE_CDN_QUERY_PARAM;
			download_params.slot = 0;
		}
		if(DEBUGENABlED == 1)
			SendMessageToPS4("sceBgftServiceIntDownloadRegisterTaskByStorageEx");

		rv = sceBgftServiceIntDownloadRegisterTaskByStorageEx(&download_params, &task);
		if (rv == 0x80990088)
		{
			if(DEBUGENABlED == 1)
				SendMessageToPS4("0x80990088");
			rv = sceAppInstUtilAppUnInstall(title_id);
			if(rv!=0)
			{				
				SendMessageToPS4("Error Uninstalling");
				return 0;
			}

			if(DEBUGENABlED == 1)
				SendMessageToPS4("sceBgftServiceIntDownloadRegisterTaskByStorageEx");
			rv = sceBgftServiceIntDownloadRegisterTaskByStorageEx(&download_params, &task);
		}
		/*else if (rv) 
		{
		SendMessageToPS4("sceBgftServiceIntDownloadRegisterTaskByStorageEx Error");
		return 0;
		}*/if(DEBUGENABlED == 1)
			SendMessageToPS4("sceBgftDownloadStartTask");
		rv = sceBgftDownloadStartTask(task);
		return 0;
	}
	return 0;
}

PRX_EXPORT int UnloadPKGModule()
{
	if(bgft)
	{
		int ret = sceKernelStopUnloadModule(bgft,0, NULL, 0, NULL, NULL);
		return ret;
	}
	return 1;
}

#pragma endregion << BGFT >>

#pragma region agnostic jailbreak

bool(*Jailbreak)();

//for some reason it does not avtually execute the way i wanted it to mght just be for unity apps that it freaks out

PRX_EXPORT bool JailbreakMe()
{
	//initSysUtil();

	int ModuleHandle_libjbc = sceKernelLoadStartModule("/app0/sce_module/libjbc.sprx", 0, nullptr, 0, nullptr, nullptr);
	if (ModuleHandle_libjbc == 0) {
		//notify("Failed to load libjbc Library.\n");
		//klog("Failed to load libjbc Library.\n");
		return false;
	}

	sceKernelDlsym(ModuleHandle_libjbc, "Jailbreak", (void**)&Jailbreak);
	if (Jailbreak == nullptr) {
		//klog("Failed to load Jailbreak Import.\n");
		//notify("Failed to load Jailbreak Import.\n");
		return false;
	}
	return true;
}

#pragma endregion agnostic jailbreak

#pragma region << Drive Spcae >>

typedef struct statvfs {
	unsigned long  f_bsize;    /* filesystem block size */
	unsigned long  f_frsize;   /* fragment size */
	fsblkcnt_t     f_blocks;   /* size of fs in f_frsize units */
	fsblkcnt_t     f_bfree;    /* # free blocks */
	fsblkcnt_t     f_bavail;   /* # free blocks for unprivileged users */
	fsfilcnt_t     f_files;    /* # inodes */
	fsfilcnt_t     f_ffree;    /* # free inodes */
	fsfilcnt_t     f_favail;   /* # free inodes for unprivileged users */
	unsigned long  f_fsid;     /* filesystem ID */
	unsigned long  f_flag;     /* mount flags */
	unsigned long  f_namemax;  /* maximum filename length */
};
PRX_EXPORT int calculate_storage_size( )
{

	char *temp_char_ptr = (char *)NULL;
	int storage_size_percent = -1;  
	FILE *fp ;
	fp = fopen ("disk.stat" , "r");
	if (fp != (FILE *)NULL)
	{
		temp_char_ptr = (char*) calloc ( 6 , 1 );
		fscanf( fp,"%s %s %s %s %d", temp_char_ptr, temp_char_ptr, temp_char_ptr, temp_char_ptr, &storage_size_percent);
	}
	free (temp_char_ptr);
	fclose(fp);
	return storage_size_percent;

}

long GetDiscStat(const char* path)
{

}

/*Powered By LM's homebrew store*/
static const char     *sizes[] = { "EiB", "PiB", "TiB", "GiB", "MiB", "KiB", "B" };
static const uint64_t  exbibytes = 1024ULL * 1024ULL * 1024ULL *
	1024ULL * 1024ULL * 1024ULL;
#define DIM(x)  (sizeof(x)/sizeof(*(x)))

char *calculateSize(uint64_t size)
{
	char     *result = (char *)malloc(sizeof(char) * 20);
	uint64_t  multiplier = exbibytes;
	int i;

	for (i = 0; i < DIM(sizes); i++, multiplier /= 1024)
	{
		if (size < multiplier)
			continue;
		if (size % multiplier == 0)
			sprintf(result, "%" PRIu64 " %s", size / multiplier, sizes[i]);
		else
			sprintf(result, "%.1f %s", (float)size / multiplier, sizes[i]);
		return result;
	}
	strcpy(result, "0");
	return result;

}



PRX_EXPORT char *CalcAppsize(char *path)
{

	return calculateSize(0);

	/* FILE *fp = NULL;
	long off;

	fp = fopen(path, "r");
	if (fp == NULL)
	printf("failed to fopen %s\n", path);

	printf("DEBUG: app fd = %s", path);

	if (fseek(fp, 0, SEEK_END) == -1)
	printf("DEBUG: failed to fseek %s\n", path);

	off = ftell(fp);
	if (off == (long)-1)
	printf("DEBUG: failed to ftell %s\n", path);


	printf("[*] fseek_filesize - file: %s, size: %ld\n", path, off);

	if (fclose(fp) != 0)
	printf("DEBUG: failed to fclose %s\n", path);


	if(off)
	return calculateSize(off);
	else
	return NULL;*/

}

PRX_EXPORT char* GetAvailableSpace(const char* path)
{
	//char* Space;

	loadAndImport();
	_statfs bstat;

	//memset(&bstat, 0, sizeof(bstat));
	//SendMessageToPS4("Setting Stat");

	// int statfs(const char *path, struct statfs *buf);
	//fstatfs(int fd, struct statfs *buf);
	int ret = syscall(396,path,&bstat);
	if(ret)
	{
		int err = errno;
		char buffer [150];
		sprintf(buffer,"Error %d",err);
		SendMessageToPS4(buffer);
		return 0;
	}
	SendMessageToPS4("Stat found");
	//char buffer [150];
	//sprintf(buffer,"%d",bstat.f_bsize);
	//SendMessageToPS4(buffer);
	//sprintf(buffer,"%d",bstat.f_bavail);
	//SendMessageToPS4(buffer);

	long available = (bstat.f_bavail * bstat.f_bsize);
	char buffer [15000];

	sprintf(buffer,"%llu",available);
	SendMessageToPS4(buffer);
	//free(Space);
	SendMessageToPS4("Returning Buffer");
	//memcpy(RTNval, available,sizeof(available)+1);
	return buffer;

	//free(&bstat);
	//return &available;
	/*unsigned long blksize, blocks, freeblks, disk_size, used, free;
	blksize = bstat.f_bsize;
	blocks = bstat.f_blocks;
	freeblks = bstat.f_bfree;

	disk_size = blocks*blksize;
	free = freeblks*blksize;
	used = disk_size - free;

	sprintf(buffer,"disk_size %f",disk_size);
	SendMessageToPS4(buffer);
	sprintf(buffer,"free %f",free);
	SendMessageToPS4(buffer);	
	sprintf(buffer,"used %f",used);
	SendMessageToPS4(buffer);

	float freeBytes = bstat.f_bavail * bstat.f_frsize;
	sprintf(buffer,"free %f",freeBytes);
	SendMessageToPS4(buffer);
	return freeBytes;

	return free;*/


	//return (long)(bstat.f_blocks * bstat.f_bavail);




	//return calculate_storage_size();

	//struct stat sb;
	//if((stat(path,&sb))==0)
	//{
	//	//SendMessageToPS4(sb.st_size);
	//	return sb.st_size;
	//}
	return 0;
}

#pragma endregion << Drive Space >>

#pragma region << Mono Stuff >>

//
// this is all thanks to OSM
//
PRX_EXPORT bool Unity_Plugin()
{
	//klog("Hooking Mono");

	//init Mono
	//Mono::Init();//here we should be winning

	//notify("We hooked into UNITY !");

	return true;
}

#pragma endregion << Mono Stuff >>



#pragma region << Sound Stuff >>


PRX_EXPORT void PlaySoundControler(char* Path)
{
	try
	{
		//SetDebuggerTrue();
		PlaySound(Path);
	}
	catch(std::exception ex)
	{
		notify((char*)ex.what());
	}
}


#pragma endregion << Sound Stuff >>