#include "numsolvers.h"

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
 * NOTE: x1, x1min and x1max refer to 
 * generic coordinate x1.
 *
 * NOTE: The function func(x1) is 
 * implemented as polnottree struct 
 * pointer *expr. Function application
 * is achieved with 'apply' function
 * from polnottree.c as
 *   apply(expr, p, result);
 * where 'p' is a Coordpoint object
 * and 'result' points to a double
 * where the result of the application
 * is accumulated. p can even correspond 
 * to coordinates in multiple dimensions
 * (e.g. spherical coordinates) but the
 * algorithm of finding the root is 
 * carried out along one-dimensional 
 * curve in the multi-dimensional space. 
 * This one-dimensional curve is in the
 * direction pointed to by
 * 'restofcoord->x1'. 
 *
 * NOTE: We only ensure that applystatus
 * pointed to by 'applyerror' has a valid
 * (non-garbage) value when 'bruteforceroot' 
 * returns 'APPLY_ERROR'. */
enum solverstatus bruteforceroot(
    const double *x1min, /* lower bound */
    const double *x1max, /* upper bound */
    double *xi,          /* loop index and
                            return value
                            container */
    const struct polnottree *expr, 
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
    double precision,
    enum applystatus *applyerror
                         /* make encountered
                          * applystatus
                          * signaling an
                          * error
                          * accessible */ )
{
  double exprval;
  enum applystatus status;

  if (restofcoord->x1 == NULL)
    return BAD_ARG; /* algorithm does not
                       know along which
                       coordinate to find
                       roots */
  for (xi = x1min; xi < x1max; xi++) {
    *(restofcoord->x1) = *xi; /* move forward
                                 along the line
                                 to a new point
                                 for the algo-
                                 rithm */
    switch (status = apply(expr, 
          restofcoord, 
          &exprval)) {
      case APPLY_OK:
        /* note: no assignment to
         * applyerror */
        if (fabs(exprval) < precision)
          return SOLVED; /* solution found
                            at xi */
        break;
      default:
        *applyerror = status; /* "throw" apply 
                                 error */
        return APPLY_ERROR;
    }
  }
  /* no solution found */
  return UNSOLVABLE;
}

