#pragma once

#include "prerequisites.h"

#include <system_service.h>

#ifdef __cplusplus
extern "C" {
#endif

int sceSystemServiceLaunchWebApp(const char* url);

#ifdef __cplusplus
}
#endif
