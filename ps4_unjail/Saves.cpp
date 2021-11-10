#pragma once

#include "SysUtil.h"
#include <_kernel.h>
#include <stdio.h>
#include "Saves.h"
#include "UserService.h"

int32_t (*sceSaveDataMount)(
	const SceSaveDataMount *mount,
	SceSaveDataMountResult *mountResult
	);

int32_t (*sceSaveDataMount2)(
	const SceSaveDataMount2 *mount,
	SceSaveDataMountResult *mountResult
	);

int32_t (*sceSaveDataDelete)(const SceSaveDataDelete *del);

int32_t (*sceSaveDataDirNameSearch)(const SceSaveDataDirNameSearchCond *cond,SceSaveDataDirNameSearchResult *result);
int32_t (*sceSaveDataTransferringMount)(const SceSaveDataTransferringMount *mount,SceSaveDataMountResult *mountResult);
int32_t (*sceSaveDataUmount)(const SceSaveDataMountPoint *mountPoint);
int (*sceSaveDataInitialize3)(const SceSaveDataInitParams3 *initParam);

int (*sceSaveDataInitialize2)(const SceSaveDataInitParams3 *initParam);

int (*sceSaveDataInitialize5)(const SceSaveDataInitParams3 *initParam);

int32_t (*sceSaveDataDeleteAllUser)();


int TotalSavesMounted =0;
SceSaveDataMountPoint SaveDataMountPoints[15];//max save ammount accourding to the dev documentation

void initSaveData(void)
{
	int sysSavaData = sceKernelLoadStartModule("/system/common/lib/libSceSaveData.sprx", 0, NULL, 0, 0, 0);
	sceKernelDlsym(sysSavaData, "sceSaveDataInitialize3", (void **)&sceSaveDataInitialize3);
	sceKernelDlsym(sysSavaData,"sceSaveDataInitialize2",(void **)&sceSaveDataInitialize2);
	//sceKernelDlsym(sysSavaData, "sceSaveDataSetupSaveDataMemory2",(void **)&sceSaveDataSetupSaveDataMemory2);
	sceKernelDlsym(sysSavaData, "sceSaveDataMount",(void **)&sceSaveDataMount);
	sceKernelDlsym(sysSavaData, "sceSaveDataMount2",(void **)&sceSaveDataMount2);
	sceKernelDlsym(sysSavaData, "sceSaveDataDirNameSearch",(void**)&sceSaveDataDirNameSearch);
	sceKernelDlsym(sysSavaData, "sceSaveDataTransferringMount",(void**)&sceSaveDataTransferringMount);
	sceKernelDlsym(sysSavaData, "sceSaveDataUmount",(void**)&sceSaveDataUmount);
	sceKernelDlsym(sysSavaData,"sceSaveDataDelete",(void **)&sceSaveDataDelete);
	sceKernelDlsym(sysSavaData,"sceSaveDataDeleteAllUser",(void **)&sceSaveDataDeleteAllUser);
}
static inline uint32_t ceil(uint32_t x, uint32_t a)
{
	return (((x) + ((a) - 1)) & ~((a) - 1));
}

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



int SaveDataUnmount(const SceSaveDataMountPoint *mountPoint)
{
	return sceSaveDataUmount(mountPoint);
}

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

int SaveDataTest(char* TITLEID,char* DataDir,char* testfingerprint)//CUSA12345
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
		ret = GetInitialUser();
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

	SaveDataMountPoints[1];
	TotalSavesMounted = 1;
	//for (int i = 0; i < 1; i++)
	{   
		SceSaveDataDirName dirName;
		strlcpy(dirName.data,DataDir, sizeof(dirName.data));
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
		SaveDataMountPoints[0] = mountResult.mountPoint;//set each mount points value

		//free memory
		//free(&dirName);
		/*if(DEBUGENABlED == 1)	
		{
		notify("Memory Freed");
		}*/
	}

	return ret;
}

