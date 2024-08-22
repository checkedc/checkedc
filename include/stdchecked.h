#ifndef __STDCHECKED_H
#define __STDCHECKED_H

#define ptr _Ptr
#define array_ptr _Array_ptr
#define nt_array_ptr _Nt_array_ptr
#define checked _Checked
#define nt_checked _Nt_checked
#define unchecked _Unchecked
#define bounds_only _Bounds_only
#define where _Where
#define dynamic_check _Dynamic_check
#define dynamic_bounds_cast _Dynamic_bounds_cast
#define assume_bounds_cast _Assume_bounds_cast
#define return_value _Return_value
#define _Dynamic_bounds_cast_M(T, e1, ... ) _Dynamic_bounds_cast<T>(e1, __VA_ARGS__)
#define _Dynamic_bounds_cast_M_N(T, e1 ) _Dynamic_bounds_cast<T>(e1)
#define _Assume_bounds_cast_M(T, e1, ... ) _Assume_bounds_cast<T>(e1, __VA_ARGS__)
#define _Assume_bounds_cast_M_N(T, e1 ) _Assume_bounds_cast<T>(e1)
#endif /* __STDCHECKED_H */
