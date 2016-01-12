#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
//#include "extern.h"

size_t global_counter;

size_t malloc_indirection(size_t size);

size_t malloc_indirection(size_t size)
{
  global_counter += size;
  return malloc(size);
}
