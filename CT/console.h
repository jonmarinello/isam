#define	MAXSTRING	50
#define PRI	"FILE1"
#define SEC	"FILE2"

struct CON {
	int	key;
	char	comment[MAXSTRING];
};

#include "../include/iisam.h"
#include "../include/ismaint.h"
