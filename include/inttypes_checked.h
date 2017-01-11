//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in inttypes.h that             //
// take pointer arguments.                                             //
//                                                                     //
// These are listed in the same order that they occur in the C11       //
// specification.                                                      //
//                                                                     //
// TODO: revise string types after support for pointers to             //
// null-terminated arrays is added to C.                               //
/////////////////////////////////////////////////////////////////////////

#include <stddef.h> // define wchar_t for wcstoimax and wcstoumax
#include <inttypes.h>

intmax_t strtoimax(const char * restrict nptr,
                   char ** restrict endptr : itype(restrict _Ptr<char *>),
                   int base);
uintmax_t strtoumax(const char * restrict nptr,
                    char ** restrict endptr : itype(restrict _Ptr<char *>),
                    int base);

intmax_t wcstoimax(const wchar_t * restrict nptr,
                   wchar_t ** restrict endptr : itype(restrict _Ptr<wchar_t *>),
                   int base);
uintmax_t wcstoumax(const wchar_t * restrict nptr,
                    wchar_t ** restrict endptr : itype(restrict _Ptr<wchar_t *>),
                    int base);
