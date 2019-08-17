/*
 * This files provides a customized memory allocator and a deallocator
 * for struct objects pointed by a _MMSafe_ptr<T>.
 * */

#include <stdchecked.h>
#include <stdint.h>
#include <stdlib.h>

// A helper struct that has the "same" inner structure as a mmsafe_ptr.
// It is used to help create a _MMSafe_ptr.
typedef struct {
  void *p;
  uint64_t ID;
} _MMSafe_ptr_Rep;


//
// Function: mmsafe_alloc()
//
// This is a customized memory allocator. First it creates a helper struct
// _MMSafe_ptr_Rep and initializes its two field accordingly. Second it casts
// the type of an implicit pointer to the type of a pointer to a _MMSafe_ptr.
// Finally it returns the dereference of the pointer to _MMSafe_ptr.
//
for_any(T) mmsafe_ptr<T> mmsafe_alloc(unsigned long struct_size) {
    void *raw_ptr = malloc(struct_size);

    // Generate a random number as the ID.
    // FIXME: replace the naive rand() function with a robust random
    // number generator which gives a good 64-bit random number.
    uint64_t new_ID = rand();
    // We assume that ID is always the first field of a struct; so here
    // we can set the ID without knowing the concrete structure of a struct.
    *((uint64_t *)(raw_ptr)) = new_ID;

    // Create a helper struct.
    _MMSafe_ptr_Rep safe_ptr = { .p = raw_ptr, .ID = new_ID };

    mmsafe_ptr<T> *mmsafe_ptr_ptr = (mmsafe_ptr<T> *)&safe_ptr;
    return *mmsafe_ptr_ptr;
}

// 
// Function: mmsafe_free()
//
// This is a customized memory deallocator. It sets the ID of the struct to 0
// and calls free() from the stdlib to free the struct.
// 
// @param p - a _MMSafe_ptr whose pointee is going to be freed.
//
for_any(T) void mmsafe_free(mmsafe_ptr<T> p) {
  // Declare a _MMSafe_ptr_Rep so that we can manipulate the real _MMSafe_ptr
  // conveniently. The "volatile" keyword is needed because without which
  // The compiler may optimize away the statement that zeros out the ID.
  volatile _MMSafe_ptr_Rep *mmsafe_ptr_ptr = (_MMSafe_ptr_Rep *)&p;

  // This step may not be necessary in some cases. In some implementation,
  // free() zeros out all bytes of the memory region of the freed object.
  *((uint64_t *)(mmsafe_ptr_ptr->p)) = 0;

  free(mmsafe_ptr_ptr->p);
}
