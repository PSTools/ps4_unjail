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
	if (FWVersion == 405){
		return Sys::kexec((void *)&unjail405, &td);
	}
	else if (FWVersion == 455){
		return Sys::kexec((void *)&unjail455, &td);
	}
	else if (FWVersion == 474){
		return Sys::kexec((void *)&unjail474, &td);
	}
	else if (FWVersion == 501){
		return Sys::kexec((void *)&unjail501, &td);
	}
	else if (FWVersion == 505){
		return Sys::kexec((void *)&unjail505, &td);
	}
	else if (FWVersion == 672){
		return Sys::kexec((void *)&unjail672, &td);
	}
	else if (FWVersion == 702){
		return Sys::kexec((void *)&unjail702, &td);
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
