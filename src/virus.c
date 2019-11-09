//
// Created by Joris GOUNAND on 11/9/19.
//
#include "../inc/famine.h"

int magic = 0;
char env[1024];
		// 1 => changement signature
void change_signature(void)
{
	//changement signature
}

size_t  ft_strlen(const char *s)
{
	size_t  i;

	i = 0;
	while (*(s + i) != '\0')
	{
		i++;
	}
	return (i);
}

int get_env_var(char *name, char *content, int content_size)
{
	char buf[4];
	int fd;
	size_t n,i;
	int j,k = 0;
	int nb_start = 1;
	int name_lenght = ft_strlen(name);
	if ((fd = open("/proc/self/environ",O_RDONLY,0))<0)
		return (0);
	do
	{
		n = read(fd,buf,4);
		for (i= 0; i < n ; ++i)
		{
			if (nb_start)
			{
				if (k != name_lenght)
				{
					if (buf[i] == name[k])
						k++;
					else
					{
						k = 0;
						nb_start = 0;
						j = 0;
					}
				}
				else if (j < content_size - 1)
				{
					if (buf[i] == '\0')
						goto  out;
					content[j++] = buf[i];
				}
				else
					goto out;
			}
			else if (buf[i] == '\0')
				nb_start = 1;
		}
	}
	while(n);
	out:
		content[j] = '\0';
		close(fd);
	return (content[0] != '\0');
}
//BSS:
//UN MAGIC NUMBER
//UN BUFFER POUR LE PATH
//UN BUFFER POUR LE READ
int virus(void)
{
	if (magic == 1)
		change_signature();
	printf("virus\n");
	char path[256];
	int ret  = get_env_var("PATH=",path,256);

	printf("ret %d\n",ret);
	printf("content: %s\n",path);
	return (0);
}