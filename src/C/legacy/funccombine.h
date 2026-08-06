#include "coordinates.h"

/* The operator to be applied
 * between the current
 * funclistnode and the next
 * funclistnode. */
enum applyoper {
  FINAL, /* final funclistnode
            in the linked list,
            no operation */
  SUM,   /* summation */
  MINUS, /* difference */
  PROD,  /* product */
  DIV    /* division */
};

/* Defines the type of an
 * element in linked list
 * of function pointers
 * and an operator to be
 * applied between the 
 * function pointers when
 * they are combined into
 * one function call. 
 *
 * NOTE: The final element
 * in the linked list has
 *   operation=FINAL
 * and
 *   next=NULL.
 * If operation is something
 * else than FINAL, next
 * cannot be NULL.*/
struct funclistnode {
  double (*funcptr)(Coordpoint);
  enum applyoper operation;
  struct funclistnode *next;
};

/* Status signal to be returned
 * by the apply function. */
enum applystatus {
  APPLYOK = 0,
  DIVBYZERO,  /* double is tried
                 to divide by
                 0.0 */
  UNKNOWNOPER /* Operator flag
                 does not match
                 any flag in
                 enum applyoper */
};

/* Given a pointer curr to beginning 
 * of a linked list of funclistnode's:
 *   *curr
 *     funcptr
 *     operation
 *     next      ->  *next
 *                     funcptr
 *                     operation
 *                     next      -> ...
 * and a Coordpoint argument arg,
 * cumulates the following into
 * double pointed to by result:
 *   (*curr->funcptr)(arg)
 *   [curr->operation]
 *   (*next->funcptr)(arg)
 *   [next->operation]
 *   ...
 * where [...->operation] is the
 * binary operation on double
 * corresponding to operation (e.g.
 * SUM corresponds to '+').
 *
 * NOTE: apply goes through the linked
 * list recursively. The recursing
 * stops when the final element in the
 * linked list (signaled by
 * operation=FINAL) is reached.
 * 
 * RETURN: Status code signaling either
 * success at all stages of recursion
 * (APPLYOK) or an error at some point
 * of recursion (DIVBYZERO or
 * UNKNOWNOPER). */
static enum applystatus apply(struct funclistnode *curr,
    Coordpoint arg, double *result) 
{
  double operand1, operand2;
  enum applystatus status;

  operand1 = (*curr->funcptr)(arg);
  switch (curr->operation) {
    case FINAL:
      *result = operand1;
      return APPLYOK;
    default:
      /* note that the following recursive
       * call is evaluated even if 
       * curr->operation corresponds to
       * invalid operation (causes
       * UNKNOWNOPER to be returned) */
      status = apply(curr->next, arg, &operand2);
      if (status != APPLYOK)
        return status;
      switch (curr->operation) {
        /* case FINAL catched above */
        case SUM:
          *result = operand1+operand2;
          return APPLYOK;
        case MINUS:
          *result = operand1-operand2;
          return APPLYOK;
        case PROD:
          *result = operand1*operand2;
          return APPLYOK;
        case DIV:
          if (operand2 == 0.0)
            return DIVBYZERO;
          *result = operand1/operand2;
          return APPLYOK;
        default:
          return UNKNOWNOPER;
      }
  }
}
