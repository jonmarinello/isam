#include "../include/iisam.h"
#include "../include/ismaint.h"

getfinfo(i)
short	i;
{
	short	tmpshort;
	char	tmpchar;

	actkey->kd.k_part[i].kp_type = 0;
	do {
		printf("\nLength of field #%d: ", i + 1);
		tmpshort = atoi(gets(isarg));
		if (tmpshort <= 0 || tmpshort > recordlength)
			printf("\n%cInvalid field length\n", BELL);
	} while (tmpshort <= 0 || tmpshort > recordlength);
	overall += actkey->kd.k_part[i].kp_leng = tmpshort;
	do {
		printf("\nPosition of field #%d: ", i + 1);
		tmpshort = atoi(gets(isarg));
		if (tmpshort < 0 || tmpshort + actkey->kd.k_part[i].kp_leng >
			recordlength)
				printf( "\n%cInvalid field position\n", BELL);
	} while (tmpshort < 0 || tmpshort + actkey->kd.k_part[i].kp_leng >
		recordlength);
	actkey->kd.k_part[i].kp_start = tmpshort;
	do {
		printf("\nIs field #%d ascending or descending (+/-): ", i + 1);
		gets(isarg);	
		tmpchar = *isarg;
		if (tmpchar != ASCENDING && tmpchar != DESCENDING)
			printf("\n%cInvalid ascending/descending code\n", BELL);
	} while (tmpchar != ASCENDING && tmpchar != DESCENDING);
	if (tmpchar == DESCENDING)
		actkey->kd.k_part[i].kp_type = ISDESC;
	for (;;) {
		printf("\nIs field #%d of type  (C)haracter,", i + 1);
		printf("\n                     (I)nteger,");
		printf("\n                     (L)ong,");
		printf("\n                     (D)ouble,");
		printf("\n                     (F)loat,");
		printf("\n                     (S)hort");
		printf("\n\n                      Choice: ");
		gets(isarg);	
		switch ((tmpchar = toupper(*isarg))) {
			case 'C' :
				actkey->kd.k_part[i].kp_type += CHARTYPE;
				return;
			case 'I' :
				actkey->kd.k_part[i].kp_type += INTTYPE;
				return;
			case 'L' :
				actkey->kd.k_part[i].kp_type += LONGTYPE;
				return;
			case 'D' :
				actkey->kd.k_part[i].kp_type += DOUBLETYPE;
				return;
			case 'F' :
				actkey->kd.k_part[i].kp_type += FLOATTYPE;
				return;
			case 'S' :
				actkey->kd.k_part[i].kp_type += SHORTTYPE;
				return;
			default :
				continue;
		}
	}
}
