#include <math.h>
#include "zstream.h"
#include "composition.h"

/* Calculate numerically the root of function
 * func, i.e. x satisfying
 *   func(x) = 0.
 * This is done numerically by testing in
 * order each element x in array
 *   [*xmin, *(xmin+1), ..., *xmax]
 * whether (*func)(x) is closer to zero than
 * the given precision ("brute force"):
 *   abs((*func)(x)) < precision
 * where "abs" is absolute value. 
 * 
 * NOTE: x, xmin and xmax refer to the
 * coordinate of general 1D space. They
 * don't necessarily correspond to the 
 * coordinate x of spherical coordinates
 * (in the application of asciimap.c,
 * they correspond to coordinate theta).
 *
 * RETURN: If x satisfies the above condition,
 * return
 *   { SOL, { x } }
 * otherwise return
 *   { EMPTY, { ' ' } }. */
struct zstreamelem bruteforce(double *xmin, double *xmax,
    double (*func)(double),
    double precision) {
  double *xi;

  for (xi = xmin; xi < xmax; xi++) {
    if (fabs((*func)(*xi)) < precision)
      return { SOL, { *xi } };
  }
  /* no solution found */
  return tagtononsol(EMPTY);
}

/* Helper struct for function composition.
 * In this file used for:
 *   (*comp.f) --> fix2ndarg */
static struct compdouble2D comp;

/* Makes the function
 *   double (*comp.f)(double arg1, double arg2);
 * into function
 *   double h(double arg1);
 * by fixing the second argument of (*comp.f)
 * as the value comp.param. */
static double fix2ndarg(double arg) {
  return (*(comp.f))(arg, comp.param);
}

/* Solve roots of the function pointed to by
 * func along a 1D path given by fix2ndarg
 * (external) and param. The algorithm 
 * bruteforce is used for solving the roots 
 * numerically. */
struct zstreamelem bruteforce2D(double *xmin, double *xmax,
    double (*func)(double, double),
    double param,
    double precision) {
  comp.f     = func;
  comp.param = param;
  return bruteforce(xmin, xmax
      &fix2ndarg,
      precision);
}
