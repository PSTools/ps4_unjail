#include "pluginunjail.h"

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
#include <sysmodule_ex.h>
#include <usb_storage.h>
#include <string>
#include <sys/time.h>
#include "kernel_ex.h"
#include "types.h""


extern "C" {
#include "unjail.h"
#include "ftps4.h"
#include "mount.h"

}

int FreeFTP()
{
	char ip_address[SCE_NET_CTL_IPV4_ADDR_STR_LEN];
	get_ip_address(ip_address);
	ftps4_init(ip_address, 21);
	return 0;
}

struct tm * netGetTimeWide(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    time_t timedate = tv.tv_sec;
    struct tm * timestring = localtime(&timedate);
    return timestring;
}

int FreeUnjail(int FWVersion)
{
	struct thread1 td;
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
	}else if (FWVersion == 750){
		return Sys::kexec((void *)&unjail750, &td);
	}else if (FWVersion == 751){
		return Sys::kexec((void *)&unjail751, &td);
	}else if (FWVersion == 755){
		return Sys::kexec((void *)&unjail755, &td);
	}

	return 0;
}

#define SCE_SYSMODULE_USB_STORAGE 0xD5

int FreeMountUsb(int offset)
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

long unsigned int  Temperature() {
	return Temp();
}

void  SetTemperature(long unsigned int celsius) {
	SetTemp(celsius);
}

int PupDecrypt(const char* path){
	int res = DecryptPup(path);
	return res;
}

uint32_t FW()
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



int FreeMountUsbMuilti(){
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

int FreeMount()
{
	mount_large_fs("/dev/da0x0.crypt", "/preinst", "exfatfs", "511", MNT_UPDATE);
	mount_large_fs("/dev/da0x1.crypt", "/preinst2", "exfatfs", "511", MNT_UPDATE);
	mount_large_fs("/dev/da0x4.crypt", "/system", "exfatfs", "511", MNT_UPDATE);
	mount_large_fs("/dev/da0x5.crypt", "/system_ex", "exfatfs", "511", MNT_UPDATE);

	return 0;
}