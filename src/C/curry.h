/* return type of function from double to
 * double -> double */
typedef double (*Curriedfunc)(double);

double secondarg;
double (*twoargfunc)(double, double);

double fix2ndarg(double arg) {
  return (*twoargfunc)(arg, secondarg);
}

double fix1starg(double arg) {
  return (*twoargfunc)(secondarg, arg);
}

Curriedfunc curry1starg(double (*func)(double, double),
    double arg) {
  secondarg = arg;
  twoargfunc = func;
  return &fix2ndarg;
}

Curriedfunc curry2ndarg(double (*func)(double, double),
    double arg) {
  secondarg = arg;
  twoargfunc = func;
  return &fix1starg;
}
