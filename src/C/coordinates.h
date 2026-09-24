#ifndef COORDINATES
#define COORDINATES

#include <math.h>

#define MAXAMOUNTOFALIASES 3

/* ====================
 * GENERAL INFORMATION:
 * ====================
 *
 * This file together with
 * 'coordinates.c' defines a 
 * "Coordpoint" type. Objects
 * of this type store components 
 * of some point in some
 * coordinate systems. Supported
 * coordinate systems are labelled
 * in the definition of 'enum
 * coordystem'.
 *
 * Coordpoint objects also store
 * aliases that are pointers to
 * the components of the same
 * object. Aliases make possible
 * to write functions and algorithms
 * that use Coordpoint objects
 * but don't rely on the components
 * of the Coordpoint object being
 * in a specific coordinate system. 
 * Instead they operate on the 
 * aliases which the user can set 
 * to point to components of any 
 * coordinate system. */


/* Possible tags for a Coordpoint
 * object. The tag states which
 * coordinate system the coordinate
 * components of the object
 * correspond to. 
 *
 * EXAMPLE: If a Coordpoint object 
 * stores values 1.2 and 5.3
 * corresponding to x- and y-components
 * in two-dimensional cartesian
 * coordinate system, then the 
 * 'coordsystem' tag of the object
 * is 'CARTESIAN_2D'. */
enum coordsystem {
  /* cartesian coordinate systems: */
  CARTESIAN_1D, 
  CARTESIAN_2D,
  CARTESIAN_3D, 
  /* spherical coordinate systems: */
  UNIT_SPHERICAL, /* 2D surface of unit
                     sphere */
  SPHERICAL,
  /* cylindrical coordinate systems: */
  POLAR, 
  CYLINDRICAL
};

/* Single bit flags for indicating
 * which aliases of a Coordpoint
 * object are valid pointers to
 * its components. Individual flags
 * can be combined to a mask that
 * indicates validity of a combination
 * of several aliases.
 *
 * EXAMPLE:
 *   ALIAS_1_VALID | ALIAS_3_VALID;
 * has integer value 1 + 4 = 5 or
 *    0001 (ALIAS_1_VALID)
 *   +0100 (ALIAS_3_VALID)
 *   -----
 *    0101 (ALIAS_1_VALID | ALIAS_3_VALID)
 * If 'flags' has type 'enum
 * coordaliasstatus', then
 *   if (flags & (ALIAS_2_VALID)) ...
 * is true if the bit corresponding
 * to 'ALIAS_2_VALID' (i.e. the
 * second bit because 'ALIAS_2_VALID'
 * is equivalent to '0010') is turned
 * on in 'flags'. */
enum coordaliasflag {
  EVERY_ALIAS_INVALID = 00,
  ALIAS_1_VALID       = 01,
  ALIAS_2_VALID       = 02,
  ALIAS_3_VALID       = 04
};

/* The integer values of the
 * 'coordaliasflag' rise as
 * powers of 2. Let's say
 * there is another set of
 * integer values corresponding
 * to them where the values
 * rise as addition by 1
 * (hence the other set is
 * more suitable for array
 * indices when shifted down
 * to start from 0). This 
 * function transforms that
 * integers from the other
 * set into the corresponding 
 * 'coordaliasflag'.
 * 
 * If the input integer is outside
 * the other set of integers
 * (signaling out of bounds error),
 * 'EVERY_ALIAS_INVALID' is returned
 * instead. */
static enum coordaliasflag _aliasflag(int i)
{
  switch (i) {
    case 1:
      return ALIAS_1_VALID;
    case 2:
      return ALIAS_2_VALID;
    case 3:
      return ALIAS_3_VALID;
    default:
      /* i out of bounds,
       * 'EVERY_ALIAS_INVALID'
       * repurposed to signal
       * that error */
      return EVERY_ALIAS_INVALID;
  }
}

/* Checks that the value of
 *   'MAXAMOUNTOFALIASES'
 * macro is consistent with the
 * definition of 'coordaliasflag'
 * and '_aliasflag'. For negative
 * answer, 0 is returned, and
 * for positive answer, some
 * other integer is returned.
 * 
 * If the consistency is checked
 * to be true with this function,
 * the function '_alisflag' can
 * be used to find out of bounds
 * errors before an integer is used
 * (after shifting, see the
 * documentation of '_aliasflag') 
 * as an index for array with
 * size 'MAXAMOUNTOFALIASES'. */
static int _aliasflagisconsistent()
{
  return _aliasflag(MAXAMOUNTOFALIASES) == ALIAS_3_VALID;
}

/* Container type for the coordinate
 * values of a point in one-, two-
 * and three-dimensional space. The
 * members are:
 *  - 'tag': signals which coordinate
 *    system the components appear in,
 *  - 'components': the component
 *    values are stored here as
 *    doubles and
 *  - 'aliases': references to values
 *    in 'components' are stored here.
 *
 * NOTE: If a coordinate system
 * corresponds to a subspace of a
 * bigger coordinate system, then
 * only the bigger coordinate
 * system is mentioned in 'components'.
 * Hence if 'obj' is a Coordpoint
 * object that stores components of a
 * point in two-dimensional Cartesian
 * coordinate system, then only
 * the members
 *   p.components.cartesian.x
 * and
 *   p.components.cartesian.y
 * contain a value. The component
 *   p.components.cartesian.z
 * of three-dimensional Cartesian
 * coordinate system can be left 
 * uninitialized. */ 
typedef struct {
  enum coordsystem tag;
  union {
    struct {
      double x;
      double y; /* not used in 1D */
      double z; /* not used in 1D
                   and 2D */
    } cartesian;
    struct {
      double r; /* not used on unit
                   sphere */
      double theta;
      double phi;
    } spherical;
    struct {
      double r;
      double theta;
      double z; /* not used in polar */
    } cylindrical;
  } components;
  struct {
    enum coordaliasflag flag;
    double *aliasarr[MAXAMOUNTOFALIASES];
  } aliases;
} Coordpoint;

/* Declarations for Coordpoint
 * constructors defined in
 * 'coordinates.c': */

Coordpoint gen3D(double x,
    double y, double z);

Coordpoint genunitspher(double theta,
    double phi); 

/* Return type for functions
 * that access and modify
 * aliases of Coordpoint
 * objects. Signals a success-
 * ful operation or an error
 * that occurred.
 *
 * NOTE: The name of
 *   'COORDINATES_H_INCONSISTENT'
 * refers to an inconsistency in
 * this header file, specificly 
 * to an inconsistency in the 
 * definition of the macro
 *   'MAXAMOUNTOFALIASES',
 * the enum 'coordaliasflag' and
 * the function '_aliasflag' as
 * desribed by the function
 * '_aliasflagisconsistent'. */
enum aliasaccessed {
  ALIAS_ACCESS_OK,
  ALIAS_INDEX_OUT_OF_BOUNDS,
  COORDINATES_H_INCONSISTENT,
  REQUESTED_ALIAS_UNVALID
};

/* Declarations for functions
 * that access and modify 
 * aliases of Coordpoint 
 * objects: */

enum aliasaccessed aliastoptr(
    Coordpoint *p,
    int i,
    double const *ptr);

enum aliasaccessed getalias(
    Coordpoint *p,
    int i,
    double *returnptr);

#endif COORDINATES
