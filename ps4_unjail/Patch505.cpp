//#include <stddef.h>
//#include <stdint.h>
//
//int (*memcmp)(const void *ptr1, const void *ptr2, size_t num);
//int (*_sx_xlock)(struct sx *sx, int opts, const char *file, int line);
//int (*_sx_xunlock)(struct sx *sx);
//void* (*malloc)(unsigned long size, void* type, int flags);
//void (*free)(void* addr, void* type);
//char * (*strstr) (const char *haystack, const char *needle);
//int (*fpu_kern_enter)(struct thread *td, struct fpu_kern_ctx *ctx, uint32_t flags);
//int (*fpu_kern_leave)(struct thread *td, struct fpu_kern_ctx *ctx);
//void* (*memcpy)(void* dst, const void* src, size_t len);
//void* (*memset)(void *s, int c, size_t n);
//size_t (*strlen)(const char *str);
//int (*printf)(const char *fmt, ...);
//void (*eventhandler_register)(void *list, const char *name, void *func, void *arg, int priority);
//
//void* M_TEMP;
//uint8_t* MINI_SYSCORE_SELF_BINARY;
//struct proc** ALLPROC;
//struct sbl_map_list_entry** SBL_DRIVER_MAPPED_PAGES;
//struct sx* SBL_PFS_SX;
//struct sbl_key_slot_queue* SBL_KEYMGR_KEY_SLOTS;
//struct sbl_key_rbtree_entry** SBL_KEYMGR_KEY_RBTREE;
//uint8_t* SBL_KEYMGR_BUF_VA;
//uint64_t* SBL_KEYMGR_BUF_GVA;
//void* FPU_CTX;
//
//
//#define resolve(name) name = (void *)(kernbase + name##_addr)
//
//int my_entrypoint()
//{
//	resolve_kdlsym();
//	//install_fself_hooks();
//	//install_fpkg_hooks();
//	//install_patches();
//	return shellcore_fpkg_patch();
//}
//
//void resolve_kdlsym()
//{
//	uint64_t kernbase = getkernbase();
//
//	// data
//	resolve(M_TEMP);
//	resolve(MINI_SYSCORE_SELF_BINARY);
//	resolve(ALLPROC);
//	resolve(SBL_DRIVER_MAPPED_PAGES);
//	resolve(SBL_PFS_SX);
//	resolve(SBL_KEYMGR_KEY_SLOTS);
//	resolve(SBL_KEYMGR_KEY_RBTREE);
//	resolve(SBL_KEYMGR_BUF_VA);
//	resolve(SBL_KEYMGR_BUF_GVA);
//	resolve(FPU_CTX);
//
//	// common
//	resolve(memcmp);
//	resolve(_sx_xlock);
//	resolve(_sx_xunlock);
//	resolve(malloc);
//	resolve(free);
//	resolve(strstr);
//	resolve(fpu_kern_enter);
//	resolve(fpu_kern_leave);
//	resolve(memcpy);
//	resolve(memset);
//	resolve(strlen);
//	resolve(printf);
//	resolve(eventhandler_register);
//
//	//// Fself
//	//resolve(sceSblACMgrGetPathId);
//	//resolve(sceSblServiceMailbox);
//	//resolve(sceSblAuthMgrSmIsLoadable2);
//	//resolve(_sceSblAuthMgrGetSelfInfo);
//	//resolve(_sceSblAuthMgrSmStart);
//	//resolve(sceSblAuthMgrVerifyHeader);
//
//	//// Fpkg
//	//resolve(sceSblPfsSetKeys);
//	//resolve(sceSblKeymgrCleartKey);
//	//resolve(sceSblKeymgrSetKeyForPfs);
//	//resolve(sceSblKeymgrSetKeyStorage);
//	//resolve(sceSblKeymgrSmCallfunc);
//	//resolve(sceSblDriverSendMsg_0);
//	//resolve(RsaesPkcs1v15Dec2048CRT);
//	//resolve(AesCbcCfb128Encrypt);
//	//resolve(AesCbcCfb128Decrypt);
//	//resolve(Sha256Hmac);
//
//	// Patch
//	resolve(proc_rwmem);
//	resolve(vmspace_acquire_ref);
//	resolve(vmspace_free);
//	resolve(vm_map_lock_read);
//	resolve(vm_map_unlock_read);
//	resolve(vm_map_lookup_entry);
//}
//
//struct payload_headers
//{
//	uint64_t signature;
//	void* entrypoint_offset;
//};
//
//
//
//int _start()
//{
//	payload_headers PAYLOAD_HEADER = {
//	0x5041594C4F414458ull,
//	&my_entrypoint,
//};
//
//	return 0;
//}