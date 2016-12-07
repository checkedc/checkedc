//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in time.h that                 //
// take pointer arguments.                                             //
//                                                                     //
// These are listed in the same order that they occur in the C11       //
// specification.                                                      //
//                                                                     //
// TODO: revise string types after support for pointers to             //
// null-terminated arrays is added to C.                               //
/////////////////////////////////////////////////////////////////////////

#include <time.h>

time_t mktime(struct tm *timeptr : itype(_Ptr<struct tm>));
int timespec_get(struct timespec *ts : itype(_Ptr<struct timespec>),
                 int base);
char *asctime(const struct tm *timeptr : itype(_Ptr<const struct tm>));
char *ctime(const time_t *timer : itype(_Ptr<const time_t>));
struct tm *gmtime(const time_t *timer : itype(_Ptr<const time_t>)) :
  itype(_Ptr<struct tm>);
struct tm *localtime(const time_t *timer : itype(_Ptr<const time_t>)) :
  itype(_Ptr<struct tm>);
size_t strftime(char * restrict output : count(maxsize),
                size_t maxsize,
                const char * restrict format,
                const struct tm * restrict timeptr :
                   itype(restrict _Ptr<const struct tm>));


