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

#ifndef ITO_OPENCL_MEMORY_H_
#define ITO_OPENCL_MEMORY_H_

#include "base.hpp"

namespace ito {
namespace cl {

/**
 * @brief Create a buffer memory object.
 */
cl_mem CreateBuffer(
    const cl_context &context,
    cl_mem_flags flags,
    size_t size,
    void *host_ptr);

/**
 * @brief Create a 1d-image of type CL_MEM_OBJECT_IMAGE1D.
 */
cl_mem CreateImage1d(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    void *host_ptr);

/**
 * @brief Create a 1d-image buffer of type CL_MEM_OBJECT_IMAGE1D_BUFFER.
 */
cl_mem CreateImage1dBuffer(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    const cl_mem &buffer,
    void *host_ptr);

/**
 * @brief Create a 1d-image buffer of type CL_MEM_OBJECT_IMAGE1D_ARRAY.
 */
cl_mem CreateImage1dArray(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    size_t array_size,
    size_t row_pitch,
    void *host_ptr);

/**
 * @brief Create a 2d-image buffer of type CL_MEM_OBJECT_IMAGE2D.
 */
cl_mem CreateImage2d(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    size_t height,
    size_t row_pitch,
    void *host_ptr);

/**
 * @brief Create a 2d-image buffer of type CL_MEM_OBJECT_IMAGE2D_ARRAY.
 */
cl_mem CreateImage2dArray(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    size_t height,
    size_t array_size,
    size_t row_pitch,
    size_t slice_pitch,
    void *host_ptr);

/**
 * @brief Create a 1d-image buffer of type CL_MEM_OBJECT_IMAGE3D.
 */
cl_mem CreateImage3d(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    size_t height,
    size_t depth,
    size_t row_pitch,
    size_t slice_pitch,
    void *host_ptr);

/**
 * @brief Decrement the memory object reference count.
 */
cl_int ReleaseMemObject(const cl_mem &memobj);

} /* cl */
} /* ito */

#endif /* ITO_OPENCL_MEMORY_H_ */
