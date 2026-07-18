#include <stdio.h>
#include <math.h>
#include "zstream.h"
#include "coordinates.h"
#include "numsolvers.h" /* TODO: this header
                           file */

#define XMIN -1.0
#define XMAX 1.0
#define XNUM 30
#define YMIN -1.0
#define YMAX 1.0
#define YNUM 30
#define MAXGRID 5000

main() 
{
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
               double min, double max) 
{
  int i;
  double d = (max-min)/((double)len);
  
  arr[0] = min;
  i = 0;
  while (i < len)
    arr[i++] += d;
}

/* Use function findz to map all points 
 * (x,y) where x comes from array 
 *   [*xmin, *(xmin+1), ..., *xmax]
 * and y comes from array
 *   [*ymin, *(ymin+1), ..., *ymax].
 * Each (x,y) is mapped by findz to an
 * zstreamelem struct which has
 *  - .tag=SOL if z-value was found by
 *    findz (solution exists)
 *  - .tag=EMPTY if no fitting
 *    z-value was found (no solution).  
 * In any case, the return value of
 * findz is inserted into the array
 * pointed by zstream until the end
 * of the array is reached (side effect).
 *
 * NOTE: Because the pointer zstream has
 * type Zstreamptr, the array it points
 * to ends with and zstreamelem of the form
 *   { END, { '\0' } }.
 * 
 * NOTE: A streamelem struct of the form
 *   { NEWLINE, { '\n' } }
 * is added to zstream array after each
 * row of y's (see the example below).
 *
 * For each z-value found by findz, it is
 * tested if it's smaller than the z-value
 * corresponding to *zmin or greater than
 * the z-value corresponding ot *zmin.
 * If yes, the *zmin or *zmax is replaced
 * by the zstreamelem object returned by
 * findz accordingly (side effect). 
 *
 * EXAMPLE: If
 *  - [*xmin, ..., *xmax] = [1, 2, 3],
 *  - [*ymin, ..., *ymax] = [1, 2, 3, 4, 5]
 * and the array pointed by zstream has 8
 * elements, the zstream is modified to form
 *   [*, *, *, *, *, |, *, \]
 * where
 *  - *.tag = SOL or EMPTY,
 *  - |.tag = NEWLINE and
 *  - \.tag = END.
 * Hence from the (x,y)-grid
 *   [[(1,1), (1,2), (1,3), (1,4), (1,5)]
 *    [(2,1), (2,2), (2,3), (2,4), (2,5)]
 *    [(3,1), (3,2), (3,3), (3,4), (3,5)]],
 * only the first row and first element on
 * the second row get mapped by findz. */
void xytozmap(Zstreamptr zstream,
    const double *xmin, const double *xmax,
    const double *ymin, const double *ymax,
    struct zstreamelem (*findz)(double, double)
    struct zstreamelem *zmin,
    struct zstreamelem *zmax) 
{
  double *xi, *yi;
  Zstreamptr zi;

  zi = zstream;
  for (xi = xmin; xi <= xmax; xi++) {
    for (yi = ymin; yi <= ymax; yi++) {
      if (zi->tag == END)
        return; /* array zstream full */

      *zi = (*findz)(*xi, *yi);
      if (zi->tag == SOL) {
        /* solution exists */
        if (elemcmp(*zi, *zmin) < 0)
          *zmin = *zi; /* new minimum */
        if (elemcmp(*zi, *zmax) > 0)
          *zmax = *zi; /* new maximum */
      }
      zi++;
    }
    if (zi->tag == END)
      return; /* array zstream full */
    
    /* add newline after finishing one
     * row of y's, if space left */
    *(zi++) = tagtononsol(NEWLINE);
  }
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
            struct zstreamelem elem2) 
{
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
 *  - { EMPTY, { ' ' } } is returned. */
struct zstreamelem zfinder(double x, double y,
    const double *thetamin, 
    const double *thetamax,
    struct zstreamelem (*rootfinder)(double *, double *, 
      double (*)(Coordpoint), 
      Coordpoint, double), 
    double precision,
    double (*spherfunc)(Coordpoint)) 
{
  struct streamelem theta;
  double phi, r, cosphi;
  Coordpoint p;

  /* calculate phi using radius r in xy-plane */
  r = sqrt(pow(x,2) + pow(y,2));
  cosphi = x/r;
  phi = y/fabs(y) * acos(cosphi);

  /* initialize Coordpoint p to hold
   * phi-value and having generic
   * coordinate x1 pointing to theta */
  p = unitsphergen(1.0 /* placeholder */, phi);
  p.x1 = &p.coord.unitspher.theta; /* p.x1 now
                                      points
                                      to theta */

  /* calculate theta and corresponding height z */
  theta = (*rootfinder)(thetamin, thetamax, 
      &spherfunc, &p, precision);
  switch (theta.tag) {
    case SOL:
      /* solution exists -> can calculate z 
       * (z is stored in the same zstreamelem
       * where theta was) */
      theta.content.height = r * cos(theta.content.height);
      return theta;
    default:
      /* no solution */
      return tagtononsol(EMPTY);
  }
}
