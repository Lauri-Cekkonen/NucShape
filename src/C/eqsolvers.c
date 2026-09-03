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
 * RETURN: In the following table,
 * 'found?' means 'an approximative
 * solution inside the precision
 * was found?'.
 *  
 *  found?|apply_error?|return
 *  ______|____________|__________
 *  yes   |no          |SOLVED
 *  yes   |yes         |SOLVED
 *  no    |no          |UNSOLVABLE
 *  no    |yes         |APPLY_ERROR
 *  
 * Note that if an approximative
 * solution is found, any apply
 * error found is ignored (the
 * found apply error is not
 * updated to 'applyerror'
 * argument as a side effect).
 * In general,  we ensure that 
 * applystatus pointed to by 
 * 'applyerror' has a valid
 * (non-garbage) value as a side
 * effect only  when 'bruteforceroot'
 * returns 'APPLY_ERROR'. *
 *
 * If an approximative solution
 * is found, the root is updated
 * to 'foundroot' argument as a
 * side effect.
 *
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
enum solverstatus bruteforceroot(
    const double *x1min, /* lower bound */
    const double *x1max, /* upper bound */
    double *foundroot,   /* return value
                            container */
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
    double precision,
    enum applystatus *applyerror
                         /* make encountered
                          * apply error
                          * accessible 
                          * outside this
                          * function */ )
{
  enum applystatus error;

  if (restofcoord->x1 == NULL)
    return BAD_ARG; /* algorithm does not
                       know along which
                       coordinate to find
                       roots -> the user
                       providing restofcoord
                       should've provided
                       this information 
                       by setting 
                       restofcoord->x1
                       before inserting
                       restofcoord into
                       this function as
                       an argument */
  error = APPLY_OK; /* stays in this value
                       unless apply error
                       occurs during the
                       loop */
  switch (bruteforceloop(
        x1min, x1max, foundroot,
        expr, restofcoord, precision,
        &error)) {
    case SOLVED:
      return SOLVED;
    case UNSOLVABLE:
      /* check if apply error occured */
      switch (error) {
        case APPLY_OK:
          /* neither a sufficiently
           * precise solution nor
           * an apply error found */
          return UNSOLVABLE;
        default:
          /* 'throw' the apply error that 
           * occured last in the loop */
          *applyerror = error;
          return APPLY_ERROR;
      }
  }
}

/* Helper function for 'bruteforceroot'.
 * See the documentation of
 * 'bruteforceroot' for more
 * information. */
enum solverstatus bruteforceloop(
    const double *x1min,
    const double *x1max,
    double *foundroot,
    const struct polnottnode *expr,
    Coordpoint *restofcoord,
    double precision,
    enum applystatus *applyerror)
{
  double exprval; /* stores the value
                     of expr */
  const double *x1; /* loop index */
  enum applystatus status; /* status at each
                              step in the
                              loop */
  enum applystatus error; /* container for
                             latest apply 
                             error during 
                             the loop */

  error = APPLY_OK; /* stays in this value
                       unless apply error
                       occurs during the
                       loop */
  for (x1 = x1min; x1 < x1max; x1++) {
    *(restofcoord->x1) = *x1; /* move forward
                                 along the line
                                 to a new point
                                 for the algo-
                                 rithm */
    switch (status = apply(expr, 
          *restofcoord, 
          &exprval)) {
      case APPLY_OK:
        if (fabs(exprval) < precision) {
          /* precise enough root found.
           * NOTE: 'applyerror' argument
           * is not updated. */
          *foundroot = *x1;
          return SOLVED; /* ignore previous
                            apply errors
                            if a root was
                            found */
        }
        break;
      default:
        error = status; /* store found
                           apply error */
        break;
    }
  }
  /* No sufficiently precise
   * root found. Just in case,
   * 'throw' the apply error 
   * that occured last in the
   * loop. */
  *applyerror = error; /* In the case of
                          no apply error,
                          this is just
                          APPLY_OK */
  return UNSOLVABLE;
}
