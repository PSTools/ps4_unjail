#ifndef _SYSUTIL_H
#define _SYSUTIL_H

//System Util Call This is a wraper for all sceSysUtil Functions
//e.g. sceSysUtilSendSystemNotificationWithText
//e.g LoadAndExecute

#pragma once

#include <_kernel.h>

// Var is used for debugging
extern int DEBUGENABlED;

void initSysUtil(void);

void notify(char *message);

int SysLoadExec(const char *path, char *const *argv);

bool IS_ERROR(uint32_t a1);

#endif