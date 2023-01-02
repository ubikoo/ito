/*
 * memory.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_CORE_MEMORY_H_
#define ITO_CORE_MEMORY_H_

#include <cstdlib>
#include <cstring>
#include <memory>

#include "base.hpp"

namespace ito {

/** ---- Memory aligned allocator ---------------------------------------------
 * align_alloc
 * @brief Allocate a memory block with a specified size in bytes on a boundary
 * specified by the alignment argument. The alignment must be a power of two
 * and a multiple of sizeof(void *). Given n > 1:
 *
 *  If n is a power of 2, then n takes the form, e.g., 01000000.
 *  Subtract 1 from n:
 *      (n-1) = 01000000 - 00000001 = 00111111.
 *  Bitwise n & (n-1) == 0:
 *      (00111111) & (01000000) = (00000000).
 *
 *  Any n which is not a power of 2, will have an extra 1, e.g., 01000010.
 *  Subtract 1 from n:
 *      (n-1) = 01000010 - 00000001 = 01000001.
 *  Bitwise n & (n-1) != 0:
 *      (01000001) & (01000010) = (01000000).
 *
 * @throw If block size is invalid.
 * @throw If alignment is not a power of 2.
 * @throw If posix_memalign fails to allocate.
 *
 * @note align_alloc and align_free are paired functions.
 */
inline void *align_alloc(size_t size, size_t alignment = 32)
{
    /* Check size is valid and alignment is power of 2. */
    ito_assert(size > 0, "invalid size");
    ito_assert(alignment > 0 && !((alignment-1) & alignment),
        "alignment value is not a power of 2");

    /* Allocate block and check for errors. */
    void *ptr = nullptr;
    int ret = posix_memalign(&ptr, alignment, size);

    ito_assert(ret != EINVAL, "alignment value was not a power of 2");
    ito_assert(ret != ENOMEM, "insufficient memory");
    ito_assert(ptr != nullptr, "failed to allocate");

    /* Ensure the memory block is initialized before returning the ptr.  */
    std::memset(ptr, 0, size);
    return ptr;
}

/**
 * align_free
 * @brief Free a memory block.
 * @note Ensure ptr is set to null after deallocation. If ptr is not null and
 * refers to a space that has been deallocated by a call to align_free or
 * align_realloc, the result of align_free is undefined.
 *
 * @note align_alloc and align_free are paired functions.
 */
inline void align_free(void *ptr)
{
    if (ptr == nullptr) {
        return;
    }
    std::free(ptr);
}

/**
 * align_realloc
 * @brief Reallocate an array of size(oldsize = oldelms * size) bytes to a new
 * array of size(newsize = newelms * size) bytes on a boundary specified by the
 * default alignment.
 *
 * The address of a block returned by malloc or realloc in 32-bit systems is a
 * multiple of eight (or a multiple of sixteen on 64-bit systems).
 * The function reimplements the realloc specification to the new boundary size.
 *
 * @note Summary of the relevant requirements for realloc:
 * - The contents of the object shall remain unchanged up to the lesser of the
 *   new and old sizes. If the new block size requires movement of the object,
 *   the space for the previous instantiation of the object is freed.
 *
 * - If the new block size is smaller than the old block size, align_realloc
 *   will allocate a new block of the appriate size, copy the contents of the
 *   original block up to the new size, free the original block and return a
 *   pointer to the newly created block.
 *   If the new block size is larger, the contents of the newly allocated extra
 *   portion are initialised to 0 as set by align_alloc.
 *
 * - If the input pointer is null, align_realloc behaves exactly as if
 *   align_alloc has been called.
 *
 * - If the newsize is 0 and ptr is not a null pointer, align_realloc
 *   behaves exactly as if align_free has been called and return a null
 *   value.
 *
 * - If the space cannot be allocated, the object shall remain unchanged.
 *   If the input pointer is null and the requested size is 0, then the
 *   result is undefined.
 */
