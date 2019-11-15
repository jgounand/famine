//
// Created by Joris GOUNAND on 11/8/19.
//

#include "../inc/famine.h"

int decrypt(void *start,size_t size, int key)
{
	void *dst;
	typedef long (*JittedFunc)(int);

	if (start || size || key)
	{;}

	if (mprotect(start, size, PROT_WRITE | PROT_READ) == -1) {
		//perror("mprotect");
		return -1;
	}
	//memcpy(dst, code, sizeof(code));

	if (mprotect(start, size, PROT_READ | PROT_EXEC) == -1) {
		perror("mprotect");
		return -1;
	}
	JittedFunc func = dst;
	func(0);
		//printf("test Jitted %d\n",(int)func(0));
		return 0;
}

void decrypter(void)
{
	size_t	size;
	size_t	offset;
	char	*start;

	if(size)
	{
		start[offset] ^= ;
		offset++;
		size++;
		while (offset < size)
		{
			start[offset] ^= start[offset - 1];
			offset++;
		}
	}
}
