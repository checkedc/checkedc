// Simple Example of Checked C, from Section 2.8 of the spec

#include <stddef.h>
#include <stdchecked.h>

void append(array_ptr<char> dst : count(dst_count),
            array_ptr<char> src : count(src_count),
            size_t dst_count, size_t src_count) {
  dynamic_check(src_count <= dst_count);
  for (size_t i = 0; i < src_count; i++) {
    if (src[i] == '\0') {
      break;
    }
    dst[i] = src[i];
  }
}   
