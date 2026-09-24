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
 * 'projection.c': */

enum projectionstatus xval(
    Coordpoint p,
    double *proj);

enum projectionstatus xptr(
    Coordpoint p,
    double *proj);
