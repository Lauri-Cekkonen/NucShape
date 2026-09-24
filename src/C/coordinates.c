#include "coordinates.h"

/* Example Coordpoint constructors:
 *
 * NOTE: Consctructors of Coordpoint
 * leave by design the 'enum
 * coordaliasstatus' member of the
 * constructed Coordpoint object as
 * 'EVERY_ALIAS_INVALID'. Other
 * a component and update the
 * 'enum coordaliasstatus' member
 * accordingly. */

/* Initialize a Coordpoint object
 * corresponding to cartesian
 * coordinates in three dimensions. */
Coordpoint gen3D(double x,
    double y, double z) 
{
  Coordpoint p;
  p.tag = CARTESIAN_3D;
  p.components.cartesian.x = x;
  p.components.cartesian.y = y;
  p.components.cartesian.z = z;
  p.aliases.flag = EVERY_ALIAS_INVALID; 
  return p;
}

/* Initialize a Coordpoint object
 * corresponding to spherical
 * coordinates on the unit sphere
 * (r=1). */
Coordpoint genunitspher(double theta,
    double phi) 
{
  Coordpoint p;
  p.tag = UNIT_SPHERICAL;
  p.components.spherical.theta = theta;
  p.components.spherical.phi   = phi;
  p.aliases.flag = EVERY_ALIAS_INVALID;
  return p;
}

/* Functions for accessing and
 * modifying aliases of
 * Coordpoint objects: */

/* Make the 'i'th alias in the
 * Coordpoint object pointed
 * to by 'p' to point to the
 * same location as 'ptr'. If
 * this was successful,
 *   'ALIAS_ACCESS_OK'
 * is returned. Otherwise some
 * other signal defined in
 * 'aliasaccessed' is returned
 * to signal a corresponding
 * error. */
enum aliasaccessed aliastoptr(
    Coordpoint *p,
    int i, /* between 1 and
              MAXAMOUNTOFALIASES */
    double const *ptr)
{
  enum coordaliasstatus signal;

  /* check for errors: */
  if (_aliasflagisconsistent() == 0)
    return COORDINATES_H_INCONSISTENT;
  if ((signal = _aliassignal(i)) == EVERY_ALIAS_INVALID)
    return ALIAS_INDEX_OUT_OF_BOUNDS;
  /* update aliases of p: */
  p->aliases.aliasarr[i-1] = ptr;
  p->aliases.flag |= signal;
  return ALIAS_ACCESS_OK;
}

/* Make 'returnptr' pointer
 * point at same location
 * as the 'i'th alias of the
 * Coordpoint object pointed
 * to by 'p'. If this was 
 * successful,
 *   'ALIAS_ACCESS_OK'
 * is returned. Otherwise some
 * other signal defined in
 * 'aliasaccessed' is returned
 * to signal a corresponding
 * error. */
enum aliasaccessed getalias(
    Coordpoint *p,
    int i, /* between 1 and
              MAXAMOUNTOFALIASES */
    double *returnptr)
{
  enum coordaliasstatus signal;

  /* check for errors: */
  if (_aliasflagisconsistent() == 0)
    return COORDINATES_H_INCONSISTENT;
  if ((signal = _aliassignal(i)) == EVERY_ALIAS_INVALID)
    return ALIAS_INDEX_OUT_OF_BOUNDS;
  if ((p->aliases.flag & signal) == 0)
    return REQUESTED_ALIAS_INVALID;
  /* proceed without errors: */
  returnptr = p->aliases.aliasarr[i-1];
  return ALIAS_ACCESS_OK;
}
