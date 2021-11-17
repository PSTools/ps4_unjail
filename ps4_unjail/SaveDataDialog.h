#include "UserService.h"
#include "CommonDialog.h"
#include "Saves.h"

//revesed
#define ORBIS_SAVE_DATA_DIALOG_USER_MSG_MAXSIZE			(256)


typedef int32_t OrbisSaveDataDialogMode;
#define ORBIS_SAVE_DATA_DIALOG_MODE_INVALID				(0)
#define ORBIS_SAVE_DATA_DIALOG_MODE_LIST				(1)
#define ORBIS_SAVE_DATA_DIALOG_MODE_USER_MSG			(2)
#define ORBIS_SAVE_DATA_DIALOG_MODE_SYSTEM_MSG			(3)
#define ORBIS_SAVE_DATA_DIALOG_MODE_ERROR_CODE			(4)
#define ORBIS_SAVE_DATA_DIALOG_MODE_PROGRESS_BAR		(5)

typedef int32_t OrbisSaveDataDialogType;
#define ORBIS_SAVE_DATA_DIALOG_TYPE_INVALID				(0)
#define ORBIS_SAVE_DATA_DIALOG_TYPE_SAVE				(1)
#define ORBIS_SAVE_DATA_DIALOG_TYPE_LOAD				(2)
#define ORBIS_SAVE_DATA_DIALOG_TYPE_DELETE				(3)


typedef int32_t OrbisSaveDataDialogFocusPos;
#define ORBIS_SAVE_DATA_DIALOG_FOCUS_POS_LISTHEAD		(0)
#define ORBIS_SAVE_DATA_DIALOG_FOCUS_POS_LISTTAIL		(1)
#define ORBIS_SAVE_DATA_DIALOG_FOCUS_POS_DATAHEAD		(2)
#define ORBIS_SAVE_DATA_DIALOG_FOCUS_POS_DATATAIL		(3)
#define ORBIS_SAVE_DATA_DIALOG_FOCUS_POS_DATALATEST		(4)
#define ORBIS_SAVE_DATA_DIALOG_FOCUS_POS_DATAOLDEST		(5)
#define ORBIS_SAVE_DATA_DIALOG_FOCUS_POS_DIRNAME		(6)

typedef int32_t OrbisSaveDataDialogSystemMessageType;
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_INVALID						(0)
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_NODATA						(1)
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_CONFIRM						(2)
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_OVERWRITE					(3)
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_NOSPACE						(4)
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_PROGRESS						(5)
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_FILE_CORRUPTED				(6)
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_FINISHED						(7)
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_NOSPACE_CONTINUABLE			(8)
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_CORRUPTED_AND_DELETE			(10)
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_CORRUPTED_AND_CREATE			(11)
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_CORRUPTED_AND_RESTORE		(13)
#define ORBIS_SAVE_DATA_DIALOG_SYSMSG_TYPE_TOTAL_SIZE_EXCEEDED			(14)


typedef int32_t OrbisSaveDataDialogButtonType;
#define ORBIS_SAVE_DATA_DIALOG_BUTTON_TYPE_OK			(0)
#define ORBIS_SAVE_DATA_DIALOG_BUTTON_TYPE_YESNO		(1)
#define ORBIS_SAVE_DATA_DIALOG_BUTTON_TYPE_NONE			(2)
#define ORBIS_SAVE_DATA_DIALOG_BUTTON_TYPE_OKCANCEL		(3)

typedef int32_t OrbisSaveDataDialogButtonId;
#define ORBIS_SAVE_DATA_DIALOG_BUTTON_ID_INVALID		(0)
#define ORBIS_SAVE_DATA_DIALOG_BUTTON_ID_OK				(1)
#define ORBIS_SAVE_DATA_DIALOG_BUTTON_ID_YES			(1)
#define ORBIS_SAVE_DATA_DIALOG_BUTTON_ID_NO				(2)


typedef int32_t OrbisSaveDataDialogOptionBack;
#define ORBIS_SAVE_DATA_DIALOG_OPTION_BACK_ENABLE		(0)
#define ORBIS_SAVE_DATA_DIALOG_OPTION_BACK_DISABLE		(1)


typedef int32_t OrbisSaveDataDialogProgressBarType;
#define ORBIS_SAVE_DATA_DIALOG_PROGRESSBAR_TYPE_PERCENTAGE	(0)

typedef int32_t OrbisSaveDataDialogProgressBarTarget;
#define ORBIS_SAVE_DATA_DIALOG_PROGRESSBAR_TARGET_BAR_DEFAULT	(0)

typedef int32_t OrbisSaveDataDialogItemStyle;
#define ORBIS_SAVE_DATA_DIALOG_ITEM_STYLE_TITLE_DATESIZE_SUBTITLE		(0)
#define ORBIS_SAVE_DATA_DIALOG_ITEM_STYLE_TITLE_SUBTITLE_DATESIZE		(1)
#define ORBIS_SAVE_DATA_DIALOG_ITEM_STYLE_TITLE_DATESIZE				(2)


typedef int32_t OrbisSaveDataDialogAnimation;
#define ORBIS_SAVE_DATA_DIALOG_ANIMATION_ON					(0)
#define ORBIS_SAVE_DATA_DIALOG_ANIMATION_OFF				(1)

typedef int32_t OrbisSaveDataDialogUserMessageType;
#define ORBIS_SAVE_DATA_DIALOG_USERMSG_TYPE_NORMAL			(0)
#define ORBIS_SAVE_DATA_DIALOG_USERMSG_TYPE_ERROR			(1)

