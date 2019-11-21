//
// Created by Joris GOUNAND on 11/8/19.
//

#include <asm/unistd.h>
#include <asm/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
# include <stdbool.h>

#include <dirent.h>
#include <elf.h>
#include <string.h>

# include <stdio.h> // DELL printfs


# define __syscall3(type,name,type1,arg1,type2,arg2,type3,arg3)\
type name(type1 arg1,type2 arg2, type3 arg3)\
{\
	register long long    syscall_no  asm("rax") = __NR_##name;\
	register type1   arg_1        asm("rdi") = arg1;\
	register type2        arg_2        asm("rsi") = arg2;\
	register type3    arg_3        asm("rdx") = (type3)arg3;\
	asm("syscall");\
	register type i asm("rax");\
return i;\
}
# define __syscall2(type,name,type1,arg1,type2,arg2)\
type name(type1 arg1,type2 arg2)\
{\
	register long long    syscall_no  asm("rax") = __NR_##name;\
	register type1   arg_1        asm("rdi") = arg1;\
	register type2        arg_2        asm("rsi") = arg2;\
	asm("syscall");\
	register type i asm("rax");\
return i;\
}
# define __syscall1(type,name,type1,arg1)\
type name(type1 arg1)\
{\
	register long long    syscall_no  asm("rax") = __NR_##name;\
	register type1  arg_1        asm("rdi") = arg1;\
	asm("syscall");\
	register type i asm("rax");\
return i;\
}

# define __syscall0(type,name)\
type name(void)\
{\
	register long long    syscall_no  asm("rax") = __NR_##name;\
	asm("syscall");\
	register type i asm("rax");\
return i;\
}

__syscall3(ssize_t, write, int, fd, const void *, buf, size_t, count); // poura etre decalle apres (used for debug)
__syscall2(int, rename, const char *, old, const char *, new);
__syscall1(int, close, int, fd);
__syscall1(int, exit, int, status);

size_t write2(int rdi,const void* rsi, size_t rdx)
{
		register int    syscall_no  asm("rax") = 1;
		register int    arg1        asm("rdi") = rdi;
		register const void*        arg2        asm("rsi") = rsi;
		register size_t    arg3        asm("rdx") = rdx;
		asm("syscall");
		register int i asm("rax");
	return i;
}
int exit1(unsigned int rdi)
{
	register long long    syscall_no  asm("rax") = 60;
	register unsigned int    arg1        asm("rdi") = rdi;
	asm("syscall");
}
void _start()
{

	asm(".globl real_start\n"
	                       "real_start:\n"
	                       "call go_main\n"
	);
	exit(0);
}
int go_main(void)
{
	write(1,"toto 4\n",7);

	return (0);
}