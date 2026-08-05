#include "coordinates.h"
#include <math.h>

/* Operator flag corresponding 
 * to the unary operation to be
 * applied to the next polnottnode 
 * object. */
enum unaryoper {
  UNARYPLUS,     /* +x */
  UNARYMINUS,    /* -x */
  SIN, COS, TAN, /* trigonometric
                    functions */
  EXP,           /* exponential
                    function */
  SQRT           /* square root */
};

/* Operator flag corresponding 
 * to the binary operation to be 
 * applied to the next left and
 * next right polnottnode 
 * objects. */
enum binarynode {
  PLUS,  /* x + y */
  MINUS, /* x - y */
  PROD,  /* x * y */
  DIV    /* x / y */
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
    double (*funcptr)(Coordpoint);
    struct {
      /* The unary operation
       * corresponding to 'flag'
       * will be applied to the
       * polnottnode object pointed
       * to by 'next'. */
      enum unaryoper flag;
      struct polnottnode *next;
    } unarynode;
    struct {
      /* The binary operation
       * corresponding to 'flag'
       * will be applied to the
       * polnottnode objects pointed
       * to by 'nextleft' and
       * 'nextright'. */
      enum binaryoper flag;
      struct polnottnode *nextleft;
      struct polnottnode *nextright;
    } binarynode;
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
