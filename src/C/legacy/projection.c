#include "coordinates.h"
#include "projection.h"

/* NOTE: 'coordinates.h' needs
 * to be in the same folder
 * as this file when compiling */

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
    case CARTESIAN_1D: 
    case CARTESIAN_2D:
    case CARTESIAN_3D:
      *proj = p.components.cartesian.x;
      return PROJECTION_OK;
    case UNIT_SPHERICAL:
      *proj = sin(p.components.spherical.theta)*
              cos(p.components.spherical.phi);
      return PROJECTION_OK;
    case SPHERICAL:
      *proj = p.components.spherical.r*
          sin(p.components.spherical.theta)*
          cos(p.components.spherical.phi);
      return PROJECTION_OK;
    case POLAR: case CYLINDRICAL:
      *proj = p.components.cylindrical.r*
          cos(p.components.cylindrical.theta);
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
    case CARTESIAN_1D: 
    case CARTESIAN_2D:
    case CARTESIAN_3D:
      proj = &p.coordinates.cartesian.x;
      return PROJECTION_OK;
    case UNIT_SPHERICAL: 
    case SPHERICAL:
    case POLAR: 
    case CYLINDRICAL:
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
