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

/* Allocate memory for a
 * polnottnode object in
 * the heap memory and
 * return a pointer to
 * the allocated object. */
static struct polnottnode *polnottalloc(void)
{
  return (struct polnottnode *) malloc(
      sizeof(struct polnottnode));
}

/* Allocate memory for a
 * unarynode object in
 * the heap memory and
 * return a pointer to
 * the allocated object. */
static struct unarynode *unaryalloc(void)
{
  return (struct unarynode *) malloc(
      sizeof(struct unarynode));
}

/* Allocate memory for a
 * binarynode object in
 * the heap memory and
 * return a pointer to
 * the allocated object. */
static struct binarynode *binaryalloc(void)
{
  return (struct binarynode *) malloc(
      sizeof(struct binarynode));
}

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

/* Declarations of functions
 * defined in polnottree.c: */

enum applystatus apply(
    const struct polnottnode *curr,
    Coordpoint arg,
    double *result);

enum applystatus applyunary(
    const struct unarynode *curr,
    Coordpoint arg,
    double *result);

enum applystatus applybinary(
    const struct binarynode *curr,
    Coordpoint arg,
    double *result);
