/// \file

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "heap.h"

#ifndef __gc__
#define __gc__

// /// The opaque data type holding all the heap data
// typedef struct heap heap_t;

/// The signature of the trace function
typedef void *(*trace_f)(heap_t *h, void *obj);

/// The signature of object-specific trace functions. It will be
/// called for its specific objects, and be given a generic trace
/// function f to be called on each pointer inside obj.
typedef void *(*s_trace_f)(heap_t *h, trace_f f, void *obj);

/// Delete a heap and trace, killing off stack pointers.
///
/// \param h the heap
/// \param dbg_value a value to be written into every pointer into h on the stack
void h_delete_dbg(heap_t *h, void *dbg_value);

/// Allocate a new object on a heap with a given size, and
/// object-specific trace function.
///
/// See example of object-specific trace function
/// at the end of gc.h
///
/// \sa s_trace_f
///
/// \param h the heap
/// \param bytes the size in bytes
/// \param f the object-specific trace function
/// \return the newly allocated object
void *h_alloc_union(heap_t *h, size_t bytes, s_trace_f f);

/// Manually trigger garbage collection.
///
/// Garbage collection is otherwise run when an allocation is
/// impossible in the available consecutive free memory.
///
/// \param h the heap
/// \return the number of bytes collected
size_t h_gc(heap_t *h);

/// Manually trigger garbage collection with the ability to
/// override the setting for how stack pointers are treated.
///
/// Garbage collection is otherwise run when an allocation is
/// impossible in the available consecutive free memory.
///
/// \param h the heap
/// \param unsafe_stack true if pointers on the stack are to be considered unsafe pointers
/// \return the number of bytes collected
size_t h_gc_dbg(heap_t *h, bool unsafe_stack);

#endif
