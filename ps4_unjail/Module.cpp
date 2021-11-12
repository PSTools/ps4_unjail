//#include "syscall.h"
//#include "Kernel.h"
//
//#include "Module.h"
//
///*this needs to be resloved*/
//int (*sceSysmoduleLoadModule)(int id);
//
//
////SYSCALL(getFunctionAddressByName, 591);
////SYSCALL(getLoadedModules, 592);
//
//int getModuleInfo(int loadedModuleID, struct moduleInfo *destination) {
//	destination->size = sizeof(destination);
//	return syscall(593, loadedModuleID, destination);
//}
//
////int loadModule(const char *name, int *idDestination) {
////	return syscall(594, name, 0, idDestination, 0);
////}
//
//int unloadModule(int id) {
//	return syscall(595, id, 0, 0);
//}
//
//void initModule(void) {
//	int libModule = sceKernelLoadStartModule("libSceSysmodule.sprx", 0, NULL, 0, 0, 0);
//	
//	// Just use sceKernelLoadStartModule instead
//	RESOLVE(libModule, sceSysmoduleLoadModule);
//}