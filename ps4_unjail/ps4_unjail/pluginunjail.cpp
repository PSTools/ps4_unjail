#include "pluginunjail.h"
#include "syscall.h"

int Sys::open(const char *path, int flags, int mode) 
{ 
	return syscall(5, path, flags, mode); 
}

int Sys::kexec(void* func, void *user_arg) { 
	return syscall(11, func, user_arg); 
}

int Sys::getuid() { return syscall(24); }
int Sys::getpid() { return syscall(20); }