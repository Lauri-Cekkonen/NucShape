#ifndef COORDINATES
#define COORDINATES

#include <math.h>

/* Possible tags for a Coordpoint
 * object. */
enum coordsystem {
  CART1D, CART2D, CART3D, /* cartesian
                             coordinates */
  UNITSPHER, SPHER, /* spherical
                       coordinates */
  POLAR, CYLIND /* polar and cylindrical
                   coordinates */
};

/* Container type for the coordinate
 * values of a point in one-, two-
 * and three-dimensional space. The
 * member 'tag' signals which
 * coordinate system is being used
 * and member 'coord' stores the
 * corresponding coordinate values.
 * 
 * NOTE: Coordpoint object can be
 * used in a function or algorithm
 * that is written in terms of
 * generic coordinates {x1} (1D),
 * {x1,x2} (2D) or {x1,x2,x3} (3D).
 * In that case the function/algorithm
 * accesses the coordinate values
 * stored in 'coordinates' member via
 * pointers x1, x2 and x3 which are
 * set to point to the coordinate
 * values. For example, a user could
 * set
 *   double *x;
 *   if (xptr(p, x) == PROJECTION_OK)
 *     p.x1 = x;
 * and input the Coordpoint object p
 * into the function/algorithm after
 * which
 *   *(p.x1)
 * gives an access to the value
 *   p.coordinates.cartesian.x
 * inside the function/algorithm.
 *
 * NOTE: When initializing a Coordpoint
 * object, the pointers x1, x2 and x3
 * have to be explicitly set to NULL.
 * Otherwise a function or algorithm
 * using generic coordinates via
 * pointers x1, x2 and x3 might not 
 * catch correctly the error of 
 * uninitialized pointers x1, x2 or
 * x3 */
typedef struct {
  enum coordsystem tag;
  double *x1;
  double *x2;
  double *x3;
  union {
    struct {
      double x;
      double y; /* not used for:
                   tag=CART1D */
      double z; /* not used for:
                   tag=CART2D
                   tag=CART1D */
    } cartesian;
    struct {
      double r; /* not used for:
                   tag=UNITSPHER */
      double theta;
      double phi;
    } spherical;
    struct {
      double r;
      double theta;
      double z; /* not used for:
                   tag=POLAR */
    } cylindrical;
  } coordinates;
} Coordpoint;

/* Declarations for Coordpoint
 * constructors defined in
 * coordinates.c: */

Coordpoint gen3D(double x,
    double y, double z);

Coordpoint genunitspher(double theta,
    double phi); 

/* Status signal to be returned
 * by projection functions that
 * sets a double object to a
 * coordinate value of an input
 * point or a double pointer 
 * to point to a coordinate 
 * value of an input point. */
enum projectionstatus {
  PROJECTION_OK = 0,
  WRONG_COORD_SYS, /* the coord system
                      of input point
                      lacks info
                      to perform
                      requested
                      projection */ 
  UNKNOWN_TAG      /* input point
                      is of unknown
                      coord system
                      (updated
                      Coordpoint,
                      outdated
                      projection) */
};

/* Declarations for projection
 * functions defined in
 * coordinates.c: */

enum projectionstatus xval(
    Coordpoint p,
    double *proj);

enum projectionstatus xptr(
    Coordpoint p,
    double *proj);

#endif COORDINATES
