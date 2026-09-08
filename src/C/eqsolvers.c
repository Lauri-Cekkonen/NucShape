#include "eqsolvers.h"

/* Calculate numerically the root of 
 * function func, i.e. x1 satisfying
 *   func(x1) = 0.
 * This is done numerically by testing in
 * order each element x1 in an array
 *   [*x1min, *(x1min+1), ..., *x1max]
 * whether (*func)(x1) is closer to zero 
 * than the given 'precision' ('brute 
 * force'):
 *   abs((*func)(x1)) < precision
 * where 'abs' is absolute value.
 *
 * NOTE: The function bruteforceloop
 * does the main job of finding
 * the root and testing for errors.
 * This function just tests
 * for badly initalized arguments
 * and "throws" an appropriate
 * error when necessary.
 *  
 * NOTE: x1min and x1max refer to 
 * generic coordinate x1.
 *
 * NOTE: The function func(x1) is 
 * implemented as polnottnode struct 
 * pointer *expr. Function application
 * is achieved with 'apply' function
 * from polnottree.c as
 *   apply(expr, p, result);
 * where 'p' is a Coordpoint object
 * and 'result' points to a double
 * where the result of the application
 * is accumulated. p can even correspond 
 * to coordinates in multiple dimensions
 * (e.g. 3D cartesian coordinates) but 
 * the algorithm of finding the root is 
 * carried out along one-dimensional 
 * curve in the multi-dimensional space. 
 * This one-dimensional curve is in the
 * direction pointed to by
 * 'restofcoord->x1'. */
struct solverresult bruteforceroot(
    const double *x1min, /* lower bound */
    const double *x1max, /* upper bound */
    const struct polnottnode *expr, 
                         /* algorithm goal:
                            find roots of
                            'expr' */
    Coordpoint *restofcoord,
                         /* x1 member provides
                          * the algorithm a
                          * direction along
                          * which to find a
                          * root and other 
                          * members provide 
                          * the rest of the 
                          * coordinates */
    double precision)
{
  if (restofcoord->x1 == NULL)
    return othererrorresult(BAD_ARG); 
  return bruteforceloop(
        x1min, x1max,
        expr, restofcoord, precision)
}

/* Helper function for the
 * function 'bruteforceroot'.
 * See the documentation of
 * 'bruteforceroot' for more
 * information.
 *
 * NOTE: The last occurred
 * apply error is saved to
 * the return value only
 * if no precise enough 
 * solution was found. If 
 * an appropriate solution
 * was found, no information
 * about apply errors that
 * occurred before founding
 * the solution leaves this 
 * function.*/
struct solverresult bruteforceloop(
    const double *x1min,
    const double *x1max,
    const struct polnottnode *expr,
    Coordpoint *restofcoord,
    double precision)
{
  double exprval; /* stores the value of expr 
                     after application */
  const double *x1; /* loop index */
  enum applystatus currerror; 
  enum applystatus savederror;

  savederror = APPLY_OK; /* stays in this value
                            unless apply error
                            occurs during the
                            loop */
  for (x1 = x1min; x1 < x1max; x1++) {
    *(restofcoord->x1) = *x1;
    switch (currerror = apply(expr, 
          *restofcoord, 
          &exprval)) {
      case APPLY_OK:
        if (fabs(exprval) < precision) {
          /* precise enough root found */
          return solutionresult(*x1);
        }
        break;
      default:
        savederror = currerror;
        break;
    }
  }
  /* No sufficiently precise
   * root found. */
  switch (savederror) {
    case APPLY_OK:
      return othererrorresult(UNSOLVABLE);
    default:
      return applyerrorresult(savederror);
  }
}
