#include <stdio.h>
#include <math.h>

/* possible tags of the elements
 * in a z-value stream */ 
enum zstreamtag {
  SOL     = 01, /* solution exists */
  NOSOL   = 02, /* solution does not exist */
  NEWLINE = 04, /* newline when stream converted
                   to string and printed */
  END     = 010 /* end of stream */
};

/* z-value stream element */
struct zstreamelem {
  enum zstreamtag tag;
  union {
    double height; /*      for SOL */
    char   c;      /* ' '  for NOSOL,
                      '\n' for NEWLINE,
                      '\0' for END */
  } content;
};

typedef struct {
  struct zstreamelem min;
  struct zstreamelem max;
} MinMaxPair;

/* Return:
 *  < 0 if elem1 < elem2
 *  = 0 if elem1 = elem2
 *  > 0 if elem1 > elem2
 * If both elements have zstreamtag = SOL,
 * the one with greater height is greater.
 * If one element is SOL and the other one
 * not, the SOL element is greater.
 * Elements with other zstreamtag than SOL
 * are equivalent. */
int elemcmp(struct zstreamelem elem1,
            struct zstreamelem elem2) {
  switch (elem1.tag) {
    case SOL:
      switch (elem2.tag) {
        case SOL:
          if (elem1.content.height < elem2.content.height)
            return -1;
          if (elem1.content.height > elem2.content.height)
            return 1;
          return 0; 
        default:
          return 1;
      }
    default:
      switch (elem2.tag) {
        case SOL:
          return -1;
        default:
          return 0;
      }
  }
}



/* Use function findz to map points (x, y)
 * where x comes from array 
 * [*xmin, *(xmin+1), ..., *xmax]
 * and y comes from array
 * [*ymin, *(ymin+1), ..., *ymax].
 * Structs zstreamelem returned by findz
 * are stored in array zstream (side effect). 
 * Maximum amount of elements in zstream is
 * znummax.*/
void xytozmap(struct zstreamelem zstream[], int znummax,
              double *xmin, double *xmax,
              double *ymin, double *ymax,
              struct zstreamelem (*findz)(double, double)) {
  int i = 0;
  double *xi, *yi;
  
  for (xi = xmin; xi <= xmax; xi++) {
    for (yi = ymin; yi <= ymin; yi++) {
      if (i >= znummax)
        return; /* array zstream full */
      zstream[i++] = (*findz)(*xi, *yi);
    }
    /* add newline after finishing one
     * row of y's */
    zstream[i].tag = NEWLINE;
    zstream[i].content.c = '\n';
    i++;
  }
  /* replace last newline by end */
  --i;
  zstream[i].tag = END;
  zstream[i].content.c = '\0';
}
