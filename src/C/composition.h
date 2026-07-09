/* Provides solutions for turning 
 * a function such as f:Y1*Y2->Z 
 * into a function h:X->Z as if
 * we had a function g:X->Y1*Y2 and
 * made a composition h=f*g. Such
 * composition is not otherwise possible
 * in C for a function declared as
 *   Z f(Y1 arg1, Y2 arg2); */

/* Helper struct for turning a function
 *   double f(double arg1, double arg2); 
 * into a function
 *   double h(double arg);
 * as if we had a function (INVALID C:)
 *   (double, double) g(double arg);
 * and made a composition
 *   double h(double arg) {
 *     return f(g(arg));
 *   }
 * 
 * USAGE: Make an external struct
 *   struct compdouble2D comp;
 * and a function definition implementing
 * the composition h=f*g, for example
 *   double fix1starg(double arg) {
 *     return (*(comp.f))(comp.param,arg);
 *   }
 * Then one can set comp.param and comp.f
 * in another function after which using
 * fix1starg achieves the effect of
 * composition f*g. comp.param can be
 * changed between calling fix1starg
 * corresponding to changing g
 * dynamically between different function 
 * calls.
 *
 * NOTE: One needs to define separate
 * external struct compdouble2D and
 * function implementing the composition
 * (see fix1starg above) for each 
 * composition. */
 struct compdouble2D {
  double param; /* helper parameter for
                   defining composition
                   h=f*g */
  double (*f)(double, double);
};
