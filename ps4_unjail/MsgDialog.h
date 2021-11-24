#ifndef _ORBIS_MSG_H_
#define _ORBIS_MSG_H_
#pragma once

#include <stdint.h>

extern "C" {

#include <common_dialog.h>

#define ORBIS_SYSMODULE_MESSAGE_DIALOG				0x00a4
#define ORBIS_MSG_DIALOG_MODE_USER_MSG				(1)
#define ORBIS_MSG_DIALOG_BUTTON_TYPE_WAIT			(5)
#define ORBIS_MSG_DIALOG_BUTTON_TYPE_OK				(0)

#define ORBIS_MSG_DIALOG_MODE_INVALID				(0)
#define ORBIS_MSG_DIALOG_MODE_USER_MSG				(1)
#define ORBIS_MSG_DIALOG_MODE_PROGRESS_BAR			(2)
#define ORBIS_MSG_DIALOG_MODE_SYSTEM_MSG			(3)

	typedef struct OrbisMsgDialogButtonsParam {
		const char *msg1;					
		const char *msg2;					
		char reserved[32];					
	} OrbisMsgDialogButtonsParam;

	typedef struct OrbisMsgDialogUserMessageParam {
		int32_t buttonType;				
		int :32;										
		const char *msg;								
		OrbisMsgDialogButtonsParam *buttonsParam;		
		char reserved[24];								

	} OrbisMsgDialogUserMessageParam;

	typedef struct OrbisMsgDialogSystemMessageParam {
		int32_t sysMsgType;		
		char reserved[32];								
	} OrbisMsgDialogSystemMessageParam;

	typedef struct OrbisMsgDialogProgressBarParam {
		int32_t barType;			
		int :32;										
		const char *msg;								
		char reserved[64];								
	} OrbisMsgDialogProgressBarParam;



	typedef struct OrbisMsgDialogParam {
		SceCommonDialogBaseParam baseParam; 	
		size_t size;									
		int32_t mode;							
		int :32;	
		OrbisMsgDialogUserMessageParam *userMsgParam;	
		OrbisMsgDialogProgressBarParam *progBarParam;		
		OrbisMsgDialogSystemMessageParam *sysMsgParam;	
		int32_t userId;					
		char reserved[40];								
		int :32;										
	} OrbisMsgDialogParam;

	typedef struct OrbisErrorMsgDialogParam{
		size_t size;
		int32_t errorCode;
		int32_t userId;		
		int :32;	
	} OrbisErrorMsgDialogParam;


	typedef enum OrbisErroDialogTernary {
		False					= 0,
		True					= 1,
		Inderterminate			= 2,
	} OrbisErroDialogTernary;


	typedef struct OrbisErrorMsgDetailParam
	{
		size_t size;
		int32_t errorCode;
		int32_t userId;		
		OrbisErroDialogTernary showUserInfoPanel;
		OrbisErroDialogTernary showCSCode;
		OrbisErroDialogTernary logging;
		bool enableActionError;
		bool enableDimmer;
		int :32;	
	} OrbisErrorMsgDetailParam;





	typedef struct OrbisMsgDialogResult {
		int32_t mode;							

		int32_t result;									

		int32_t buttonId;					

		char reserved[32];								

	} OrbisMsgDialogResult;


	static inline
		void OrbisMsgDialogParamInitialize(OrbisMsgDialogParam *param)
	{
		memset( param, 0x0, sizeof(OrbisMsgDialogParam) );

		_sceCommonDialogBaseParamInit( &param->baseParam );
		param->size = sizeof(OrbisMsgDialogParam);
	}


	typedef enum OrbisCommonDialogStatus {
		ORBIS_COMMON_DIALOG_STATUS_NONE				= 0,
		ORBIS_COMMON_DIALOG_STATUS_INITIALIZED		= 1,
		ORBIS_COMMON_DIALOG_STATUS_RUNNING			= 2,
		ORBIS_COMMON_DIALOG_STATUS_FINISHED			= 3
	} OrbisCommonDialogStatus;

}
#endif