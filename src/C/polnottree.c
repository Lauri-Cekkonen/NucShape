#include "polnottree.h"

/* Constructor that creates a
 * scalar-type polnottnode object.
 * When 'apply' function is used
 * on the created object, a real
 * number corresponding to 'num'
 * argument is returned.
 * 
 * NOTE: If the heap runs out of 
 * memory, polnottalloc cannot 
 * allocate memory to create the 
 * object and NULL is returned. */
struct polnottnode *scalar(
    double num)
{
  struct polnottnode *p = polnottalloc();
  if (p == NULL)
    return NULL; /* run out of memory */
  p->tag = SCALAR;
  p->content.scalar = num;
  return p;
}

/* Constructor that creates a
 * func-type polnottnode object.
 * When 'apply' function is used
 * on the created object, a real
 * number corresponding to
 * 'funcptr' applied on the
 * Coordpoint argument of 'apply'
 * is returned.
 * 
 * NOTE: If the heap runs out of 
 * memory, polnottalloc cannot 
 * allocate memory to create the 
 * object and NULL is returned. */
struct polnottnode *func(
    double (*funcptr)(Coordpoint))
{
  struct polnottnode *p = polnottalloc();
  if (p == NULL)
    return NULL; /* run out of memory */
  p->tag = FUNC;
  p->content.func = funcptr;
  return p;
}

/* Constructor that creates a
 * unary-type polnottnode object.
 * When 'apply' function is used
 * on the created object, the
 * unary operation corresponding
 * to 'operation' argument is
 * applied on the real number
 * corresponding to 'operand'
 * argument.
 *
 * NOTE: If the heap runs out of 
 * memory, polnottalloc cannot 
 * allocate memory to create the 
 * object and NULL is returned.
 *
 * NOTE: If 'operand' is NULL,
 * NULL is returned. Also if
 * 'operation' is not a
 * recognized operation (see
 * enum unaryoper in
 * polnottree.h), NULL is
 * returned. */
struct polnottnode *unary(
    enum unaryoper operation,
    struct polnottnode *operand)
{
  struct polnottnode *p;
  struct unarynode   *u;

  if (operand == NULL)
    return NULL; /* operating NULL
                    undefined */
  switch (operation) {
    case UNARYPLUS:
    case UNARYMINUS:
    case SIN: case COS:
    case EXP:
    case SQRT:
      u = unaryalloc();
      if (u == NULL)
        return NULL; /* run out of memory */
      u->flag = operation;
      u->next = operand;
      
      p = polnottalloc();
      if (p == NULL)
        return NULL; /* run out of memory */
      p->tag = UNARY;
      p->content.unary = u;
      return p;
    default:
      return NULL; /* unknown operation */
  }
}

/* Constructor that creates a
 * binary-type polnottnode object.
 * When 'apply' function is used
 * on the created object, the
 * binary operation corresponding
 * to 'operation' argument is
 * applied on the real numbers
 * corresponding to 'operand1'
 * and 'operand2' arguments.
 *
 * NOTE: If the heap runs out of 
 * memory, polnottalloc cannot 
 * allocate memory to create the 
 * object and NULL is returned.
 *
 * NOTE: If 'operand1' or
 * 'operand2' is NULL,
 * NULL is returned. Also if
 * 'operation' is not a
 * recognized operation (see
 * enum binaryoper in
 * polnottree.h), NULL is
 * returned. */
struct polnottnode *binary(
    struct polnottnode *operand1,
    enum binaryoper operation,
    struct polnottnode *operand2)
{
  struct polnottnode *p;
  struct binarynode  *b;

  if (operand1 == NULL || operand2 == NULL)
    return NULL; /* operating NULL
                    undefined */
  switch (operation) {
    case PLUS:
    case MINUS:
    case PROD:
    case DIV:
      b = binaryalloc();
      if (b == NULL)
        return NULL; /* run out of memory */
      b->flag = operation;
      b->nextleft = operand1;
      b->nextright = operand2;

      p = polnottalloc();
      if (p == NULL)
        return NULL; /* run out of memory */
      p->tag = BINARY;
      p->content.binary = b;
      return p;
    default:
      return NULL; /* unknown operation */
  }
}


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
enum applystatus apply(
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
enum applystatus applyunary(
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
enum applystatus applybinary(
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

