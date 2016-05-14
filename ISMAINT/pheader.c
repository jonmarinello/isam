#include "../include/iisam.h"
#include "../include/ismaint.h"

pheader(s)
char	*s;
{
	printf("%s %s\n", s, isversnumber);
	printf("%s\n", iscopyright);
	printf("Software Serial Number %ld\n\n", isserial);
}
