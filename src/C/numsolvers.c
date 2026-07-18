#include <math.h>
#include "zstream.h"
#include "coordinates.h"

/* Calculate numerically the root of function
 * func, i.e. x1 satisfying
 *   func(x1) = 0.
 * This is done numerically by testing in
 * order each element x1 in array
 *   [*x1min, *(x1min+1), ..., *x1max]
 * whether (*func)(x1) is closer to zero than
 * the given precision ('brute force'):
 *   abs((*func)(x1)) < precision
 * where 'abs' is absolute value. 
 * 
 * NOTE: x1, x1min and x1max refer to 
 * generic coordinate x1.
 *
 * NOTE: The function func(x1) is implemented
 * as (*func)(p) where p is a Coordpoint
 * object. p can even correspond to
 * coordinates in multiple dimensions
 * (e.g. spherical coordinates) but the
 * algorithm of finding the root is carried
 * out along the one-dimensional direction
 * in the direction along the coordinate
 * pointed to by p.x1.
 *
 * RETURN: If x1 satisfies the above condition,
 * return
 *   { SOL, { x1 } }
 * otherwise return
 *   { EMPTY, { ' ' } }. */
struct zstreamelem bruteforce(const double *x1min, 
    const double *x1max,
    double (*func)(Coordpoint),
    Coordpoint *allcoord,
    double precision) 
{
  double *x1; /* loop index */

  if (allcoord->x1 == NULL)
    fprintf(stderr, "bruteforce-function: "
        "x1 member in input Coordpoint "
        "object was not initialized.");
  for (x1 = x1min; x1 < x1max; x1++) {
    *(allcoord->x1) = *x1;
    if (fabs((*func)(*allcoord)) < precision)
      return { SOL, { *x1 } };
  }
  /* no solution found */
  return tagtononsol(EMPTY);
}

