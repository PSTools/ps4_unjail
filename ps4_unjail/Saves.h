#ifndef _SAVES_H
#define _SAVES_H

#include "UserService.h"


typedef struct SceSaveDataMemorySetupResult {
	size_t    existedMemorySize;
	uint8_t   reserved[16];
} SceSaveDataMemorySetupResult;

typedef struct _SceSaveDataInitParams3 SceSaveDataInitParams3;

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


typedef struct SceSaveDataDelete {
	SceUserServiceUserId userId;
	int :32;
	const SceSaveDataTitleId *titleId;
	const SceSaveDataDirName *dirName;
	uint32_t unused;
	uint8_t reserved[32];
	int :32;
} SceSaveDataDelete;

extern int TotalSavesMounted ;
extern SceSaveDataMountPoint SaveDataMountPoints[];//max save ammount accourding to the dev documentation

//
//int TotalSavesMounted;
//SceSaveDataMountPoint SaveDataMountPoints[15];//max save ammount accourding to the dev documentation

void initSaveData(void);
static inline uint32_t ceil(uint32_t x, uint32_t a);
int SaveDataUnmount(const SceSaveDataMountPoint *mountPoint);
int saveDataMount(const SceSaveDataMount *mount, SceSaveDataMountResult *mountResult);
int SaveDataTest(char* TITLEID,char* testfingerprint);
int SaveDataTest(char* TITLEID,char* DataDir,char* testfingerprint);
int SaveDataTestMount2(char* TITLEID,char* DataDir);
int SaveDataDelete(char* DataDir);
int SaveDataDeleteGame(char* TitleId);
int SaveDataDeleteAllForUser();


#endif