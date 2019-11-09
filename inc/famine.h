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
#include <fcntl.h>


# include <inttypes.h>

bool process_runing(void);
int decrypt(void *src_virus,size_t s_virus, void *src_pre,size_t s_pre, int key);
int virus(void);
#endif //FAMINE_FAMINE_H
