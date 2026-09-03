#include "coordinates.h"

/* Example Coordpoint constructors: */

/* Initialize a Coordpoint object
 * corresponding to cartesian
 * coordinates in three dimensions. */
Coordpoint gen3D(double x,
    double y, double z) 
{
  Coordpoint p;
  p.tag = CART3D;
  p.coordinates.cartesian.x = x;
  p.coordinates.cartesian.y = y;
  p.coordinates.cartesian.z = z;
  /* initialize pointers to null */
  p.x1 = NULL;
  p.x2 = NULL;
  p.x3 = NULL;
  return point;
}

/* Initialize a Coordpoint object
 * corresponding to spherical
 * coordinates on the unit sphere
 * (r=1). */
Coordpoint genunitspher(double theta,
    double phi) 
{
  Coordpoint p;
  p.tag = UNITSPHER;
  p.coordinates.spherical.theta = theta;
  p.coordinates.spherical.phi   = phi;
  /* initialize pointers to null */
  p.x1 = NULL;
  p.x2 = NULL;
  p.x3 = NULL;
  return point;
}

/* Example projection functions: */

/* Update the double pointed to
 * by 'proj' to the x-coordinate
 * value of 'p' (side effect). 
 * If the coordinate system of
 * 'p' is not cartesian,
 * calculate the corresponding
 * x coordinate value in
 * cartesian coordinate 
 * system. */
enum projectionstatus xval(
    Coordpoint p,
    double *proj)
{
  switch (p.tag) {
    case CART1D: case CART2D:
    case CART3D:
      *proj = p.coordinates.cartesian.x;
      return PROJECTION_OK;
    case UNITSPHER:
      *proj = sin(p.coordinates.spherical.theta)*
              cos(p.coordinates.spherical.phi);
      return PROJECTION_OK;
    case UNITSPHER:
      *proj = p.coordinates.spherical.r*
          sin(p.coordinates.spherical.theta)*
          cos(p.coordinates.spherical.phi);
      return PROJECTION_OK;
    case POLAR: case CYLIND:
      *proj = p.coordinates.cylindrical.r*
          cos(p.coordinates.cylindrical.theta);
      return PROJECTION_OK;
    default:
      /* This function is outdated
       * and does not include
       * the tag of p as non-default
       * case */
      return UNKNOWN_TAG;
  }
}

/* Set 'proj' to point to
 * the x-coordinate value
 * of Coordpoint 'p'. This
 * requires 'p' to be in
 * cartesian coordinate
 * system. */
enum projectionstatus xptr(
    Coordpoint p,
    double *proj)
{
  switch (p.tag) {
    case CART1D: case CART2D:
    case CART3D:
      proj = &p.coordinates.cartesian.x;
      return PROJECTION_OK;
    case UNITSPHER: case SPHER:
    case POLAR: case CYLIND:
      /* coordinate system not
       * cartesian */
      return WRONG_COORD_SYSTEM;
    default:
      /* This function is outdated
       * and does not include
       * the tag of p as non-default
       * case */
      return UNKNOWN_TAG;
  }
}
