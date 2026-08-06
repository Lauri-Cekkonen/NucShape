#include "coordinates.h"
#include <math.h>

/* Operator flag corresponding 
 * to the unary operation to be
 * applied to the next polnottnode 
 * object. */
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

/* The unary operation
 * corresponding to 'flag'
 * will be applied to the
 * polnottnode object pointed
 * to by 'next'. */  
struct unarynode {
  enum unaryoper flag;
  struct polnottnode *next;
};

/* Operator flag corresponding 
 * to the binary operation to be 
 * applied to the next left and
 * next right polnottnode 
 * objects. */
enum binaryoper {
  PLUS,  /* x + y */
  MINUS, /* x - y */
  PROD,  /* x * y */
  DIV    /* x / y */
};

/* The binary operation
 * corresponding to 'flag'
 * will be applied to the
 * polnottnode objects pointed
 * to by 'nextleft' and
 * 'nextright'. */ 
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
          /* Stores: */
  SCALAR, /* - scalar (double) */
  FUNC,   /* - function pointer */
  UNARY,  /* - unary operation tag 
               and a pointer to next
               polnottnode object */
  BINARY  /* - binary operation tag
               and a pointer to next
               left and next right
               polnottnode objects */
};

/* Type of the nodes in polnottree
 * ('Polish notation tree'). */
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
 * by the 'apply' function. */
enum applystatus {
  APPLY_OK = 0,
  DIV_BY_ZERO,  /* double is tried
                   to divide by
                   0.0 */
  SQRT_OF_NEG,  /* negative argument
                   for square root
                   (note: complex
                   numbers not supported
                   in the current
                   version) */
  UNKNOWN_OPER  /* operator flag
                   does not match
                   any flag defined in
                   enum applyoper */
};

static enum applystatus apply(
    const struct polnottnode *curr,
    Coordpoint arg,
    double *result)
{
  switch (curr->tag) {
    case
  }
};

static enum applystatus applyunary(
    const struct unarynode *curr,
    Coordpoint arg,
    double *result)
{
  enum applystatus status;
  double operand;

  switch (curr->flag) {
    case UNARYPLUS:
      return apply(curr->next, arg, result);
    case default:
      status = apply(curr->next, arg, &operand);
      if (status == APPLY_OK)
        switch (curr->flag) {
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
            return UNKNOWN_OPER;
        };
      return status;
  }
}

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
        return UNKNOWN_OPER;
    };
  return status2;
}

