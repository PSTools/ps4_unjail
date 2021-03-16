#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <time.h>



void notify(char* message);
unsigned char GetElapsed(unsigned long ResetInterval);

extern int sock;
extern time_t prevtime;

#define SSIZET_FMT "%zd"

#endif
