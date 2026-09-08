#include "polnottree.h"

/* Status signal about the end
 * state of an equation solver
 * when the algorithm stopped.
 * 
 * NOTE: The signals are 
 * implemented as bit masks:
 *  SOLVED      = 0000
 *  UNSOLVED    = 0001
 *  BAD_ARG     = 0010
 *  UNSOLVABLE  = 0100
 *  APPLY_ERROR = 1000
 * Hence
 *  enum solverstatus flag = UNSOLVED;
 *  flag |= UNSOLVABLE;
 * makes 'flag' to have the
 * bit field 0101. Therefore, 
 *  if (flag == SOLVED) ...
 * is false,
 *  if (flag & UNSOLVED) ...
 * is true (not zero) and
 *  if (flag & BAD_ARG) ...
 * is false (zero). The last 
 * two cases can be seen
 * schematically in the
 * following manner:
 *   0101 (flag)
 * & 0001 (UNSOLVED)
 * ------
 *   0001 (true)
 * and
 *   0101 (flag)
 * & 0010 (BAD_ARG)
 * ------
 *   0000 (false) */
enum solverstatus {
  SOLVED     = 0,   /* equation could
                       be solved */
  UNSOLVED   = 01,  /* equation was
                       not solved.
                       The following
                       signals give
                       extra info: */
  BAD_ARG    = 02,  /* the algorithm
                       could not run
                       due to in-
                       sufficient
                       arguments */
  UNSOLVABLE = 04,  /* the algorithm
                       could run to
                       the end but
                       no satisfactory
                       solution was
                       found */
  APPLY_ERROR = 010 /* error when
                       applying the
                       expression of
                       the equation
                       during algo-
                       rithm */
};

/* Declarations for equation
 * solvers and their helper
 * functions: */

enum solverstatus bruteforceroot(
    const double *x1min, 
    const double *x1max,
    double *foundroot,
    const struct polnottnode *expr,
    Coordpoint *restofcoord,
    double precision,
    enum applystatus *applyerror);

enum solverstatus bruteforceloop(
    const double *x1min,
    const double *x1max,
    double *foundroot,
    const struct polnottnode *expr,
    Coordpoint *restofcoord,
    double precision,
    enum applystatus *applyerror);
