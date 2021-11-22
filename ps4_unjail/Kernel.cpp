

#include "SysUtil.h"
#include "Kernel.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <_fs.h>
#include <unistd.h>



int (*sysctlbyname)(const char *, void *, size_t *, const void *, size_t);

int (*sceKernelGetIdPs)(char* ret);
int (*sceKernelGetOpenPsIdForSystem)(void* ret);
int (*sceKernelGetOpenPsId)(void* ret);

//int (*sysctlbyname)(const char *name, void *oldp,size_t *oldlenp,const void *newp, size_t newlen);
//(__int64 a1, __int64 a2, __int64 a3, __int64 a4, __int64 a5)
//int (*sysctlbyname)(const char *, void *, size_t *, const void *, size_t);
int (*sysctl)(const int *, u_int, void *, size_t *, const void *, size_t);
//int (*sceKernelGetSystemSwVersion)(char* ret);
int (*sysctlnametomib)(const char *name, int *mibp, size_t *sizep);
// returns sandbox random word - used to load system modules by filepath.
//const char (*sceKernelGetFsSandboxRandomWord)(void *);

char* (*sceKernelGetFsSandboxRandomWord_1)() = NULL;
int (*sceKernelGetSystemSwVersion1)(SceFwInfo* fw_info) = NULL;

//Loads and imports the scekernel functions
void loadAndImport()
{
	int kernel_lib = sceKernelLoadStartModule("/system/common/lib/libkernel.sprx", 0, NULL, 0, NULL, NULL);
	sceKernelDlsym(kernel_lib, "sceKernelGetIdPs", (void **)&sceKernelGetIdPs);//this just returns some fed up info
	sceKernelDlsym(kernel_lib, "sceKernelGetOpenPsIdForSystem", (void **)&sceKernelGetOpenPsIdForSystem);
	sceKernelDlsym(kernel_lib, "sceKernelGetOpenPsId", (void **)&sceKernelGetOpenPsId);
	sceKernelDlsym(kernel_lib, "sysctlbyname", (void **)&sysctlbyname);
	sceKernelDlsym(kernel_lib,"sysctl",(void **)&sysctl);
	//sceKernelDlsym(kernel_lib,"_fstat",(void **)&statfs);
	//sceKernelDlsym(kernel_lib,"fstatfs",(void **)&fstatfs);
	sceKernelDlsym(kernel_lib, "sceKernelGetSystemSwVersion",(void **)&sceKernelGetSystemSwVersion1);
	sceKernelDlsym(kernel_lib, "sysctlnametomib",(void **)&sysctlnametomib);
	sceKernelDlsym(kernel_lib, "sceKernelGetFsSandboxRandomWord",(void **)&sceKernelGetFsSandboxRandomWord_1);
}

int loadModule(const char *name, int *idDestination) {
	return syscall(594, name, 0, idDestination, 0);
}



char* GetAllSysCtl()
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


char* KernelGetIDPS()
{
	//notify("IDPS");
	char* IDPS;

	int ret = -1;
	//notify("IDPS 1");
	loadAndImport();
	//notify("IDPS 2");
	initSysUtil();
	//notify("IDPS 3");
	char* idps = (char *)malloc(64);//dont knwo why its being a little bitch
	memset(idps, 0, 64);
	//notify("IDP4");
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
	//notify(buffer);
	return buffer;
}

const char* KernelGetPSID()
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
	//notify(buffer);
	return buffer;
}

char* Kernel_firmware_version_libc()
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

uint16_t g_firmware = 0;

uint16_t Kernel_Get_FW_Version()
{
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


const char* Kernel_Firmware_Version()
{
	SceFwInfo fw_info;
	sceKernelGetSystemSwVersion1(&fw_info);
	static char version[28];
	snprintf(version, 0x1C, "%s", fw_info.version_string);
	return version;
}


char* Kernel_GetSandboxPath()
{
	loadAndImport();

	static char* sandboxWord = NULL;
	sandboxWord = sceKernelGetFsSandboxRandomWord_1();
	return sandboxWord;
}


const char* KernelGetOpenPSID()
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

int Kernel_Get_Fw()
{
	size_t fwlen = sizeof(struct firmware);
	if (sysctlbyname("kern.sdk_version", (char *)&version, &fwlen, NULL, 0) == 0) {
		return version.major * 0x100 + ((version.minor & 0xFF0) / 0x10);
	}

	return -1;
}
