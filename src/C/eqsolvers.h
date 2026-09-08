#include "polnottree.h"

/* Status signal about the end
 * state of an equation solver
 * when the algorithm stopped. */
enum solverstatus {
  SOLVED = 0, /* equation could
                 be solved */
  BAD_ARG,    /* the algorithm
                 could not run
                 due to in-
                 sufficient
                 arguments */
  UNSOLVABLE, /* the algorithm
                 could run to
                 the end but
                 no satisfactory
                 solution was
                 found */
  APPLY_ERROR /* error when
                 applying the
                 expression of
                 the equation
                 during algo-
                 rithm */
};

/* Equation solver return
 * value type */
struct solverresult {
  enum solverstatus flag;
  union {
    double solution;        /* flag=SOLVED */
    enum applystatus error; /* flag=APPLY_ERROR */
  } content;
}

/* Constructors for solverresult */

/* Construct solverresult in case
 * of found solution */
static struct solverresult solutionresult(
    double solution)
{
  struct solverresult result;
  result.flag = SOLVED;
  result.content.solution = solution;
  return result;
}

/* Construct solverresult in case
 * no precise enough solution
 * was found and an apply error
 * occurred. */ 
static struct solverresult applyerrorresult(
    enum applystatus applyerror)
{
  struct solverresult result;
  result.flag = APPLY_ERROR;
  result.content.error = applyerror;
  return result;
}

/* Construct solverresult in case
 * no precise enough solution
 * was found or an error other
 * than apply error occurred. 
 *
 * NOTE: This constructor leaves
 * 'content' of solverresult
 * uninitialized. */ 
static struct solverresult othererrorresult(
    enum solverstatus errorstate)
{
  struct solverresult result;
  result.flag = errorstate;
  return result;
}

/* Getter functions for
 * solverresult type */

static enum solverstatus getsolverresultflag(
    struct solverresult result)
{
  return result.flag;
}

static int getsolution(
    struct solverresult result,
    double *solution)
{
  if (result.flag == SOLVED) {
    *solution = result.content.solution;
    return 1;
  } else {
    return 0;
  }
}

static int getapplyerror(
    struct solverresult result,
    enum applystatus *applyerror)
{
  if (result.flag & APPLY_ERROR) {
    *applyerror = result.content.error;
    return 1;
  } else {
    return 0;
  }
}

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
