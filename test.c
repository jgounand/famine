#include<asm/unistd.h>             /* __NR_write */
#include<unistd.h>                 /* STDOUT_FILENO */
#include<string.h>                 /* strlen() */

#define STDOUT 1

int main(void)
{
	 char msg[] = "hello!\n";
	  int res;

	   __asm__("mov \t%2, %%rbx\n\t"
			            "int \t$0x80"

				             :"=a" (res)
					              :"0" (__NR_write), "I" (STDOUT),
						                "c" (msg), "d" (strlen(msg))
								         :"rbx");

	   return 0;
}
#define __syscall2(type,name,type1,arg1,type2,arg2) \
type name(type1 arg1,type2 arg2) \
{ \
long __res; \
__asm__ volatile ("int $0x80" \
        : "=a" (__res) \
        : "0" (__NR_##name),"b" ((long)(arg1)),"c" ((long)(arg2))); \
return(type)__res; \
}