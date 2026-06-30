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
  zstreamtag tag;
  union {
    double heigth; /*      for SOL */
    char   c;      /* ' '  for NOSOL,
                      '\n' for NEWLINE,
                      '\0' for END */
  } content;
};

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
    zstream[i++] = { NEWLINE, { '\n' } };   
  }
  /* replace last newline by end */
  zstream[--i] = { END, { '\0' } };
}
