#include "coordinates.h"
#include <math.h>

enum unaryoper {
  UNARYPLUS,
  UNARYMINUS,
  SIN, COS, TAN,
  EXP
};

/* Declaration of polnottnode
 * for structs unarynode and
 * binarynode. */
struct polnottnode;

struct unarynode {
  enum unaryoper tag;
  struct polnottnode *next;
};

enum binarynode {
  PLUS,
  MINUS,
  PROD,
  DIV
};

struct binarynode {
  enum binaryoper tag;
  struct polnottnode *left;
  struct polnottnode *right;
};

enum nodetype {
  SCALAR,
  FUNC,
  UNARY,
  BINARY
};

struct polnottnode {
  enum nodetype tag;
  union {
    double scalar;
    double (*funcptr)(Coordpoint);
    struct unarynode *node1;
    struct binarynode *node2;
  } content;
};
