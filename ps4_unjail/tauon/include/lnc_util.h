#pragma once

#include "prerequisites.h"

#ifdef __cplusplus
extern "C" {
#endif

int sceLncUtilBlockAppSuspend(const char* service_name);
int sceLncUtilUnblockAppSuspend(const char* service_name);

#ifdef __cplusplus
}
#endif
