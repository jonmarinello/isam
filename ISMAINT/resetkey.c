#include "../include/iisam.h"
#include "../include/ismaint.h"

resetkey() {
	actkey->kd.k_rootnode = 0L;
	actkey->kd.k_len = 0;
	actkey->kd.k_flags = 0;
}
