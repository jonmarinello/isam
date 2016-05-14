# include "../../include/genutil.h"

char	*gfname(path)
char	*path;
{
	char	*p = path + (unsigned)strlen(path);

	while (p-- > path)
		if (*p == '/')
			break;
	++p;
	return(p);
}
