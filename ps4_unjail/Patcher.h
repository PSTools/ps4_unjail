//#pragma once
//#include <cstdarg>
//#include <sys/stat.h>
//#include <stddef.h>
//#include <stdint.h>
//#include <stdio.h>
//#include <iostream>
//
//extern uint8_t* gKernelBase;

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <_kernel.h>
#include <stdlib.h>

#define PROT_READ        0x1                /* Page can be read.  */
#define PROT_WRITE        0x2                /* Page can be written.  */
#define PROT_EXEC        0x4                /* Page can be executed.  */
#define PROT_NONE        0x0                /* Page can not be accessed.  */
#define PROT_GROWSDOWN        0x01000000        /* Extend change to start of
growsdown vma (mprotect only).  */
#define PROT_GROWSUP        0x02000000        /* Extend change to start of
growsup vma (mprotect only).  */

#define MNT_UPDATE      0x0000000000010000ULL /* not real mount, just update */
#define MNT_DELEXPORT   0x0000000000020000ULL /* delete export host lists */
#define MNT_RELOAD      0x0000000000040000ULL /* reload filesystem data */
#define MNT_FORCE       0x0000000000080000ULL /* force unmount or readonly */
#define MNT_SNAPSHOT    0x0000000001000000ULL /* snapshot the filesystem */
#define MNT_BYFSID      0x0000000008000000ULL /* specify filesystem by ID. */
#define MNT_CMDFLAGS   (MNT_UPDATE      | MNT_DELEXPORT | MNT_RELOAD    | \
	MNT_FORCE       | MNT_SNAPSHOT  | MNT_BYFSID)
void InstallPatches(int FW);


