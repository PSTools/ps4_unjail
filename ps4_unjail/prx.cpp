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
#include "types.h""
#include "UserService.h"
#include <fcntl.h>

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

// Var is used for debugging
int DEBUGENABlED = 0;

//Call this funciton if you want to show debug messages everywhere
PRX_EXPORT void SetDebuggerTrue()
{
	DEBUGENABlED =1;
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
	}else if (FWVersion == 755){
		return Sys::kexec((void *)&unjail75X, &td);
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

int (*sceSysUtilSendSystemNotificationWithText)(int messageType, char* message);
int (*customsceSystemServiceLoadExec)(const char* path, char* const argv[]);

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

#pragma endregion <<Notification Class >>

#pragma region << KernelClasss >>

typedef struct Process{
	int process_id;
	char process_name[100];
} Process;

typedef struct {
	uint64_t unk1;
	char version_string[0x1C];
	uint32_t version;
} SceFwInfo;

typedef mode_t SceKernelMode;
int (*sceKernelGetIdPs)(char* ret);
int (*sceKernelGetOpenPsIdForSystem)(void* ret);
int (*sceKernelGetOpenPsId)(void* ret);
//int (*sysctlbyname)(const char *name, void *oldp,size_t *oldlenp,const void *newp, size_t newlen);
//(__int64 a1, __int64 a2, __int64 a3, __int64 a4, __int64 a5)
int (*sysctlbyname)(const char *, void *, size_t *, const void *, size_t);
int (*sysctl)(const int *, u_int, void *, size_t *, const void *, size_t);
//int (*sceKernelGetSystemSwVersion)(char* ret);
int (*sysctlnametomib)(const char *name, int *mibp, size_t *sizep);
// returns sandbox random word - used to load system modules by filepath.
//const char (*sceKernelGetFsSandboxRandomWord)(void *);

char* (*sceKernelGetFsSandboxRandomWord_1)() = NULL;
int (*sceKernelGetSystemSwVersion1)(SceFwInfo* fw_info) = NULL;

/*intsysctl(const int *, u_int, void *, size_t *, const void *, size_t);
int	sysctlbyname(const char *, void *, size_t *, const void *, size_t);
int	sysctlnametomib(const char *, int *, size_t *);*/

//Loads and imports the scekernel functions
void loadAndImport()
{
	int kernel_lib = sceKernelLoadStartModule("/system/common/lib/libkernel.sprx", 0, NULL, 0, NULL, NULL);
	sceKernelDlsym(kernel_lib, "sceKernelGetIdPs", (void **)&sceKernelGetIdPs);//this just returns some fed up info
	sceKernelDlsym(kernel_lib, "sceKernelGetOpenPsIdForSystem", (void **)&sceKernelGetOpenPsIdForSystem);
	sceKernelDlsym(kernel_lib, "sceKernelGetOpenPsId", (void **)&sceKernelGetOpenPsId);
	sceKernelDlsym(kernel_lib, "sysctlbyname", (void **)&sysctlbyname);
	sceKernelDlsym(kernel_lib,"sysctl",(void **)&sysctl);
	sceKernelDlsym(kernel_lib, "sceKernelGetSystemSwVersion",(void **)&sceKernelGetSystemSwVersion1);
	sceKernelDlsym(kernel_lib, "sysctlnametomib",(void **)&sysctlnametomib);
	sceKernelDlsym(kernel_lib, "sceKernelGetFsSandboxRandomWord",(void **)&sceKernelGetFsSandboxRandomWord_1);
}

int loadModule(const char *name, int *idDestination) {
	return syscall(594, name, 0, idDestination, 0);
}

#define RESOLVE(module, name) getFunctionAddressByName(module, #name, &name)

int getFunctionAddressByName(int loadedModuleID, char *name, void *destination);


void loadKernel()
{
	int kernel_lib;
	uint16_t ret;            // Numerical representation of the firmware version. ex: 505 for 5.05, 702 for 7.02, etc
	/*if (loadModule("libkernel.sprx", &kernel_lib)) {
	if (loadModule("libkernel_web.sprx", &kernel_lib)) {
	loadModule("libkernel_sys.sprx", &kernel_lib);
	}
	}*/

	kernel_lib = sceKernelLoadStartModule("libkernel_sys.sprx", 0, 0, 0, 0, 0);


	//RESOLVE(kernel_lib, sceKernelGetFsSandboxRandomWord);

	//sceKernelDlsym(kernel_lib, "sceKernelGetSystemSwVersion",(void **)&sceKernelGetSystemSwVersion);
	ret  = sceKernelDlsym(kernel_lib, "sceKernelGetFsSandboxRandomWord",(void **)&sceKernelGetFsSandboxRandomWord_1);

	if (ret)
	{

		kernel_lib = sceKernelLoadStartModule("libkernel.sprx", 0, 0, 0, 0, 0);
		ret = sceKernelDlsym(kernel_lib, "sceKernelGetFsSandboxRandomWord", (void**)&sceKernelGetFsSandboxRandomWord_1);
		if (ret) 
			return;


	}
}

#pragma endregion << KernelClass >>

#pragma region << User Service Class >>

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

#pragma endregion << User Service Class >>

SceUserServiceUserId userId = SCE_USER_SERVICE_USER_ID_INVALID;

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

#pragma region << Save Data Class>>

typedef struct SceSaveDataMemorySetupResult {
	size_t    existedMemorySize;
	uint8_t   reserved[16];
} SceSaveDataMemorySetupResult;

typedef struct _SceSaveDataInitParams3 SceSaveDataInitParams3;
int (*sceSaveDataInitialize3)(const SceSaveDataInitParams3 *initParam);

typedef struct SceSaveDataTitleId {
	char data[10];				

	char padding[6];											

} SceSaveDataTitleId;

#define SCE_SAVE_DATA_FINGERPRINT_DATA_SIZE			65


typedef struct SceSaveDataFingerprint {
	char data[SCE_SAVE_DATA_FINGERPRINT_DATA_SIZE];
	char padding[15];
} SceSaveDataFingerprint;

typedef uint64_t SceSaveDataBlocks;
typedef uint32_t SceSaveDataMountMode;

#define SCE_SAVE_DATA_DIRNAME_DATA_MAXSIZE				(32)

typedef struct SceSaveDataDirName {
	char data[SCE_SAVE_DATA_DIRNAME_DATA_MAXSIZE];
} SceSaveDataDirName;

typedef struct SceSaveDataTransferringMount {
	SceUserServiceUserId userId;
	const SceSaveDataTitleId *titleId;
	const SceSaveDataDirName *dirName;
	const SceSaveDataFingerprint *fingerprint;
	uint8_t reserved[32];
} SceSaveDataTransferringMount;
typedef uint64_t SceSaveDataBlocks;
typedef uint32_t SceSaveDataMountStatus;
#define SCE_SAVE_DATA_MOUNT_STATUS_CREATED (0x00000001)
#define SCE_SAVE_DATA_MOUNT_POINT_DATA_MAXSIZE			(16)

typedef struct SceSaveDataMountPoint {
	char data[SCE_SAVE_DATA_MOUNT_POINT_DATA_MAXSIZE];
} SceSaveDataMountPoint;

typedef struct SceSaveDataMountResult2 {
	SceSaveDataMountPoint mountPoint;
	SceSaveDataBlocks requiredBlocks;
	uint32_t unused;
	SceSaveDataMountStatus mountStatus;
	uint8_t reserved[28];
	int :32;
} SceSaveDataMountResult2;

typedef struct SceSaveDataMountResult {
	SceSaveDataMountPoint mountPoint;
	SceSaveDataBlocks requiredBlocks;
	uint32_t progress;
	uint8_t reserved[32];
} SceSaveDataMountResult;

typedef struct SceSaveDataMount {
	SceUserServiceUserId userId;
	const SceSaveDataTitleId *titleId;
	const SceSaveDataDirName *dirName;
	const SceSaveDataFingerprint *fingerprint;
	SceSaveDataBlocks blocks;
	SceSaveDataMountMode mountMode;
	uint8_t reserved[32];
} SceSaveDataMount;

typedef struct SceSaveDataMount2 {
	SceUserServiceUserId userId;
	int :32;
	const SceSaveDataDirName *dirName;
	SceSaveDataBlocks blocks;
	SceSaveDataMountMode mountMode;
	uint8_t reserved[32];
	int :32;
} SceSaveDataMount2;

int32_t (*sceSaveDataMount)(
	const SceSaveDataMount *mount,
	SceSaveDataMountResult *mountResult
	);



#define SCE_SAVE_DATA_TITLE_MAXSIZE						(128)
#define SCE_SAVE_DATA_SUBTITLE_MAXSIZE					(128)
#define SCE_SAVE_DATA_DETAIL_MAXSIZE					(1024)

typedef uint32_t SceSaveDataSaveDataMemoryOption;
#define SCE_SAVE_DATA_MEMORY_OPTION_NONE				(0x00000000)

#define SCE_SAVE_DATA_MEMORY_OPTION_SET_PARAM			(0x00000001 << 0)

#define SCE_SAVE_DATA_MEMORY_OPTION_DOUBLE_BUFFER		(0x00000001 << 1)

#define SCE_SAVE_DATA_ICON_WIDTH						(228)		

#define SCE_SAVE_DATA_ICON_HEIGHT						(128)	

#define SCE_SAVE_DATA_ICON_FILE_MAXSIZE					(SCE_SAVE_DATA_ICON_WIDTH * SCE_SAVE_DATA_ICON_HEIGHT * 4)

typedef struct SceSaveDataIcon {
	void *buf;
	size_t bufSize;
	size_t dataSize;
	uint8_t reserved[32];
} SceSaveDataIcon;
typedef struct SceSaveDataParam {
	char title[SCE_SAVE_DATA_TITLE_MAXSIZE];
	char subTitle[SCE_SAVE_DATA_SUBTITLE_MAXSIZE];
	char detail[SCE_SAVE_DATA_DETAIL_MAXSIZE];
	uint32_t userParam;
	int :32;
	time_t mtime;
	uint8_t reserved[32];
} SceSaveDataParam;

typedef uint32_t SceSaveDataSaveDataMemoryOption;

typedef struct SceSaveDataMemorySetup2 {
	SceSaveDataSaveDataMemoryOption option;
	SceUserServiceUserId userId;
	size_t memorySize;
	size_t iconMemorySize;
	const SceSaveDataParam *initParam;
	const SceSaveDataIcon *initIcon;
	uint8_t reserved[24];
} SceSaveDataMemorySetup2;

typedef uint32_t SceSaveDataMountMode;
#define SCE_SAVE_DATA_MOUNT_MODE_RDONLY					(0x00000001)/*J 読み込みモードでマウント */
/*E read only mount */

#define SCE_SAVE_DATA_MOUNT_MODE_RDWR					(0x00000002)/*J 読み書きモードでマウント */
/*E read write mount */
#define SCE_SAVE_DATA_MOUNT_MODE_CREATE					(0x00000004)/*J 新規作成 */
/*E create */
#define SCE_SAVE_DATA_MOUNT_MODE_DESTRUCT_OFF			(0x00000008)/*J 破損化モード OFF */
/*E destruction mode off */
#define SCE_SAVE_DATA_MOUNT_MODE_COPY_ICON				(0x00000010)/*J SCE_SAVE_DATA_MOUNT_MODE_CREATE 設定時 アプリケーションの sce_sys/save_data.png をセーブデータにコピーする */
/*E When setting SCE_SAVE_DATA_MOUNT_MODE_CREATE,
copy sce_sys/save_data.png in the application to the save data. */
#define SCE_SAVE_DATA_MOUNT_MODE_CREATE2				(0x00000020)/*J セーブデータが存在すると読み書きモードでマウント、存在しないと新規作成してから読み書きモードでマウント */
/*E If save data exists, mount in read / write mode.
if it does not exist, mount it in read-write mode after creating a new one */
typedef uint64_t SceSaveDataBlocks;
#define SCE_SAVE_DATA_BLOCK_SIZE						(32768)		/*J ブロックサイズ(byte) */
/*E block size(byte) */
#define SCE_SAVE_DATA_BLOCKS_MIN2						(96)		/*J 最小ブロック数(システム予約ブロック数を含む) */
/*E minimum block num(include system reserved) */
#define SCE_SAVE_DATA_BLOCKS_MAX						(32768)		/*J 最大ブロック数(システム予約ブロック数を含む) */
/*E maximum block num(include system reserved) */			

typedef uint32_t SceSaveDataSortKey;
typedef uint32_t SceSaveDataSortOrder;

typedef struct SceSaveDataDirNameSearchCond {
	SceUserServiceUserId userId;
	int :32;
	const SceSaveDataTitleId *titleId;
	const SceSaveDataDirName *dirName;
	SceSaveDataSortKey key;
	SceSaveDataSortOrder order;
	uint8_t reserved[32];
} SceSaveDataDirNameSearchCond;

typedef uint64_t SceSaveDataBlocks;
typedef struct SceSaveDataSearchInfo {
	SceSaveDataBlocks blocks;
	SceSaveDataBlocks freeBlocks;
	uint8_t reserved[32];
} SceSaveDataSearchInfo;

typedef struct SceSaveDataDirNameSearchResult {
	uint32_t hitNum;
	int :32;
	SceSaveDataDirName *dirNames;
	uint32_t dirNamesNum;
	uint32_t setNum;
	SceSaveDataParam *params;
	SceSaveDataSearchInfo *infos;
	uint8_t reserved[12];
	int :32;
} SceSaveDataDirNameSearchResult;



int32_t (*sceSaveDataDirNameSearch)(const SceSaveDataDirNameSearchCond *cond,SceSaveDataDirNameSearchResult *result);
int32_t (*sceSaveDataTransferringMount)(const SceSaveDataTransferringMount *mount,SceSaveDataMountResult *mountResult);
int32_t (*sceSaveDataUmount)(const SceSaveDataMountPoint *mountPoint);
void initSaveData(void)
{
	int sysSavaData = sceKernelLoadStartModule("/system/common/lib/libSceSaveData.sprx", 0, NULL, 0, 0, 0);
	sceKernelDlsym(sysSavaData, "sceSaveDataInitialize3", (void **)&sceSaveDataInitialize3);
	//sceKernelDlsym(sysSavaData, "sceSaveDataSetupSaveDataMemory2",(void **)&sceSaveDataSetupSaveDataMemory2);
	sceKernelDlsym(sysSavaData, "sceSaveDataMount",(void **)&sceSaveDataMount);
	sceKernelDlsym(sysSavaData, "sceSaveDataDirNameSearch",(void**)&sceSaveDataDirNameSearch);
	sceKernelDlsym(sysSavaData, "sceSaveDataTransferringMount",(void**)&sceSaveDataTransferringMount);
	sceKernelDlsym(sysSavaData, "sceSaveDataUmount",(void**)&sceSaveDataUmount);
}
static inline uint32_t ceil(uint32_t x, uint32_t a)
{
	return (((x) + ((a) - 1)) & ~((a) - 1));
}

#define SCE_SAVE_DATA_ERROR_NO_SPACE_FS			-2137063414	/* 0x809F000A */

#define SCE_SAVE_DATA_SORT_KEY_DIRNAME			0 
#define SAVE_DATA_SORT_KEY_USER_PARAM			1
#define SCE_SAVE_DATA_SORT_KEY_BLOCKS			2
#define SCE_SAVE_DATA_SORT_KEY_MTIME			3
#define SCE_SAVE_DATA_SORT_KEY_FREE_BLOCKS		4

#define SCE_SAVE_DATA_SORT_ORDER_ASCENT			0
#define SCE_SAVE_DATA_SORT_ORDER_DESCENT		1
#define SCE_SAVE_DATA_DIRNAME_MAX_COUNT			1024


#define SCE_SAVE_DATA_ERROR_MOUNT_FULL			-2137063412
#define SCE_SAVE_DATA_ERROR_PARAMETER			-2137063424
int saveDataMount(const SceSaveDataMount *mount, SceSaveDataMountResult *mountResult)
{

	int ret = sceSaveDataMount(mount, mountResult);
	char buffer [150];
	sprintf(buffer,"%d",ret);
	notify(buffer);
	if (ret == SCE_SAVE_DATA_ERROR_NO_SPACE_FS) {
		true;
		mountResult->requiredBlocks;
	} else {
		false;
	}
	return ret;
}

int TotalSavesMounted =0;
SceSaveDataMountPoint SaveDataMountPoints[15];//max save ammount accourding to the dev documentation

int SaveDataTest(char* TITLEID,char* testfingerprint)//CUSA12345
{
	int32_t ret;
	initSysUtil();
	char buffer[180];
	ret =InitlizieUserService();
	if(ret < 0)
	{
		sprintf(buffer, "UserServiceError\n%d", ret);
		notify(buffer);
		//notify("InitlizieUserService error");
		return ret;
	}
	if(userId == SCE_USER_SERVICE_USER_ID_INVALID)
	{
		ret = sceUserServiceGetInitialUser(&userId);
		if(ret < 0)
		{
			notify("sceUserServiceGetInitialUser failed");
			return ret;
		}
	}
	initSaveData();
	ret = sceSaveDataInitialize3(NULL);//call it with a null
	if(ret < 0)
	{
		//error 
		notify("sceSaveDataInitialize3 failed");
		return ret;
	}



	// Obtain all save data directory names under the save data title ID
	if(DEBUGENABlED == 1)	
	{
		notify("Building Save Data Handler");
	}
	SceSaveDataTitleId titleId;
	//SceSaveDataDirName dirName;
	SceSaveDataFingerprint fingerprint;
	memset(&titleId, 0x00, sizeof(titleId));
	strlcpy(titleId.data, TITLEID, sizeof(titleId.data));
	/*memset(&dirName, 0x00, sizeof(dirName));
	strlcpy(dirName.data, "SYSTEM", sizeof(dirName.data));*/
	memset(titleId.padding, 0x00, sizeof(titleId.padding));
	memset(&fingerprint, 0x00, sizeof(fingerprint));
	//294a5ed06db170618f2eed8c424b9d828879c080cc66fbc4864f69e974deb856
	strlcpy(fingerprint.data, testfingerprint, sizeof(fingerprint.data));


	SceSaveDataDirNameSearchCond cond;
	memset(&cond, 0x00, sizeof(SceSaveDataDirNameSearchCond));
	cond.userId = userId;
	//cond.dirName = NULL;
	//cond.key = SCE_SAVE_DATA_SORT_KEY_DIRNAME;
	//cond.order = SCE_SAVE_DATA_SORT_ORDER_ASCENT;
	cond.titleId = &titleId;

	SceSaveDataDirNameSearchResult result;
	memset(&result, 0x00, sizeof(result));

	result.dirNames = new SceSaveDataDirName[SCE_SAVE_DATA_DIRNAME_MAX_COUNT];
	// SCE_SAVE_DATA_DIRNAME_MAX_COUNT can be the maximum number of save data directories the application will create
	if ( result.dirNames == NULL ) 
	{
		// Error handling
	}
	result.dirNamesNum = SCE_SAVE_DATA_DIRNAME_MAX_COUNT;

	if ( sceSaveDataDirNameSearch(&cond, &result) < 0 ) 
	{
		// Error handling
		delete [] result.dirNames;
		result.dirNames = NULL;
		notify("Name Search Failed");
		ret -1;
	}
	buffer[1024];
	if(DEBUGENABlED == 1)	
	{
		for (int i = 0; i < result.hitNum; i++)
		{   
			sprintf(buffer, "SaveDir %s", result.dirNames[i].data);//dont want all those enters
			notify(buffer);
		}
	}

	/*Set the save data mount points for unmounting later*/

	SaveDataMountPoints[result.hitNum];
	TotalSavesMounted = result.hitNum;
	for (int i = 0; i < result.hitNum; i++)
	{   
		SceSaveDataDirName dirName;
		strlcpy(dirName.data,result.dirNames[i].data, sizeof(dirName.data));
		SceSaveDataMount mount;
		memset(&mount, 0x00, sizeof (mount));

		mount.userId = userId;
		mount.dirName = &dirName;
		mount.fingerprint = &fingerprint;
		mount.titleId = &titleId;
		mount.blocks = 32768;
		mount.mountMode = SCE_SAVE_DATA_MOUNT_MODE_RDONLY ;//(SCE_SAVE_DATA_MOUNT_MODE_DESTRUCT_OFF | SCE_SAVE_DATA_MOUNT_MODE_RDWR);

		SceSaveDataMountResult mountResult;
		memset(&mountResult, 0x00, sizeof(mountResult));

		ret = sceSaveDataMount(&mount, &mountResult);
		if (ret < 0) {
			if(DEBUGENABlED == 1)	
			{
				sprintf(buffer, "saveDataMount failed\n\n0x%08x", ret);
				//we dont want to display the error
				notify(buffer);
			}
		}
		SaveDataMountPoints[i] = mountResult.mountPoint;//set each mount points value

		//free memory
		//free(&dirName);
		/*if(DEBUGENABlED == 1)	
		{
		notify("Memory Freed");
		}*/
	}

	return ret;
}


#pragma endregion << Save Data Class >>

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


PRX_EXPORT int UnMountSaveData()
{
	int ret = 0;
	initSysUtil();
	initSaveData();//save data
	for (int i = 0; i < TotalSavesMounted; i++)
	{
		SceSaveDataMountPoint mountPoint ;
		mountPoint = SaveDataMountPoints[i];

		if (sceSaveDataUmount(&mountPoint) < 0 ) {
			// Error handling
			notify("Error Unmounting Save Data");
		}
	}
	return ret;
}

#pragma endregion << Save Data >>

#pragma region << Kernel Calls >>

PRX_EXPORT bool LoadExec(const char *path, char *const *argv)
{
	initSysUtil();

	if (path == nullptr) return false;
	if (path[0] == '\0') return false;
	printf("hello from xdpx");
	int ret = 1;
	ret = customsceSystemServiceLoadExec(path, NULL);
	if (ret == SCE_OK) return true;
	return false;
}

PRX_EXPORT const char* GetCallableList()
{
	initSysUtil();

	//int sceKernelGetModuleList(SceKernelModule *array, size_t numArray, size_t *actualNum);
	int kernel_lib = sceKernelLoadStartModule("/system/common/lib/libkernel.sprx", 0, NULL, 0, NULL, NULL);
	size_t moduleCount;
	SceKernelModule modules [1];
	modules[0] = kernel_lib;



	sceKernelGetModuleList(modules,ModulesSize,&moduleCount);
	//now our code should give us the total modules

	//char mess [180];
	//sprintf(mess,"<Module List (%d modules)>\n", moduleCount);
	//notify(mess);
	//for(int ix = 0; ix < moduleCount; ix++)
	//{
	//	int ret, opt = sizeof(opt), reload_mod;
	//	SceKernelModuleInfoEx info = {0};
	//	info.size = sizeof(info);
	//	if ((ret = sceKernelGetModuleInfoEx(kernel_lib, &info)) < 0) {
	//		sprintf(mess,"Failed to get module info for UID %08x, ret %08x\n", kernel_lib, ret);
	//		notify(mess);
	//		return mess;
	//	}
	//	//else we have info lets display them
	//	for (int i = 0; i < info.numSegments; i++)
	//	{
	//		sprintf(mess,"Module Info 0x%08x_%s_%d",info.segmentInfo[i].baseAddr,info.name,i );
	//		notify(mess);
	//	}

	//	//print_modinfo(ids[i], verbose);
	//}
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
	char* IDPS;

	int ret = -1;

	loadAndImport();

	initSysUtil();
	char* idps = (char *)malloc(64);//dont knwo why its being a little bitch
	memset(idps, 0, 64);
	ret = sceKernelGetIdPs(idps);
	if(ret < 0)
	{
		notify("SCE Kernel Get IDPS Failed");
	}

	//small test 
	char* idpsholder = (char *)malloc(64);
	memset(idpsholder,0,64);
	size_t bufferlen  = 64;
	ret = sysctlbyname("machdep.idps",idpsholder,&bufferlen, NULL, NULL);//just to bypass the ps4 control namne
	if(ret < 0)
	{
		notify("sysctl IDPS Failed");
		return IDPS;//all wne to hell 
	}	

	char idps_buf[255];
	for (int i = 0; i<64; i++) {
		sprintf(idps_buf+i, "%x", idps[i]);
	}

	char idps_buf2[255];
	for (int i = 0; i<64; i++) {
		sprintf(idps_buf2+i, "%x", idpsholder[i]);
	}

	char buffer[1024];
	sprintf(buffer, "IDPS: %s", idps_buf);
	sprintf(buffer, "%s", idps_buf2);
	return buffer;
}

PRX_EXPORT const char* GetPSID()
{
	char* PSID;
	loadAndImport();	
	initSysUtil();
	int ret = -1;
	char* psid = (char *)malloc(16);
	memset(psid, 0, 16);
	ret = sceKernelGetOpenPsIdForSystem(psid);
	if(ret < 0)
	{
		notify("SCE Kernel Get PSID Failed");
		return PSID;
	}
	char psid_buf[255];
	for (int i = 0; i<16; i++) {
		sprintf(psid_buf+i, "%x", psid[i]);
	}
	char buffer[1024];
	sprintf(buffer, "%s", psid_buf);
	PSID = buffer;

	return buffer;
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
	//this one migt be different ? not sure why there is two 
	char* PSID;
	loadAndImport();	
	initSysUtil();
	int ret = -1;
	char* psid = (char *)malloc(250);
	memset(psid, 0, 250);
	ret =sceKernelGetOpenPsId(psid);
	if(ret < 0)
	{
		notify("SCE Kernel Get PSID Failed");
		return PSID;
	}

	char psid_buf[255];
	for (int i = 0; i<250; i++) {
		sprintf(psid_buf+i, "%x", psid[i]);
	}
	char buffer[1024];
	sprintf(buffer, "%s", psid_buf);
	notify(buffer);
	return buffer;
}

struct firmware {
	uint32_t build : 12;
	uint32_t minor : 12;
	uint32_t major :  8;
};
struct firmware version;
//gets the version as an int
PRX_EXPORT int get_fw()
{
	size_t fwlen = sizeof(struct firmware);
	if (sysctlbyname("kern.sdk_version", (char *)&version, &fwlen, NULL, 0) == 0) {
		return version.major * 0x100 + ((version.minor & 0xFF0) / 0x10);
	}

	return -1;
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
	SceFwInfo fw_info;
	sceKernelGetSystemSwVersion1(&fw_info);
	static char version[28];
	snprintf(version, 0x1C, "%s", fw_info.version_string);
	return version;
}

uint16_t g_firmware = 0;
PRX_EXPORT uint16_t get_firmware() {
	if (g_firmware) {
		return g_firmware;
	}
	uint16_t ret;            // Numerical representation of the firmware version. ex: 505 for 5.05, 702 for 7.02, etc
	uint32_t offset;         // Offset for ealier firmware's version location
	char binary_fw[2] = {0}; // 0x0000
	char string_fw[5] = {0}; // "0000\0"
	char sandbox_path[33];   // `/XXXXXXXXXX/common/lib/libc.sprx` [Char count of 32 + nullterm]
	int sys;


	sys = sceKernelLoadStartModule("libkernel_sys.sprx", 0, 0, 0, 0, 0);
	ret = sceKernelDlsym(sys, "sceKernelGetFsSandboxRandomWord", (void**)&sceKernelGetFsSandboxRandomWord_1);
	if (ret)
	{

		sys = sceKernelLoadStartModule("libkernel.sprx", 0, 0, 0, 0, 0);
		ret = sceKernelDlsym(sys, "sceKernelGetFsSandboxRandomWord", (void**)&sceKernelGetFsSandboxRandomWord_1);
		if (ret) return -2;

	}



	snprintf(sandbox_path, sizeof(sandbox_path), "/%s/common/lib/libc.sprx", sceKernelGetFsSandboxRandomWord_1());

	int fd = open(sandbox_path, O_RDONLY, 0);
	if (fd < 0) {
		// Assume it's currently jailbroken
		fd = open("/system/common/lib/libc.sprx", O_RDONLY, 0);
		if (fd < 0) {
			// It's really broken
			return -1;
		}
	}

	lseek(fd, 0x240, SEEK_SET); // 0x240 for 1.01 -> ?.??, 0x2B0 for ?.?? (5.05) -> ???
	read(fd, &offset, sizeof(offset));

	if (offset == 0x50E57464) { // "Påtd"
		lseek(fd, 0x334, SEEK_SET);
	} else {
		lseek(fd, 0x374, SEEK_SET);
	}

	read(fd, &binary_fw,  sizeof(binary_fw));
	close(fd);

	snprintf_s(string_fw, sizeof(string_fw), "%02x%02x", binary_fw[1], binary_fw[0]);

	ret = atoi(string_fw);

	g_firmware = ret;
	return ret;
}

//Get the firmware version by libc (for prevent from kernel spoof)
//Should no longer be required thanks to LM
PRX_EXPORT char* firmware_version_libc()
{
	loadAndImport();
	initSysUtil();
	//loadKernel();
	//notify("User Function Called");
	char version[5] = {0};
	static char* sandboxWord = NULL;
	char fw[2] = {0};
	//notify("Setting Sanbox Path");
	static char sandbox_path[50];
	sandboxWord = sceKernelGetFsSandboxRandomWord_1();
	char buffer[520];
	sprintf(buffer, "Sandbox Word is \n%s", sandboxWord);
	//notify(buffer);
	snprintf(sandbox_path, 50, "/%s/system/common/lib/libc.sprx", sandboxWord);
	//notify(sandbox_path);
	int fd = sceKernelOpen(sandbox_path, 0, 0);
	if (!fd)
		//notify("Opening LibC");
			fd = sceKernelOpen("/system/common/lib/libc.sprx", 0, 0);

	if (fd) {
		//notify("SEEK");
		sceKernelLseek(fd, 0x374, SEEK_CUR);
		sceKernelRead(fd, &fw, 2);
		sceKernelClose(fd);
		//notify("Version");
		snprintf_s(version,sizeof(version), "%02x%02x", fw[1], fw[0]);
		sprintf(buffer, "Version is \n%s", version);
		//notify(buffer);

		char ch[5];
		(void)memset(ch, 0, sizeof(ch));
		//notify("char copied");
		char * a = new char[1024];
		strcpy(a, version);
		return (a);
		return 0;
	} else {
		return "";
	}
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



#pragma endregion << Kernel Calls >>

#pragma region << User Service Functions >>

PRX_EXPORT char* GetUsername()
{
	char* rtnchar;
	//memset(rtnchar,0,sizeof(rtnchar));
	int ret = -1;
	if(sceSysUtilSendSystemNotificationWithText == nullptr)
	{
		//initilize util
		initSysUtil();
	}
	char username[32];
	//memset(username, 0, 32);
	//memset(username, 0, 32);
	char buffer[520];
	if(DEBUGENABlED == 1)
	{
		notify("Ilitilize User Service");
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

PRX_EXPORT const char* GetUserId()
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
		ret = sceUserServiceGetInitialUser(&userId);
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
		ret = sceUserServiceGetInitialUser(&userId);
		if(ret < 0)
		{
			notify("sceUserServiceGetInitialUser failed");
			return ret;
		}
	}
	(void)memset(username, 0, sizeof(username));

	if (sceUserServiceGetUserName(userId, username, sizeof(username) - 1) < 0)
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
PRX_EXPORT int MountandLoad()
{
	//even if it just mounts im going to be flippen happy

	loadAndImport();
	initSysUtil();
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

		ret = sceUserServiceGetInitialUser(&userId);
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
int(*sceBgftInitialize)(struct bgft_init_params*);
int(*sceBgftDownloadRegisterTaskByStorage)(struct bgft_download_param*, int*);
int(*sceBgftDownloadStartTask)(int);
int bgft;
PRX_EXPORT int InstallPKG(char * path,char * name,char * imgpath)
{
	int rv;
	//int sceScreenshotHandle = sceKernelLoadStartModule("/system/common/lib/libSceScreenShot.sprx", 0, NULL, 0, 0, 0);
	//sceKernelDlsym(sceScreenshotHandle,"sceScreenShotSetParam",(void **)&sceScreenShotSetParam);  
	bgft = sceKernelLoadStartModule("/system/common/lib/libSceBgft.sprx", 0,NULL,0,0,0);

	sceKernelDlsym(bgft, "sceBgftServiceIntInit",(void **)&sceBgftInitialize);

	sceKernelDlsym(bgft, "sceBgftServiceIntDownloadRegisterTaskByStorage",(void **)&sceBgftDownloadRegisterTaskByStorage);

	sceKernelDlsym(bgft, "sceBgftServiceIntDownloadStartTask",(void **)&sceBgftDownloadStartTask);
	struct bgft_init_params ip = {
		.mem = mmap(NULL, 0x100000, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0),
		.size = 0x100000,
	};
	rv = sceBgftInitialize(&ip);
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
	rv = sceBgftDownloadRegisterTaskByStorage(&params, &task);
	rv = sceBgftDownloadStartTask(task);
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