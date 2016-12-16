//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in math.h that                 //
// take pointer arguments.                                             //
//                                                                     //
// These are listed in the same order that they occur in the C11       //
// specification.                                                      //
//                                                                     //
// TODO: revise string types after support for pointers to             //
// null-terminated arrays is added to C.                               //
/////////////////////////////////////////////////////////////////////////

#include <math.h>

double frexp(double value, int *exp : itype(_Ptr<int>));
float frexpf(float value, int *exp : itype(_Ptr<int>));
long double frexpl(long double value, int *exp : itype(_Ptr<int>));

double modf(double value, double *iptr : itype(_Ptr<double>));
float modff(float value, float *iptr : itype(_Ptr<float>));
long double modfl(long double value,
                  long double *iptr : itype(_Ptr<long double>));

double remquo(double x, double y, int *quo : itype(_Ptr<int>));
float remquof(float x, float y, int *quo : itype(_Ptr<int>));
long double remquol(long double x, long double y, int *quo : itype(_Ptr<int>));

// TODO: strings
// double nan(const char *t);
// float nanf(const char *t);
// long double nanf(const char *t);