#include <stdio.h>

#include "segment.h"

main(argc, argv)
int argc;
char *argv[];
{
  FILE *Rf;
  reglist rlist;
  region r;
  region mr;
  int nr;
  float carray[4];
  int maxpix = 0;
  int maxreg = 0;
  int i;

  if (argc != 2) {
    fprintf(stderr, "Usage: stest fname\n");
      exit(1);
  }
  Rf = fopen(argv[1], "r");
  if (!Rf) {
    fprintf(stderr, "Can't open file %s\n", argv[1]);
    exit(1);
  }

  nr = fread(&rlist, sizeof(reglist), 1, Rf);
  if (nr != 1) {
    fprintf(stderr, "Can't read reglist\n");
    exit(1);
  }

  printf("# regions  = %d\n", rlist.nreg);

  for (i=0; i<rlist.nreg; i++) {
    nr = fread(&r, sizeof(region), 1, Rf);
    if (nr != 1) {
      fprintf(stderr, "EOF on region %d\n", i);
      exit(1);
    }
    nr = fread(carray, sizeof(float), 4, Rf);
    if (nr != 4) {
      fprintf(stderr, "EOF on region %d\n", i);
      exit(1);
    }
    if ( r.npix > maxpix) {
      maxpix = r.npix;
      maxreg = i;
      mr = r;
    }
  }

  printf("Region # %d has max of %d pixels\n", maxreg, maxpix);
  printf("Region uleft = (%d, %d);  lright = (%d, %d)\n",
	 mr.uleft.x, mr.uleft.y, mr.lright.x, mr.lright.y);
}
