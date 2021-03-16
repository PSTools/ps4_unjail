#pragma once

class Sys {
private:
	//private for me

public:
	static int open(const char *path, int flags, int mode);
	static int kexec(void* func, void *user_arg);
	static int getuid();
	static int getpid();
};