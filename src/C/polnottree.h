#include "coordinates.h"
#include <math.h>

/* Unary operator flag. */
enum unaryoper {
  UNARYPLUS,     /* +x */
  UNARYMINUS,    /* -x */
  SIN, COS,      /* trigonometric
                    functions */
  EXP,           /* exponential
                    function */
  SQRT           /* square root */
};

/* Declaration of polnottnode
 * for definitions of unarynode 
 * and binarynode. */
struct polnottnode; 


/* Type of a 'subnode'
 * of Polish notation tree
 * node (polnottnode)
 * corresponding to unary
 * operation. */ 
struct unarynode {
  enum unaryoper flag;
  struct polnottnode *next;
};

/* Unary operator flag. */
enum binaryoper {
  PLUS,  /* x + y */
  MINUS, /* x - y */
  PROD,  /* x * y */
  DIV    /* x / y */
};

/* Type of 'subnode'
 * of Polish notation tree
 * node (polnottnode)
 * corresponding to binary
 * operation. */ 
struct binarynode {
  enum binaryoper flag;
  struct polnottnode *nextleft;
  struct polnottnode *nextright;
};

/* Type tag for polnottnode type.
 * The tag corresponds to what
 * is stored inside the content of
 * polnottnode object. */
enum nodetype {
          /* polnottnode stores: */
  SCALAR, /* - scalar (double) */
  FUNC,   /* - function pointer */
  UNARY,  /* - unarynode pointer */
  BINARY  /* - binarynode pointer */
};

/* Type of the nodes in Polish 
 * notation tree ('polnottree'). */
struct polnottnode {
  enum nodetype tag;
  union {
    double scalar;
    double (*func)(Coordpoint);
    struct unarynode *unary;
    struct binarynode *binary;
  } content;
};

/* Status signal to be returned
 * by different apply function. */
enum applystatus {
  APPLY_OK = 0,
  DIV_BY_ZERO,     /* double is tried
                      to divide by
                      0.0 */
  SQRT_OF_NEG,     /* negative argument
                      for square root
                      (note: complex
                      numbers not supported
                      in the current
                      version) */
  UNKNOWN_UNARY,   /* operator flag
                      does not match
                      any flag defined in
                      enum unaryoper */
  UNKNOWN_BINARY,  /* operator flag
                      does not match
                      any flag defined in
                      enum binaryoper */
  UNKNOWN_POLNOT,  /* type tag does
                      not match any
                      tag defined in
                      enum nodetype */ 
};

/* Apply Polish notation tree
 * starting from 'curr' on
 * 'arg'. 'curr' points to
 * a polnottnode object
 * and depending on that
 * object the double pointed
 * to by 'result' is
 * cumulated (side effect)
 * or functions applyunary or 
 * applybinary are called. 
 * Functions applyunary and
 * applybinary can then further
 * call function 'apply', 
 * achieving recursivity.
 *
 * RETURN: A status signal of
 * type enum applystatus.
 * Status signals other than
 * APPLY_OK work like
 * exceptions that 'apply'
 * throws at the caller of
 * 'apply'.
 *
 * NOTE: 'curr' does not have
 * to be at the top of the
 * Polish notation tree
 * (recursivity).
 *
 * NOTE: If status signal other
 * than APPLY_OK is returned,
 * the double pointed to by
 * 'result' might not have
 * a valid value. */
static enum applystatus apply(
    const struct polnottnode *curr,
    Coordpoint arg,
    double *result)
{
  switch (curr->tag) {
    case SCALAR:
      *result = curr->content.scalar;
      return APPLY_OK;
    case FUNC:
      *result = (curr->content.func)(arg);
      return APPLY_OK;
    case UNARY:
      return applyunary(curr->unary, arg, result);
    case BINARY:
      return applybinary(curr->binary, arg, result);
    default:
      return UNKNOWN_POLNOT;
  }
}

/* Apply unary operation
 * corresponding to
 * 'curr->flag' on the result
 * of applying 'curr->next'
 * on 'arg' with apply 
 * function. Then cumulate
 * the result in double 
 * pointed to by 'result'
 * (side effect).
 * 
 * RETURN: A status signal of
 * type enum applystatus.
 * Status signals other than
 * APPLY_OK work like
 * exceptions that 'applyunary'
 * throws at the caller of
 * 'applyunary'.
 * 
 * NOTE: If status signal other
 * than APPLY_OK is returned,
 * the double pointed to by
 * 'result' might not have
 * a valid value. */
static enum applystatus applyunary(
    const struct unarynode *curr,
    Coordpoint arg,
    double *result)
{
  enum applystatus status;
  double operand;

  status = apply(curr->next, arg, &operand);
  if (status == APPLY_OK)
    switch (curr->flag) {
      case UNARYPLUS:
        *result = +operand;
        break;
      case UNARYMINUS:
        *result = -operand;
        break;
      case SIN:
        *result = sin(operand);
        break;
      case COS:
        *result = cos(operand);
        break;
      case EXP:
        *result = exp(operand);
        break;
      case SQRT:
        if (operand < 0)
          return SQRT_OF_NEG;
        *result = sqrt(operand);
        break;
      default:
        return UNKNOWN_UNARY;
      };
  return status;
}

/* Apply binary operation
 * corresponding to
 * 'curr->flag' on the result
 * of applying 'curr->nextleft'
 * and 'curr->nextright'
 * on 'arg' with apply 
 * function (two operands for
 * the binary function). Then 
 * cumulate the result in 
 * double pointed to by 
 * 'result' (side effect).
 * 
 * RETURN: A status signal of
 * type enum applystatus.
 * Status signals other than
 * APPLY_OK work like
 * exceptions that 'applybinary'
 * throws at the caller of
 * 'applybinary'.
 *
 * NOTE: Since the apply function
 * is called twice, there are
 * two status signals 
 * corresponding to the two 
 * operands of the binary 
 * operation. If first call to
 * apply produces a status signal
 * other than APPLY_OK, that
 * status signal is returned.
 * Otherwise the status signal
 * of the latter call to apply
 * is preferred.
 * 
 * NOTE: If status signal other
 * than APPLY_OK is returned,
 * the double pointed to by
 * 'result' might not have
 * a valid value. */
static enum applystatus applybinary(
    const struct binarynode *curr,
    Coordpoint arg,
    double *result)
{
  enum applystatus status1, status2;
  double operand1, operand2;

  status1 = apply(curr->nextleft, arg, &operand1);
  if (status1 != APPLY_OK)
    return status1; /* 'throw' left
                       operand error */
  status2 = apply(curr->nextright, arg, &operand2);
  if (status2 == APPLY_OK)
    switch (curr->flag) {
      case PLUS:
        *result = operand1 + operand2;
        break;
      case MINUS:
        *result = operand1 - operand2;
        break;
      case PROD:
        *result = operand1 * operand2;
        break;
      case DIV:
        if (operand2 == 0.0)
          return DIV_BY_ZERO;
        *result = operand1 / operand2;
        break;
      default:
        return UNKNOWN_BINARY;
    };
  return status2;
}

