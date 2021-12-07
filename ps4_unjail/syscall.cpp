/*
*
*        _       _________ ______            _______  _______  _______  ______   _______  _______
*       ( \      \__   __/(  ___ \ |\     /|(  ___  )(       )(  ____ \(  ___ \ (  ____ )(  ____ \|\     /|
*       | (         ) (   | (   ) )| )   ( || (   ) || () () || (    \/| (   ) )| (    )|| (    \/| )   ( |
*       | |         | |   | (__/ / | (___) || |   | || || || || (__    | (__/ / | (____)|| (__    | | _ | |
*       | |         | |   |  __ (  |  ___  || |   | || |(_)| ||  __)   |  __ (  |     __)|  __)   | |( )| |
*       | |         | |   | (  \ \ | (   ) || |   | || |   | || (      | (  \ \ | (\ (   | (      | || || |
*       | (____/\___) (___| )___) )| )   ( || (___) || )   ( || (____/\| )___) )| ) \ \__| (____/\| () () |
*       (_______/\_______/|/ \___/ |/     \|(_______)|/     \|(_______/|/ \___/ |/   \__/(_______/(_______)
*
*
*
*/

#include "syscall.h"
#include <stdint.h>

/* ----------------------------------------------------------------------- */
/* PS4 FreeBSD Linux-Style Syscall implementation [64 bit]. (The Real one) */
/* ----------------------------------------------------------------------- */
/*
long syscall(long num, ...) {
	int64_t result = 0;                               // Storage for the return value.
	__asm__(".intel_syntax noprefix\n"                // Tell the compiller we are gonna using intel asm syntax.
		"mov rax, rdi\n"                              // Shift Registers: Syscall number into register rax.
		"mov rdi, rsi\n"                              // Argument 2 into register for argument 1. (rdi)
		"mov rsi, rdx\n"                              // And so on for all others too arg2 - arg5.
		"mov rdx, r10\n"
		"mov r10, r8\n"
		"mov r8, r9\n"
		"mov r9, [rsp+8]\n"                           // Kernel calls only take 6 arguments, so we need to shift the last argument as first arg from the stack register.
		"syscall"                                     // Now we can make a syscall.
		: "=a" (result)                               // Output pipped into result.
		:                                             // No input.
		: "rcx", "r11", "memory"                      // Clobbering.
	);

	if (result >= -4095 && result < -1) {             // Check result for error.
		errno = -result;                              // Set errno.
		return -1;                                    // Return a error to the caller.
	} else return result;                             // Return the result to the caller.
}
*/

/* ----------------------------------------------------------------------- */
/* PS4 FreeBSD Linux-Style Syscall0 implementation [64 bit]. (The Art one) */
/* ----------------------------------------------------------------------- */
long syscall(long num, ...) {
	int64_t result = 0;                               // Storage for the result value.	
	__asm__(".intel_syntax noprefix\n"                // No need to shift anythign here, registers are already set up.
		"xor rax, rax\n"                              // We just need to make sure that rax, the register which holds the syscall number, holds 0 for syscall0.		
		"syscall"                                     // Now we just call the kernel and automaticly the correct registers are used.
		: "=a" (result)                               // Pipe return value in our function.
		::                                            // No input, no clobbering.
	);
	return result;                                    // Return the result to the caller.
}



