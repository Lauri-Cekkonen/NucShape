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

/* Contains 2 zstreamelem structs with
 * zstreamtag=SOL and their heights
 * correspond to minimum and maximum
 * z-value. */
typedef struct {
  struct zstreamelem min;
  struct zstreamelem max;
} Minmaxpair;

#define XMIN -1.0
#define XMAX 1.0
#define XNUM 30
#define YMIN -1.0
#define YMAX 1.0
#define YNUM 30
#define MAXGRID 5000

main() {
  double xvals[XNUM];
  double yvals[YNUM];
  struct zstreamelem zvals[MAXGRID];
  
  arrcreate(xvals, XNUM, XMIN, XMAX);
  arrcreate(yvals, YNUM, YMIN, YMAX);
}

/* Makes input double array arr into form
 * [min, min+d, min+2*d, ..., max]
 * with equal spacing
 * d = (max-min)/len. */
void arrcreate(double arr[], int len,
               double min, double max) {
  int i;
  double d = (max-min)/((double)len);
  arr[0] = min;
  for (i = 1; i < len; i++)
    arr[i] = arr[i-1] + d;
}

/* Use function findz to map points (x, y)
 * where x comes from array 
 * [*xmin, *(xmin+1), ..., *xmax]
 * and y comes from array
 * [*ymin, *(ymin+1), ..., *ymax].
 * Structs zstreamelem returned by findz
 * are stored in array zstream (side effect). 
 * Maximum amount of elements in zstream is
 * znummax.
 * RETURN: Minmaxpair struct containing the
 * minimum and maximum element of zstream.*/
Minmaxpair xytozmap(struct zstreamelem zstream[], int znummax,
              double *xmin, double *xmax,
              double *ymin, double *ymax,
              struct zstreamelem (*findz)(double, double)) {
  int i = 0;
  double *xi, *yi;
  Minmaxpair zminmax = {{NOSOL, {' '}}, {NOSOL, {' '}}}; 
  
  for (xi = xmin; xi <= xmax; xi++) {
    for (yi = ymin; yi <= ymin; yi++) {
      if (i >= znummax)
        return zminmax; /* array zstream full */
      
      newelem = (*findz)(*xi, *yi);
      if (elemcmp(newelem, zminmax.min) < 0)
        zminmax.min = newelem; /* new minimum */
      if (elemcmp(newelem, zminmax.max) > 0)
        zminmax.max = newelem; /* new maximum */
      zstream[i++] = newelem;
    }
    /* add newline after finishing one
     * row of y's */
    zstream[i].tag = NEWLINE;
    zstream[i].content.c = '\n';
    i++;
  }
  /* replace last newline by end marker */
  --i;
  zstream[i].tag = END;
  zstream[i].content.c = '\0';
  return zminmax;
}

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
          else if (elem1.content.height > elem2.content.height)
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

struct zstreamelem zfinder(double x, double y,
    double *thetamin, double *thetamax,
    struct zstreamelem (*numsolver)(double, double, 
      double (*)(double))
    double (*spherfunc)(double, double)) {
}
