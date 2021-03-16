#pragma once

#include "prerequisites.h"


#ifdef __cplusplus
extern "C" {
#endif

	struct SceUsbStorageDeviceInfo
	{
		uint64_t totalSpace;
		uint64_t availableSpace;
		uint16_t vendorId;
		uint16_t productId;
		uint16_t bcdDevice;
		char manufacturer[256];
		uint64_t manufacturerLength;
		char product[256];
		uint64_t productLength;
		char serialNumber[256];
		uint64_t serialNumberLength;
		uint64_t additionalFeatures;
		uint64_t capacity;
	};


	// int32_t sceUsbStorageIsExist(uint32_t, const char*);
	// int32_t sceUsbStorageRegisterCallback(uint32_t, ...);
	// int32_t sceUsbStorageUnregisterCallback(uint32_t, ...);

	int32_t sceUsbStorageGetDeviceInfo(uint32_t, SceUsbStorageDeviceInfo *);
	int32_t sceUsbStorageGetDeviceList(uint32_t*, int32_t*);
	int32_t sceUsbStorageInit(ScePthreadAttr *);
	int32_t sceUsbStorageRequestMap(uint32_t, const char *, int32_t, uint64_t, char *, uint64_t *, const void *, size_t);
	int32_t sceUsbStorageRequestMapWSB(uint32_t, const char *, int32_t, uint64_t, char *, uint64_t *, const void *, size_t);
	int32_t sceUsbStorageRequestUnmap(uint32_t, const char *, const void *, size_t);
	int32_t sceUsbStorageTerm();

#ifdef __cplusplus
}
#endif
