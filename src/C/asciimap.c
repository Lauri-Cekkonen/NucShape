#include <stdio.h>
#include <math.h>
#include "zstream.h"

/* Contains 2 zstreamelem structs with
 * zstreamtag=SOL and their heights
 * correspond to minimum and maximum
 * z-value. */
typedef struct {
  struct zstreamelem min;
  struct zstreamelem max;
} Minmaxpair;

#define XMIN -1.0
#define XMAX 1.0
#define XNUM 30
#define YMIN -1.0
#define YMAX 1.0
#define YNUM 30
#define MAXGRID 5000

main() {
  double xvals[XNUM];
  double yvals[YNUM];
  struct zstreamelem zvals[MAXGRID];
  
  arrcreate(xvals, XNUM, XMIN, XMAX);
  arrcreate(yvals, YNUM, YMIN, YMAX);
}

/* Makes input double array arr into form
 * [min, min+d, min+2*d, ..., max]
 * with equal spacing
 * d = (max-min)/len. */
void arrcreate(double arr[], int len,
               double min, double max) {
  int i;
  double d = (max-min)/((double)len);
  arr[0] = min;
  for (i = 1; i < len; i++)
    arr[i] = arr[i-1] + d;
}

/* Use function findz to map points (x, y)
 * where x comes from array 
 *   [*xmin, *(xmin+1), ..., *xmax]
 * and y comes from array
 *   [*ymin, *(ymin+1), ..., *ymax].
 * Structs zstreamelem returned by findz
 * are stored in array zstream (side effect). 
 * 
 * Maximum amount of elements in zstream is
 * znummax which is assume to be at least 
 * one (PRECONDITION!):
 *   znummax >= 1
 * Note that after every row of 
 * y's, a newline is added, so if
 *  - [*xmin, ..., *xmax] = [1, 2, 3],
 *  - [*ymin, ..., *ymax] = [1, 2, 3, 4, 5]
 *    and
 *  - znummax = 8,
 * the zstream has form
 *   [*, *, *, *, *, |, *, \]
 * where
 *  - *.tag = SOL or NOSOL,
 *  - |.tag = NEWLINE and
 *  - \.tag = END.
 * Hence from the (x,y)-grid
 *   [[(1,1), (1,2), (1,3), (1,4), (1,5)]
 *    [(2,1), (2,2), (2,3), (2,4), (2,5)]
 *    [(3,1), (3,2), (3,3), (3,4), (3,5)]],
 * only the first row and first element on
 * the second row get mapped by findz.
 *
 * RETURN: Minmaxpair struct containing the
 * minimum and maximum element of zstream. */
Minmaxpair xytozmap(struct zstreamelem zstream[], int znummax,
    double *xmin, double *xmax,
    double *ymin, double *ymax,
    struct zstreamelem (*findz)(double, double)) {
  int i;
  double *xi, *yi;
  struct zstreamelem newelem;
  Minmaxpair zminmax = {tagtoelem(NOSOL), tagtoelem(NOSOL)}; 
  
  i = 0;
  for (xi = xmin; xi <= xmax; xi++) {
    for (yi = ymin; yi <= ymax; yi++) {
      if (i >= znummax-1)
        break; /* array zstream full */
      
      newelem = (*findz)(*xi, *yi);
      if (elemcmp(newelem, zminmax.min) < 0)
        zminmax.min = newelem; /* new minimum */
      if (elemcmp(newelem, zminmax.max) > 0)
        zminmax.max = newelem; /* new maximum */
      zstream[i++] = newelem;
    }
    if (i >= znummax-1)
      break; /* array zstream full */

    /* add newline after finishing one
     * row of y's, if space left */
    zstream[i++] = tagtoelem(NEWLINE);
  }
  /* add end marker to very end */
  zstream[i] = tagtoelem(END);
  return zminmax;
}

/* Return:
 *  < 0 if elem1 < elem2
 *  = 0 if elem1 = elem2
 *  > 0 if elem1 > elem2
 * If both elements have zstreamtag = SOL,
 * the one with greater height is greater.
 * If one element is SOL and the other one
 * not, the SOL element is greater.
 * Elements with other zstreamtag than SOL
 * are equivalent. */
int elemcmp(struct zstreamelem elem1,
            struct zstreamelem elem2) {
  switch (elem1.tag) {
    case SOL:
      switch (elem2.tag) {
        case SOL:
          if (elem1.content.height < elem2.content.height)
            return -1;
          else if (elem1.content.height > elem2.content.height)
            return 1;
          return 0; 
        default:
          return 1;
      }
    default:
      switch (elem2.tag) {
        case SOL:
          return -1;
        default:
          return 0;
      }
  }
}

/* return type of function from double to
 * double -> double */
typedef double (*Curriedfunc)(double);

/* First, calculate the phi-coordinate in spherical
 * coordinates of a point given by the cartesian
 * coordinates x and y. Then calculate the 
 * theta-coordinate at that phi-value where function
 * theta->spherfunc(theta, phi)
 * has its root (zero value). Calculating the
 * theta-value is done using a 1D numerical root 
 * finder (rootfinder) which returns zstreamelem of
 * the form
 *  - { SOL, { theta-value } } if solution exists or
 *  - some other zstreamelem otherwise.
 * In case a solution exists, a cartesian z-coordinate
 * value corresponding to theta-value above point
 * (x, y) is calculated and
 *  - { SOL, { z-value} } is returned.
 * Otherwise
 *  - { NOSOL, { ' ' } } is returned. */
struct zstreamelem zfinder(double x, double y,
    double *thetamin, double *thetamax,
    struct zstreamelem (*rootfinder)(double *, double *, 
      double (*)(double), double), double precision,
    Curriedfunc (*spherfunc)(double)) {
  struct streamelem theta;
  double phi, r, cosphi;

  /* calculate phi using radius r in xy-plane */
  r = sqrt(pow(x,2) + pow(y,2));
  cosphi = x/r;
  phi = y/fabs(y) * acos(cosphi);

  /* calculate theta and corresponding height z */
  theta = rootfinder(thetamin, thetamax, 
      spherfunc(phi), precision);
  switch (theta.tag) {
    case SOL:
      /* solution exists -> can calculate z */
      theta.content.height = r * cos(theta.content.height);
      return theta;
    default:
      /* no solution */
      return tagtoelem(NOSOL);
  }
}
