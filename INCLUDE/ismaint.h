/************************************************************************/
/*                                                                      */
/*                              LOGORDI S.A.                            */
/*                                                                      */
/*                  - ISMAINT MACRO & DATA DESCRIPTIONS -               */
/*                                                                      */
/************************************************************************/

#define ASCENDING       '+'
#define DESCENDING      '-'
#define MAXSLEN         128
#define BELL            '\007'

extern  char    *cmfname;
extern  char    ofilename[];
extern  char    nfilename[];
extern  char    sfname[];
extern  char    ufile[];
extern  char    ofile[];
extern  char    isarg[];
extern  short   recordlength;
extern  long    dupspress;
extern  FILE    *ofp;
extern  FILE    *sfile;
extern  int     nfp;
extern  char    *record;
extern  long    livecnt;
extern  long    deadcnt;
extern  short   overall;
extern  short   oln, nln;
extern  short   setup;
extern  long    uid;
extern  int     gotuid;

struct  keys {
        struct  keydesc kd;
        short   compression;
        short   duplicates;
        struct  keys    *link;
};

extern  struct  keys    *head, *tail, *actkey;
extern  struct  keys    *allockey();
extern  char    *gfname();
