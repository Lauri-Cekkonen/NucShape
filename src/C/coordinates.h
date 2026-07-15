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
 * stored in 'coord' member via
 * pointers x1, x2 and x3 which are
 * set to point to the coordinate
 * values. For example, a user could
 * set
 *   p.x1 = &p.coord.cart2D.y;
 * and input the Coordpoint object p
 * into the function/algorithm after
 * which
 *   p->x1
 * gives an access to the value
 *   p.coord.cart2D.y
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
    } cart1D; /* tag=CART1D */
    struct {
      double x;
      double y;
    } cart2D; /* tag=CART2D */
    struct {
      double x;
      double y;
      double z;
    } cart3D; /* tag=CART3D */
    struct {
      double theta;
      double phi;
    } unitspher; /* tag=UNITSPHER */
    struct {
      double r;
      double theta;
      double phi;
    } spher; /* tag=SPHER */
    struct {
      double r;
      double theta;
    } polar; /* tag=POLAR */
    struct {
      double r;
      double theta;
      double z;
    } cylind; /* tag=CYLIND */
  } coord;
} Coordpoint;

/* Initialize a Coordpoint object
 * corresponding to cartesian
 * coordinates in three dimensions. */
static struct coordpoint cart3Dgen(double x,
    double y, double z) {
  struct coordpoint point;
  point.tag = CART3D;
  point.coord.cart3D.x = x;
  point.coord.cart3D.y = y;
  point.coord.cart3D.z = z;
  /* initialize pointers to null */
  point.x1 = NULL;
  point.x2 = NULL;
  point.x3 = NULL;
  return point;
}

/* Initialize a Coordpoint object
 * corresponding to spherical
 * coordinates on the unit sphere
 * (r=1). */
static struct coordpoint unitsphergen(double theta,
    double phi) {
  struct coordpoint point;
  point.tag = UNITSPHER;
  point.coord.unitspher.theta = theta;
  point.coord.unitspher.phi   = phi;
  /* initialize pointers to null */
  point.x1 = NULL;
  point.x2 = NULL;
  point.x3 = NULL;
  return point;
}

