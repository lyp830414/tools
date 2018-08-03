#pragma once

#include <types.h>


extern "C" {
  void* memset( void* ptr, uint32_t value, uint32_t num );
  void* memcpy( void* destination, const void* source, uint32_t num );
  int32_t memcmp( void* ptr1, const void* ptr2, uint32_t num );
} 

void* sbrk(size_t num_bytes);
