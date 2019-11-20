//
// Created by Joris GOUNAND on 11/8/19.
//

#include <asm/unistd.h>
#include <asm/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
# include <stdbool.h>
#include <stdlib.h>

#include <dirent.h>
#include <elf.h>
#include <string.h>

# include <stdio.h> // DELL printfs
#include <unistd.h>


int mywrite(long long rax,long long rdi,char* rsi,  long long rdx)
{
		register long long    syscall_no  asm("rax") = rax;
		register long long    arg1        asm("rdi") = rdi;
		register char*        arg2        asm("rsi") = rsi;
		register long long    arg3        asm("rdx") = rdx;
		asm("syscall");
		return 0;
}

void _start()
{

	asm(".globl real_start\n"
	                       "real_start:\n"
	                       "call go_main\n"
	                       "ret\n"
	);
}



int go_main(void)
{
	mywrite(1,1,"hello, world!\n",14);
	return (0);
}