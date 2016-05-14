#include "../include/iisam.h"
#include "../include/ismaint.h"

dmpkey(K)
struct	keys *K;
{
	int	i;

	fwrite((char *)&K->kd.k_flags, SHORTSIZE, 1, sfile);
	fwrite((char *)&K->kd.k_nparts, SHORTSIZE, 1, sfile);
	for (i = 0; i < K->kd.k_nparts; ++i) {
		fwrite((char *)&K->kd.k_part[i].kp_start,
			SHORTSIZE, 1, sfile);
		fwrite((char *)&K->kd.k_part[i].kp_leng,
			SHORTSIZE, 1, sfile);
		fwrite((char *)&K->kd.k_part[i].kp_type,
			SHORTSIZE, 1, sfile);
	}
	fwrite((char *)&K->kd.k_len, SHORTSIZE, 1, sfile);
	fwrite((char *)&K->kd.k_rootnode, LONGSIZE, 1, sfile);
	fwrite((char *)&K->compression, SHORTSIZE, 1, sfile);
	fwrite((char *)&K->duplicates, SHORTSIZE, 1, sfile);
}
