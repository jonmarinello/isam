#include "../include/iisam.h"
#include "../include/ismaint.h"

gakfields() {
	short	i;

	overall = 0;
	for (i = 0; i < actkey->kd.k_nparts; ++i)
		getfinfo(i);
}
