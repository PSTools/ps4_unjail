#pragma once

#include "prerequisites.h"

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SCE_KERNEL_S_IRWXG
#	define SCE_KERNEL_S_IRWXG S_IRWXG
#endif

#define SCE_KERNEL_MAX_MODULES 256
#define SCE_KERNEL_MAX_NAME_LENGTH 256
#define SCE_KERNEL_MAX_SEGMENTS 4
#define SCE_KERNEL_NUM_FINGERPRINT 20

struct _SceKernelModuleSegmentInfo {
	void* baseAddr;
	uint32_t size;
	int32_t prot;
};
typedef struct _SceKernelModuleSegmentInfo SceKernelModuleSegmentInfo;

struct _SceKernelModuleInfo {
	size_t size;
	char name[SCE_KERNEL_MAX_NAME_LENGTH];
	SceKernelModuleSegmentInfo segmentInfo[SCE_KERNEL_MAX_SEGMENTS];
	uint32_t numSegments;
	uint8_t fingerprint[SCE_KERNEL_NUM_FINGERPRINT];
};
typedef struct _SceKernelModuleInfo SceKernelModuleInfo;

struct _SceKernelModuleInfoEx {
	size_t size;
	char name[SCE_KERNEL_MAX_NAME_LENGTH];
	SceKernelModule handle;
	uint32_t tlsIndex;
	uint64_t tlsInitAddr;
	uint32_t tlsInitSize;
	uint32_t tlsSize;
	uint32_t tlsOffset;
	uint32_t tlsAlign;
	uint64_t initProcAddr;
	uint64_t finiProcAddr;
	void* reserved1;
	void* reserved2;
	uint64_t ehFrameHdrAddr;
	uint64_t ehFrameAddr;
	uint32_t ehFrameHdrSize;
	uint32_t ehFrameSize;
	SceKernelModuleSegmentInfo segmentInfo[SCE_KERNEL_MAX_SEGMENTS];
	uint32_t numSegments;
	uint32_t numRefs;
};
typedef struct _SceKernelModuleInfoEx SceKernelModuleInfoEx;

typedef struct
{
	SceSize size;
	char versionString[0x1C];
	SceUInt version;
	SceUInt unk_24;
} SceKernelFwInfo;

int sceKernelGetSystemSwVersion(SceKernelFwInfo * data);

#define SIZEOF_SCE_NOTIFICATION_REQUEST 0xC30

TYPE_BEGIN(struct _SceNotificationRequest, SIZEOF_SCE_NOTIFICATION_REQUEST);
	TYPE_FIELD(int32_t unk_0x10, 0x10);
	TYPE_FIELD(uint8_t buf[0], 0x2C);
TYPE_END();
typedef struct _SceNotificationRequest SceNotificationRequest;

int sceKernelError(int error);

const char* sceKernelGetFsSandboxRandomWord(void);

int DLL_EXPORT sceKernelGetModuleInfo(SceKernelModule handle, SceKernelModuleInfo* info);
int DLL_EXPORT sceKernelGetModuleInfoEx(SceKernelModule handle, SceKernelModuleInfoEx* info);

int DLL_EXPORT sceKernelGetModuleInfoByName(const char* name, SceKernelModuleInfo* info);
int DLL_EXPORT sceKernelGetModuleInfoExByName(const char* name, SceKernelModuleInfoEx* info);

int sceKernelSendNotificationRequest(int device, SceNotificationRequest* req, size_t size, int blocking);

#define	SYS_getdents 272
#define SYS_dynlib_get_info 593
#define SYS_dynlib_get_info_ex 608

int syscall(int num, ...);

#ifdef __cplusplus
}
#endif