int SaveDataTestMount2(char* TITLEID,char* DataDir)
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
		ret = GetInitialUser();
		if(ret < 0)
		{
			notify("sceUserServiceGetInitialUser failed");
			return ret;
		}
	}
	initSaveData();
	ret = sceSaveDataInitialize2(NULL);//call it with a null
	if(ret < 0)
	{
		//error 
		notify("sceSaveDataInitialize2 failed");
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
	//strlcpy(fingerprint.data, testfingerprint, sizeof(fingerprint.data));


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

	SaveDataMountPoints[1];
	TotalSavesMounted = 1;
	//for (int i = 0; i < 1; i++)
	{   
		SceSaveDataDirName dirName;
		strlcpy(dirName.data,DataDir, sizeof(dirName.data));
		SceSaveDataMount2 mount;
		memset(&mount, 0x00, sizeof (mount));

		mount.userId = userId;
		mount.dirName = &dirName;
		mount.blocks = 32768;
		mount.mountMode = SCE_SAVE_DATA_MOUNT_MODE_RDONLY ;//(SCE_SAVE_DATA_MOUNT_MODE_DESTRUCT_OFF | SCE_SAVE_DATA_MOUNT_MODE_RDWR);

		SceSaveDataMountResult mountResult;
		memset(&mountResult, 0x00, sizeof(mountResult));

		ret = sceSaveDataMount2(&mount, &mountResult);
		if (ret < 0) {
			if(DEBUGENABlED == 1)	
			{
				sprintf(buffer, "saveDataMount2 failed\n\n0x%08x", ret);
				//we dont want to display the error
				notify(buffer);
			}
		}
		SaveDataMountPoints[0] = mountResult.mountPoint;//set each mount points value
		notify(mountResult.mountPoint.data);
		//free memory
		//free(&dirName);
		/*if(DEBUGENABlED == 1)	
		{
		notify("Memory Freed");
		}*/
	}

	return ret;
}

int SaveDataDelete(char* DataDir)
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
		ret = GetInitialUser();
		if(ret < 0)
		{
			notify("sceUserServiceGetInitialUser failed");
			return ret;
		}
	}


	SceSaveDataDirName dirName;

	strlcpy(dirName.data, DataDir, sizeof(dirName.data));

	SceSaveDataDelete del;
	memset(&del, 0x00, sizeof(del));
	del.userId = userId;
	del.dirName = &dirName;

	ret = sceSaveDataDelete(&del);
	if(ret != 0)
	{
		// Error handling
		notify("Error Deleting Save");
	}
	else
	{
		notify("Save Deleted");
	}
}

int SaveDataDeleteGame(char* TitleId)
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
	//SceSaveDataFingerprint fingerprint;
	memset(&titleId, 0x00, sizeof(titleId));
	strlcpy(titleId.data, TitleId, sizeof(titleId.data));
	/*memset(&dirName, 0x00, sizeof(dirName));
	strlcpy(dirName.data, "SYSTEM", sizeof(dirName.data));*/
	memset(titleId.padding, 0x00, sizeof(titleId.padding));
	//memset(&fingerprint, 0x00, sizeof(fingerprint));
	//294a5ed06db170618f2eed8c424b9d828879c080cc66fbc4864f69e974deb856
	//strlcpy(fingerprint.data, testfingerprint, sizeof(fingerprint.data));


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
	//TotalSavesMounted = result.hitNum;
	for (int i = 0; i < result.hitNum; i++)
	{   
		SceSaveDataDirName dirName;
		strlcpy(dirName.data,result.dirNames[i].data, sizeof(dirName.data));

		SceSaveDataDelete del;
		memset(&del, 0x00, sizeof(del));
		del.userId = userId;
		del.dirName = &dirName;

		ret = sceSaveDataDelete(&del);
		if(ret != 0)
		{
			
			char result[255];   // array to hold the result.

			strcpy(result,"Error Deleting Save\n"); // copy string one into the result.
			strcat(result,dirName.data); // append string two to the result.

			// Error handling
			notify((result));
		}
		else
		{
			notify("Save Deleted");
		}

	}

	return ret;
}

int SaveDataDeleteAllForUser()
{
	
}
