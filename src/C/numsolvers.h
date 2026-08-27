#include "polnottree.h"

/* Status signal about the end
 * state of an equation solver
 * when the algorithm stopped. */
enum solverstate {
  SOLVED = 0, /* equation could
                 be solved */
  UNSOLVABLE, /* the algorithm
                 could run to
                 the end but
                 no satisfactory
                 solution was
                 found */
  BAD_ARG,    /* the algorithm
                 could not run
                 due to in-
                 sufficient
                 arguments */
  APPLY_ERROR /* error when
                 applying the
                 expression of
                 the equation
                 during algo-
                 rithm */
};

/* Declarations for equation
 * solvers: */

enum solverstatus bruteforceroot(
    const double *x1min, 
    const double *x1max,
    double *xi,
    const struct polnottree *expr,
    Coordpoint *restofcoord,
    double precision,
    enum applystatus *applyerror);
