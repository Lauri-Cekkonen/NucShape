#include "coordinates.h"
#include <math.h>

#define PI 3.14159

/* Calculate the distance between
 * the point represented as 
 * Coordpoint 'p' and the origin.
 *
 * NOTE: If the coordinate system
 * of 'p' is unknown (Coordpoint
 * defined in coordinates.h
 * contains more coordinate
 * systems not defined in this
 * function), -1.0 is returned. */
double length(Coordpoint p)
{
  switch (p.tag) {
    case CART1D:
      return fabs(p.coord.cart1D.x);
    case CART2D:
      return sqrt(
          pow(p.coord.cart2D.x, 2) +
          pow(p.coord.cart2D.y, 2));
    case CART3D:
      return sqrt(
          pow(p.coord.cart3D.x, 2) +
          pow(p.coord.cart3D.y, 2) +
          pow(p.coord.cart3D.z, 2));
    case UNITSPHER:
      return 1.0;
    case SPHER:
      return p.coord.spher.r; /* assumption: r > 0 or
                                             r = 0 */
    case POLAR:
      return p.coord.polar.r; /* assumption: r > 0 or
                                             r = 0 */
    case CYLIND:
      return sqrt(
          pow(p.coord.cylind.r, 2) +
          pow(p.coord.cylind.z, 2));
    default:
      return -1.0; /* unknown coordinate system */
  }
}

/* Real spherical harmonics.
 * 
 * NOTE: If the coordinate 
 * system is something else 
 * than 3D cartesian or (unit)
 * spherical, -1.0 is
 * returned. */

/* l = 0 */

double Y00(Coordpoint p)
{
  return (1/2)*sqrt(1/PI);
}

/* l = 1 */

double realY1_1(Coordpoint p)
{
  double coeff = sqrt(3/(4*PI));
  switch (p.tag) {
    case CART3D:
      return coeff*p.coord.cart3d.y/
                   length(p);
    case UNITSPHER:
      return coeff*sin(p.coord.unitspher.theta)*
                   sin(p.coord.unitspher.phi);
    case SPHER:
      return coeff*sin(p.coord.spher.theta)*
                   sin(p.coord.spher.phi);
    default:
      return -1.0;
  }
}

double realY10(Coordpoint p)
{
  double coeff = sqrt(3/(4*PI));
  switch (p.tag) {
    case CART3D:
      return coeff*p.coord.cart3d.z/
                   length(p);
    case UNITSPHER:
      return coeff*cos(p.coord.unitspher.theta);
    case SPHER:
      return coeff*cos(p.coord.spher.theta);
    default:
      return -1.0;
  }
}

double realY11(Coordpoint p)
{
  double coeff = sqrt(3/(4*PI));
  switch (p.tag) {
    case CART3D:
      return coeff*p.coord.cart3d.x/
                   length(p);
    case UNITSPHER:
      return coeff*sin(p.coord.unitspher.theta)*
                   cos(p.coord.unitspher.phi);
    case SPHER:
      return coeff*sin(p.coord.spher.theta)*
                   cos(p.coord.spher.phi);
    default:
      return -1.0;
  }
}

/* l = 2 */

double realY2_2(Coordpoint p)
{
  double coeff = (1/4)*sqrt(15/(PI));
  switch (p.tag) {
    case CART3D:
      return 2*coeff*p.coord.cart3d.x*p.coord.cart3d.y/
                     pow(length(p), 2);
    case UNITSPHER:
      return coeff*pow(sin(p.coord.unitspher.theta), 2)*
                   sin(2*p.coord.unitspher.phi);
    case SPHER:
      return coeff*pow(sin(p.coord.spher.theta), 2)*
                   sin(2*p.coord.spher.phi);
    default:
      return -1.0;
  }
}

double realY2_1(Coordpoint p)
{
  double coeff = (1/4)*sqrt(15/(PI));
  switch (p.tag) {
    case CART3D:
      return 2*coeff*p.coord.cart3d.y*p.coord.cart3d.z/
                     pow(length(p), 2);
    case UNITSPHER:
      return coeff*sin(2*p.coord.unitspher.theta)*
                   sin(p.coord.unitspher.phi);
    case SPHER:
      return coeff*sin(2*p.coord.spher.theta)*
                   sin(p.coord.spher.phi);
    default:
      return -1.0;
  }
}

double realY20(Coordpoint p)
{
  double coeff = (1/4)*sqrt(5/(PI));
  switch (p.tag) {
    case CART3D:
      return coeff*(3*pow(p.coord.cart3d.z, 2) 
                    - pow(length(p), 2))/
                   pow(length(p), 2);
    case UNITSPHER:
      return coeff*(3*pow(cos(p.coord.unitspher.theta), 2)
                    - 1.0);
    case SPHER:
      return coeff*(3*pow(cos(p.coord.spher.theta), 2)
                    - 1.0);
    default:
      return -1.0;
  }
}

double realY21(Coordpoint p)
{
  double coeff = (1/4)*sqrt(15/(PI));
  switch (p.tag) {
    case CART3D:
      return 2*coeff*p.coord.cart3d.x*p.coord.cart3d.z/
                     pow(length(p), 2);
    case UNITSPHER:
      return coeff*sin(2*p.coord.unitspher.theta)*
                   cos(p.coord.unitspher.phi);
    case SPHER:
      return coeff*sin(2*p.coord.spher.theta)*
                   cos(p.coord.spher.phi);
    default:
      return -1.0;
  }
}

double realY22(Coordpoint p)
{
  double coeff = (1/4)*sqrt(15/(PI));
  switch (p.tag) {
    case CART3D:
      return coeff*(pow(p.coord.cart3d.x, 2) -
                    pow(p.coord.cart3d.y, 2))/
                    pow(length(p), 2);
    case UNITSPHER:
      return coeff*pow(sin(p.coord.unitspher.theta), 2)*
                   cos(2*p.coord.unitspher.phi);
    case SPHER:
      return coeff*pow(sin(p.coord.spher.theta), 2)*
                   cos(2*p.coord.spher.phi);
    default:
      return -1.0;
  }
}
