#include <kernel_ex.h>
#include "SysUtil.h"
#include "SaveDataDialog.h"

int32_t (*sceSaveDataDialogInitialize)();

int32_t (*sceSaveDataDialogTerminate)( void );

OrbisCommonDialogStatus (*sceSaveDataDialogUpdateStatus)(void);

OrbisCommonDialogStatus (*sceSaveDataDialogGetStatus)(void);

int32_t (*sceSaveDataDialogGetResult)(OrbisSaveDataDialogResult* result);

int32_t (*sceSaveDataDialogOpen)(const OrbisSaveDataDialogParam *param);


int32_t (*sceSaveDataDialogClose)(const OrbisSaveDataDialogCloseParam *closeParam);

int32_t (*sceSaveDataDialogProgressBarInc)(OrbisSaveDataDialogProgressBarTarget target,
										   uint32_t delta);
int32_t (*sceSaveDataDialogProgressBarSetValue)(OrbisSaveDataDialogProgressBarTarget target,
												uint32_t rate);

int32_t (*sceSaveDataDialogIsReadyToDisplay)();

void InitilizeSaveDataDialog()
{
	//libSceSaveDataDialog
	int LibOrbisSaveDataDialog = sceKernelLoadStartModule("/system/common/lib/libSceSaveDataDialog.sprx", 0, NULL, 0, NULL, NULL);
	sceKernelDlsym(LibOrbisSaveDataDialog, "sceSaveDataDialogInitialize", (void **)&sceSaveDataDialogInitialize);
	sceKernelDlsym(LibOrbisSaveDataDialog, "sceSaveDataDialogTerminate", (void **)&sceSaveDataDialogTerminate);
	sceKernelDlsym(LibOrbisSaveDataDialog, "sceSaveDataDialogUpdateStatus", (void **)&sceSaveDataDialogUpdateStatus);
	sceKernelDlsym(LibOrbisSaveDataDialog, "sceSaveDataDialogGetStatus", (void **)&sceSaveDataDialogGetStatus);
	sceKernelDlsym(LibOrbisSaveDataDialog, "sceSaveDataDialogGetResult", (void **)&sceSaveDataDialogGetResult);
	sceKernelDlsym(LibOrbisSaveDataDialog, "sceSaveDataDialogOpen", (void **)&sceSaveDataDialogOpen);
	sceKernelDlsym(LibOrbisSaveDataDialog, "sceSaveDataDialogClose", (void **)&sceSaveDataDialogClose);
	sceKernelDlsym(LibOrbisSaveDataDialog, "sceSaveDataDialogProgressBarInc", (void **)&sceSaveDataDialogProgressBarInc);
	sceKernelDlsym(LibOrbisSaveDataDialog, "sceSaveDataDialogProgressBarSetValue", (void **)&sceSaveDataDialogProgressBarSetValue);
	sceKernelDlsym(LibOrbisSaveDataDialog, "sceSaveDataDialogIsReadyToDisplay", (void **)&sceSaveDataDialogIsReadyToDisplay);
}

int32_t orbisSaveDataDialogInitialize()
{
	return sceSaveDataDialogInitialize();
}

int32_t orbisSaveDataDialogTerminate( void )
{
	return sceSaveDataDialogTerminate();
}

OrbisCommonDialogStatus orbisSaveDataDialogUpdateStatus(void)
{
	return sceSaveDataDialogUpdateStatus();
}

OrbisCommonDialogStatus orbisSaveDataDialogGetStatus(void)
{
	return sceSaveDataDialogGetStatus();
}


int32_t orbisSaveDataDialogGetResult(OrbisSaveDataDialogResult* result)
{
	return sceSaveDataDialogGetResult(result);
}


int32_t orbisSaveDataDialogOpen(const OrbisSaveDataDialogParam *param)
{
	return sceSaveDataDialogOpen(param);
}

int32_t orbisSaveDataDialogClose(const OrbisSaveDataDialogCloseParam *closeParam)
{
	return sceSaveDataDialogClose(closeParam);
}

int32_t orbisSaveDataDialogProgressBarInc(OrbisSaveDataDialogProgressBarTarget target,
										uint32_t delta)
{
	return sceSaveDataDialogProgressBarInc(target,delta);
}

int32_t orbisSaveDataDialogProgressBarSetValue(OrbisSaveDataDialogProgressBarTarget target,
											 uint32_t rate)
{
	return sceSaveDataDialogProgressBarSetValue(target,rate);
}

int32_t orbisSaveDataDialogIsReadyToDisplay()
{
	return sceSaveDataDialogIsReadyToDisplay();
}
