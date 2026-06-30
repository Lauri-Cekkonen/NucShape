#include <stdio.h>
#include <math.h>

/* possible tags of the elements
 * in a z-value stream */ 
enum zstreamtag {
  SOL     = 01, /* solution exists */
  NOSOL   = 02, /* solution does not exist */
  NEWLINE = 04, /* newline when stream converted to
                   2D array */
  END     = 010 /* end of stream */
};

/* z-value stream element */
struct zstreamelem {
  zstreamtag tag;
  union {
    double heigth; /*      for SOL */
    char   c;      /* ' '  for NOSOL,
                      '\n' for NEWLINE,
                      '\0' for END */
  } content;
};


