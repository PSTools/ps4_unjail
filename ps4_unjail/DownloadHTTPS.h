#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <kernel.h>
#include <libdbg.h>
#include <sceerror.h>
#include <scetypes.h>
#include <net.h>
#include <libhttp.h>
#include "SysUtil.h"

SceUChar8* Download(const char * targetUrl);