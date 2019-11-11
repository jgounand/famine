//
// Created by Joris GOUNAND on 11/8/19.
//

#ifndef FAMINE_FAMINE_H
#define FAMINE_FAMINE_H

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/mman.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>

# include "elf.h"

# include <inttypes.h>
# define SIGN "Famine version 1.0 (c)oded by <jgounand>-<afiodier>"

bool process_runing(void);
int decrypt(void *src_virus,size_t s_virus, void *src_pre,size_t s_pre, int key);
int virus(void);

typedef struct	s_file
{
	int         fd;
	char		*data;
	size_t		size;
	bool        error;
}				t_file;

typedef struct	s_cave
{
	Elf64_Off	start_gap;
	Elf64_Phdr*	start_seg;
}				t_cave;

int     open_map(char *fname, t_file *file);

#endif //FAMINE_FAMINE_H
