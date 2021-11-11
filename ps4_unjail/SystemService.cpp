#pragma once

#include <_kernel.h>
#include "SystemService.h"
#include "SysUtil.h"
#include "UserService.h"
//#include <lnc_util.h>

/*some fixes from LM*/


int (*sceSystemServiceLaunchApp)(const char* titleId, const char* argv[], LaunchAppParam* param);
int (*sceLncUtilStartLaunchAppByTitleId)(const char* titleId, const char* argv[], LaunchAppParam* param);
int (*sceLncUtilStartLaunchApp)(const char* titleId, const char* argv[], LaunchAppParam* param);

int (*sceLncUtilInitialize)();



bool SystemServiceLaunchApp(char * titleId)
{
	/*
	public enum AppAttr
	{
	// Token: 0x040011A5 RID: 4517
	None = 0,
	// Token: 0x040011A6 RID: 4518
	DisableSystemBG = 1,
	// Token: 0x040011A7 RID: 4519
	LaunchByDebugger = 2,
	// Token: 0x040011A8 RID: 4520
	LaunchByAppHomeData = 4,
	// Token: 0x040011A9 RID: 4521
	LaunchByAppHomeHost = 8
	}*/


	/*
	public enum AppType
	{
	// Token: 0x0400119C RID: 4508
	Unknown,
	// Token: 0x0400119D RID: 4509
	ShellUI,
	// Token: 0x0400119E RID: 4510
	Daemon,
	// Token: 0x0400119F RID: 4511
	CDLG,
	// Token: 0x040011A0 RID: 4512
	MiniApp,
	// Token: 0x040011A1 RID: 4513
	BigApp,
	// Token: 0x040011A2 RID: 4514
	ShellCore,
	// Token: 0x040011A3 RID: 4515
	ShellApp
	}
	*/

	/*public enum Flag : ulong
	{
	// Token: 0x04001160 RID: 4448
	None = 0UL,
	// Token: 0x04001161 RID: 4449
	SkipLaunchCheck = 1UL,
	// Token: 0x04001162 RID: 4450
	SkipResumeCheck = 1UL,
	// Token: 0x04001163 RID: 4451
	SkipSystemUpdateCheck = 2UL
	}*/

	initSysUtil();
	//notify("LaunchApp");
	int serres = sceKernelLoadStartModule("/system/common/lib/libSceSystemService.sprx", 0, NULL, 0, 0, 0);

	if (!IS_ERROR(serres))
	{
		//notify("Found");
		serres =sceKernelDlsym(serres, "sceLncUtilLaunchApp", (void**)&sceLncUtilStartLaunchApp);

		serres = sceKernelDlsym(serres, "sceLncUtilInitialize", (void**)&sceLncUtilInitialize);
		/*serres =sceKernelDlsym(serres, "sceSystemServiceLaunchApp", (void**)&sceSystemServiceLaunchApp);
		serres =sceKernelDlsym(serres, "sceLncUtilStartLaunchAppByTitleId", (void**)&sceLncUtilStartLaunchAppByTitleId);*/

		if(serres != 0)//sce_ok
		{
			if(sceLncUtilInitialize == nullptr)
			{
				//sceLncUtilInitialize = *(&sceLncUtilInitialize + 0x1110);
				if(sceLncUtilInitialize == nullptr)
				{				//no pointer
					notify("No Pointer for Initlialize");
					return false;
				}
				//(void**)&sceLncUtilInitialize = (void**)(&sceLncUtilStartLaunchApp +0x1110);
			}
		}

		if(serres == 0)
		{
			/*sceLncUtilInitialize = (void*)(serres + 0x1110);
			sceLncUtilLaunchApp = (void*)(serres + 0x1130);
			if(!sceLncUtilInitialize || !sceLncUtilLaunchApp)
			printf("error\n");*/


			UserServiceInitializeParams params;
			//set it in memory
			memset(&params, 0, sizeof(params));
			params.priority = 700;
			//do this
			//notify("Getting user id");
			int ret =InitlizieUserService();

			if(ret != 0)
			{
				notify("Error could not initlize user service");
				return false;
			}
			notify("Getting Initial User");
			ret = GetInitialUser();
			if(ret != 0)
			{
				notify("Error sceUserServiceGetInitialUser Failed");
				return false;
			}

			LaunchAppParam param;
			param.size = sizeof(LaunchAppParam);
			param.user_id = userId;
			param.app_attr = 0;//LaunchByDebugger
			param.enable_crash_report = 0;
			param.check_flag = SkipLaunchCheck;

			notify("sceLncUtilInitialize");
			//klog("sceLncUtilInitialize %x\n", sceLncUtilInitialize());
			ret = sceLncUtilInitialize();
			if(ret != 0)
			{
				notify("Error sceLncUtilInitialize Failed");
				return false;
			}
			notify("Launcing");
			uint64_t l2 = sceLncUtilStartLaunchApp(titleId, 0, &param);
			if(l2 != 0)
			{
				switch (l2) {
				case SCE_LNC_ERROR_APP_NOT_FOUND: {
					notify("App is NOT Found ref: SCE_LNC_ERROR_APP_NOT_FOUND");
					break;
												  }
				case SCE_LNC_UTIL_ERROR_ALREADY_RUNNING: {
					notify("App is already running ref: SCE_LNC_UTIL_ERROR_ALREADY_RUNNING");
					break;
														 }
				case SCE_LNC_UTIL_ERROR_ALREADY_RUNNING_KILL_NEEDED: {
					notify("ALREADY RUNNING KILL NEEDED");
					break;
																	 }
				case SCE_LNC_UTIL_ERROR_ALREADY_RUNNING_SUSPEND_NEEDED: {
					notify("ALREADY RUNNING SUSPEND NEEDED");
					break;
																		}
				case SCE_LNC_UTIL_ERROR_SETUP_FS_SANDBOX: {
					notify( "App is NOT Launchable ref: SCE_LNC_UTIL_ERROR_SETUP_FS_SANDBOX");
					break;
														  }
				case SCE_LNC_UTIL_ERROR_INVALID_TITLE_ID: {
					notify( "TITLE_ID IS NOT VAILED ref: SCE_LNC_UTIL_ERROR_SETUP_FS_SANDBOX");
					break;
														  }

				default:{
					//msgok(WARNING, "App Launch has failed with error code: 0x%x", sys_res);
					break;
						}
				}
			}
			//l2 = sceLncUtilStartLaunchAppByTitleId("NPXS22010", 0, &param);
			//l2 = sceLncUtilStartLaunchApp("NPXS22010", 0, &param);
			notify("Launched");
			return true;
		}
		else
		{
			notify("Can not reoslve sceSystemServiceLaunchApp");
			return false;
		}
	}
	else 
	{
		notify("Can not reoslve libSceSystemService");
		return false;
	}

	return true;
}