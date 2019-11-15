//
// Created by Joris GOUNAND on 11/12/19.
//

#ifndef FAMINE_VIRUS_H
#define FAMINE_VIRUS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <linux/fcntl.h>
#include <errno.h>
#include <elf.h>
#include <asm/unistd.h>
#include <asm/stat.h>


#include <sys/types.h>
#include <dirent.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096


typedef struct	s_file
{
	int         fd;
	char		*data;
	size_t		size;
	int        error;
}				t_file;

struct linux_dirent64 {
	long long        d_ino;    /* 64-bit inode number */
	unsigned long long        d_off;    /* 64-bit offset to next structure */
	unsigned short d_reclen; /* Size of this dirent */
	unsigned char  d_type;   /* File type */
	char           d_name[]; /* Filename (null-terminated) */
};

#endif //FAMINE_VIRUS_H