inline void *align_realloc(void *ptr, size_t oldsize, size_t newsize)
{
    /*
     * New size is 0 - free the ptr and return null. Align free a null pointer
     * is a noop, otherwise the memory is freed.
     */
    if (newsize == 0) {
        align_free(ptr);
        return nullptr;
    }

    /* If ptr is a null pointer, return a newly allocated block */
    if (ptr == nullptr) {
        return align_alloc(newsize);
    }

    /*
     * Otherwise, create a new block and copy the contents of the original block
     * up to the lesser of the new and old sizes.
     */
    size_t size = newsize > oldsize ? oldsize : newsize;
    void *mem = align_alloc(newsize);
    std::memcpy(mem, ptr, size);

    /*
     * Free the original block and return the newly created block.
     */
    align_free(ptr);
    return mem;
}

/** ---- Memory templated aligned object allocator ----------------------------
 * align_alloc<T>
 * @brief Allocate a block of memory of with a single object of type T, with
 * size sizeof(T) bytes and default alignment. Initialize the object by calling
 * directly the T constructor at the newly allocated address with placement new.
 *
 * @note align_alloc<T> and align_free<T> are paired functions.
 */
template<typename T, typename... Args>
T *align_alloc(Args&&... args)
{
    T *ptr = nullptr;
    try {
        ptr = (T *) align_alloc(sizeof(T));
        ::new(static_cast<void *>(ptr)) T(static_cast<Args&&>(args)...);
    } catch (std::exception& e) {
        align_free(ptr);
        ito_throw(e.what());
    }
    return ptr;
}

/**
 * align_free<T>
 * @brief Delete a block of memory allocated by align_alloc<T> by manually
 * calling the T destructor for the object and freeing the block.
 *
 * @note align_alloc<T> and align_free<T> are paired functions.
 */
template<typename T>
void align_free(T *ptr)
{
    if (ptr == nullptr) {
        return;
    }

    /* Call the destructor for the object and free the memory block. */
    if (std::is_destructible<T>::value) {
        (*ptr).~T();
    }
    align_free((void *) ptr);
}

/** ---- Memory templated aligned array allocator -----------------------------
 *
 * align_array_alloc<T>
 * @brief Allocate a block of memory with count objects of type T, with size
 * (count*sizeof(T)) bytes, and alignment specified by the L1 cache line size.
 * Initialize each element in the array by calling directly the T constructor
 * at the newly allocated addresses with placement new.
 *
 * @note align_array_alloc<T> and align_array_free<T> are paired functions.
 */
template<typename T, typename... Args>
T *align_array_alloc(size_t count, Args&&... args)
{
    /* Allocate a block with(count*sizeof(T)) bytes. */
    T *ptr = nullptr;
    try {
        ptr = (T *) align_alloc(count * sizeof(T));
    } catch (std::exception& e) {
        align_free(ptr);
        ito_throw(e.what());
    }

    /* Construct the objects in place at each memory address. */
    size_t i = 0;
    try {
        for (; i < count; ++i) {
            ::new(static_cast<void *>(ptr + i)) T(static_cast<Args&&>(args)...);
        }
    } catch (std::exception& e) {
        for (size_t j = 0; j < i; ++j) {
           ptr[j].~T();
        }
        ito_throw(e.what());
    }

    return ptr;
}

/**
 * align_array_free<T>
 * @brief Delete a block of memory with count objects of type T, each allocated
 * by align_alloc<T> by manually calling the T destructor for each object in the
 * array before freeing the block.
 *
 * @note align_array_alloc<T> and align_array_free<T> are paired functions.
 */
template<typename T>
void align_array_free(T *ptr, size_t count)
{
    if (ptr == nullptr) {
        return;
    }

    /* Call the destructor for each object */
    if (std::is_destructible<T>::value) {
        for (size_t i = 0; i < count; ++i) {
            ptr[i].~T();
        }
    }

    /* Free the memory block */
    align_free((void *) ptr);
}

} /* ito */

#endif /* ITO_CORE_MEMORY_H_ */
