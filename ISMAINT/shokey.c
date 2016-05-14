#include "../include/iisam.h"
#include "../include/ismaint.h"

shokey(actkey)
struct	keys	*actkey;
{
	int	i;

	printf("\nDuplicate keys: %s\n",
		(actkey->duplicates ? "YES" : "NO"));
	printf("Key compression: %s\n",
		(actkey->compression ? "YES" : "NO"));
	printf("\nKey parts: %d\t", actkey->kd.k_nparts);
	for (i = 0; i < actkey->kd.k_nparts; ++i)
		printf("[- %d -] ", i + 1);
	printf("\nLength:\t\t");
	for (i = 0; i < actkey->kd.k_nparts; ++i)
		printf("  %5d ", actkey->kd.k_part[i].kp_leng);
	printf("\nPosition:\t");
	for (i = 0; i < actkey->kd.k_nparts; ++i)
		printf("  %5d ", actkey->kd.k_part[i].kp_start);
	printf("\nAsc/Desc:\t");
	for (i = 0; i < actkey->kd.k_nparts; ++i) {
		switch (actkey->kd.k_part[i].kp_type & ISDESC) {
			case ISDESC :
				printf("      %c ", DESCENDING);
				break;
			default :
				printf("      %c ", ASCENDING);
		}
	}
	printf("\nData type:\t");
	for (i = 0; i < actkey->kd.k_nparts; ++i) {
		switch (actkey->kd.k_part[i].kp_type & TYPEMASK) {
			case CHARTYPE :
				printf("   char ");
				break;
			case INTTYPE :
				printf("    int ");
				break;
			case LONGTYPE :
				printf("   long ");
				break;
			case DOUBLETYPE :
				printf(" double ");
				break;
			case FLOATTYPE :
				printf("  float ");
				break;
			case SHORTTYPE :
				printf("  short ");
				break;
		}
	}
	printf("\n\n[Hit Return to continue] ");
	gets(isarg);
}