typedef int32_t OrbisSaveDataDialogProgressSystemMessageType;
#define ORBIS_SAVE_DATA_DIALOG_PRGRESS_SYSMSG_TYPE_INVALID	(0)
#define ORBIS_SAVE_DATA_DIALOG_PRGRESS_SYSMSG_TYPE_PROGRESS	(1)
#define ORBIS_SAVE_DATA_DIALOG_PRGRESS_SYSMSG_TYPE_RESTORE	(2)

typedef struct OrbisSaveDataDialogAnimationParam {
	OrbisSaveDataDialogAnimation			userOK;		
	OrbisSaveDataDialogAnimation			userCancel;
	uint8_t									reserved[32];	
} OrbisSaveDataDialogAnimationParam;

typedef struct OrbisSaveDataDialogNewItem {
	const char							*title;			
	void								*iconBuf;		
	size_t								iconSize;		
	uint8_t								reserved[32];	
} OrbisSaveDataDialogNewItem;

typedef struct OrbisSaveDataDialogItems {
	SceUserServiceUserId				userId;			
int32_t : 32;
	const SceSaveDataTitleId			*titleId;	
	const SceSaveDataDirName			*dirName;		
	uint32_t							dirNameNum;	
int32_t : 32;
	const OrbisSaveDataDialogNewItem	*newItem;		
	OrbisSaveDataDialogFocusPos			focusPos;	
int32_t : 32;
	const SceSaveDataDirName			*focusPosDirName;
	OrbisSaveDataDialogItemStyle		itemStyle;	
	uint8_t								reserved[36];	
} OrbisSaveDataDialogItems;

typedef struct OrbisSaveDataDialogUserMessageParam {
	OrbisSaveDataDialogButtonType		buttonType;	
	OrbisSaveDataDialogUserMessageType	msgType;		
	const char							*msg;			
	uint8_t								reserved[32];	
} OrbisSaveDataDialogUserMessageParam;

typedef struct OrbisSaveDataDialogSystemMessageParam {
	OrbisSaveDataDialogSystemMessageType	sysMsgType;		
int32_t : 32;
	uint64_t							value;			
	uint8_t								reserved[32];	
} OrbisSaveDataDialogSystemMessageParam;

typedef struct OrbisSaveDataDialogErrorCodeParam {
	int32_t								errorCode;		
	uint8_t								reserved[32];	
} OrbisSaveDataDialogErrorCodeParam;


typedef struct OrbisSaveDataDialogProgressBarParam {
	OrbisSaveDataDialogProgressBarType			barType;		
int32_t : 32;
	const char									*msg;			
	OrbisSaveDataDialogProgressSystemMessageType	sysMsgType;		
	uint8_t										reserved[28];
} OrbisSaveDataDialogProgressBarParam;

typedef struct OrbisSaveDataDialogOptionParam {
	OrbisSaveDataDialogOptionBack			back;			
	uint8_t								reserved[32];	
} OrbisSaveDataDialogOptionParam;

typedef struct OrbisSaveDataDialogParam {
	OrbisCommonDialogBaseParam			baseParam;		
	int32_t								size;			
	OrbisSaveDataDialogMode				mode;			
	OrbisSaveDataDialogType				dispType;		
int32_t : 32;
	OrbisSaveDataDialogAnimationParam		*animParam;	
	OrbisSaveDataDialogItems				*items;		
	OrbisSaveDataDialogUserMessageParam	*userMsgParam;
	OrbisSaveDataDialogSystemMessageParam	*sysMsgParam;
	OrbisSaveDataDialogErrorCodeParam		*errorCodeParam;
	OrbisSaveDataDialogProgressBarParam	*progBarParam;
	void								*userData;		
	OrbisSaveDataDialogOptionParam		*optionParam;	
	uint8_t								reserved[24];	
} OrbisSaveDataDialogParam;




typedef struct OrbisSaveDataDialogCloseParam {
	OrbisSaveDataDialogAnimation			anim;			
	uint8_t								reserved[32];
} OrbisSaveDataDialogCloseParam;


typedef struct OrbisSaveDataDialogResult {
	OrbisSaveDataDialogMode				mode;			
	int32_t								result;		
	OrbisSaveDataDialogButtonId			buttonId;		
	int32_t : 32;
	SceSaveDataDirName					*dirName;		
	SceSaveDataParam					*param;			
	void								*userData;		
	uint8_t								reserved[32];
} OrbisSaveDataDialogResult;

void InitilizeSaveDataDialog();

inline void sceSaveDataDialogParamInitialize( OrbisSaveDataDialogParam *param )
{
	memset(param, 0x0, sizeof(OrbisSaveDataDialogParam));

	_orbisCommonDialogBaseParamInit(&param->baseParam);
	param->size = sizeof(OrbisSaveDataDialogParam);
}


int32_t orbisSaveDataDialogInitialize();

int32_t orbisSaveDataDialogTerminate( void );

OrbisCommonDialogStatus orbisSaveDataDialogUpdateStatus(void);


OrbisCommonDialogStatus orbisSaveDataDialogGetStatus(void);

int32_t orbisSaveDataDialogGetResult(OrbisSaveDataDialogResult* result);


int32_t orbisSaveDataDialogOpen(const OrbisSaveDataDialogParam *param);

int32_t orbisSaveDataDialogClose(const OrbisSaveDataDialogCloseParam *closeParam);

/*Increase save data Dialog progress*/
int32_t orbisSaveDataDialogProgressBarInc(OrbisSaveDataDialogProgressBarTarget target,
										uint32_t delta);

//Set the Value of the Dialog progress
int32_t orbisSaveDataDialogProgressBarSetValue(OrbisSaveDataDialogProgressBarTarget target,
											 uint32_t rate);
/*Screen is ready after the dialog has been opened*/
int32_t orbisSaveDataDialogIsReadyToDisplay();