#pragma once

#include "prerequisites.h"

#ifdef __cplusplus
extern "C" {
#endif

int sceShellCoreUtilShowErrorDialog(int error_code);

int sceShellCoreUtilExitMiniApp(void);
int sceShellCoreUtilExitMiniAppWithValue(const char* value);

#ifdef __cplusplus
}
#endif
