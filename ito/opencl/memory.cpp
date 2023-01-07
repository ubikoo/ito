/*
 * memory.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "memory.hpp"

namespace ito {
namespace cl {

/**
 * @brief Create a buffer memory object.
 * @param flags Bitfield specifying the buffer memory usage and allocation.
 *  CL_MEM_READ_WRITE
 *  CL_MEM_WRITE_ONLY
 *  CL_MEM_READ_ONLY
 *  CL_MEM_USE_HOST_PTR
 *  CL_MEM_ALLOC_HOST_PTR
 *  CL_MEM_COPY_HOST_PTR
 * @param size Size in bytes of the buffer memory object to be allocated.
 * @param host_ptr Pointer to the buffer data that may already be allocated by
 * the host application.
 */
cl_mem CreateBuffer(
    const cl_context &context,
    cl_mem_flags flags,
    size_t size,
    void *host_ptr)
{
    cl_int err;
    cl_mem buffer = clCreateBuffer(
        context,
        flags,
        size,
        host_ptr,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateBuffer");
    return buffer;
}

/**
 * @brief Create a 1d-image of type CL_MEM_OBJECT_IMAGE1D.
 * @note Size of buffer that host_ptr points to >= row_pitch.
 */
cl_mem CreateImage1d(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    void *host_ptr)
{
    /* Image descriptor describing type and dimensions of a 1d-image. */
    cl_image_desc desc{
        CL_MEM_OBJECT_IMAGE1D,          /* cl_mem_object_type image type */
        width,                          /* image width */
        0,                              /* image height */
        0,                              /* image depth */
        0,                              /* image array size */
        0,                              /* image row pitch */
        0,                              /* image slice pitch */
        0,                              /* num mip levels */
        0,                              /* num samples */
        0};                             /* cl_mem buffer, cl_mem mem_object */

    cl_int err;
    cl_mem image = clCreateImage(
        context,
        flags,
        &format,
        &desc,
        host_ptr,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateImage");
    return image;
}

/**
 * @brief Create a 1d-image buffer of type CL_MEM_OBJECT_IMAGE1D_BUFFER.
 * @note Size of buffer that host_ptr points to >= row_pitch.
 */
cl_mem CreateImage1dBuffer(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    const cl_mem &buffer,
    void *host_ptr)
{
    /* Image descriptor describing type and dimensions of a 1d-image buffer. */
    cl_image_desc desc{
        CL_MEM_OBJECT_IMAGE1D_BUFFER,   /* cl_mem_object_type image type */
        width,                          /* image width */
        0,                              /* image height */
        0,                              /* image depth */
        0,                              /* image array size */
        0,                              /* image row pitch */
        0,                              /* image slice pitch */
        0,                              /* num mip levels */
        0,                              /* num samples */
        buffer};                        /* cl_mem buffer, cl_mem mem_object */

    cl_int err;
    cl_mem image = clCreateImage(
        context,
        flags,
        &format,
        &desc,
        host_ptr,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateImage");
    return image;
}

/**
 * @brief Create a 1d-image buffer of type CL_MEM_OBJECT_IMAGE1D_ARRAY.
 * @note Size of buffer that host_ptr points to >= array_size * row_pitch.
 */
cl_mem CreateImage1dArray(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    size_t array_size,
    size_t row_pitch,
    void *host_ptr)
{
    /* Image descriptor describing type and dimensions of a 1d-image array. */
    cl_image_desc desc{
        CL_MEM_OBJECT_IMAGE1D_ARRAY,    /* cl_mem_object_type image type */
        width,                          /* image width */
        0,                              /* image height */
        0,                              /* image depth */
        array_size,                     /* image array size */
        row_pitch,                      /* image row pitch */
        0,                              /* image slice pitch */
        0,                              /* num mip levels */
        0,                              /* num samples */
        0};                             /* cl_mem buffer, cl_mem mem_object */

    cl_int err;
    cl_mem image = clCreateImage(
        context,
        flags,
        &format,
        &desc,
        host_ptr,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateImage");
    return image;
}

/**
 * @brief Create a 2d-image buffer of type CL_MEM_OBJECT_IMAGE2D.
 * @note Size of buffer that host_ptr points to >= height * row_pitch.
 */
cl_mem CreateImage2d(
    const cl_context &context,
    cl_mem_flags flags,
    const cl_image_format &format,
    size_t width,
    size_t height,
    size_t row_pitch,
    void *host_ptr)
{
    /* Image descriptor describing type and dimensions of a 2d-image. */
    cl_image_desc desc{
        CL_MEM_OBJECT_IMAGE2D,          /* cl_mem_object_type image type */
        width,                          /* image width */
        height,                         /* image height */
        0,                              /* image depth */
        0,                              /* image array size */
        row_pitch,                      /* image row pitch */
        0,                              /* image slice pitch */
        0,                              /* num mip levels */
        0,                              /* num samples */
        0};                             /* cl_mem buffer, cl_mem mem_object */

    cl_int err;
    cl_mem image = clCreateImage(
        context,
        flags,
        &format,
        &desc,
        host_ptr,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateImage");
    return image;
}

/**
 * @brief Create a 2d-image buffer of type CL_MEM_OBJECT_IMAGE2D_ARRAY.
 * @note Size of buffer that host_ptr points to >= slice_pitch * array_size.
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
    void *host_ptr)
{
    /* Image descriptor describing type and dimensions of a 2d-image array. */
    cl_image_desc desc{
        CL_MEM_OBJECT_IMAGE2D_ARRAY,    /* cl_mem_object_type image type */
        width,                          /* image width */
        height,                         /* image height */
        0,                              /* image depth */
        array_size,                     /* image array size */
        row_pitch,                      /* image row pitch */
        slice_pitch,                    /* image slice pitch */
        0,                              /* num mip levels */
        0,                              /* num samples */
        0};                             /* cl_mem buffer, cl_mem mem_object */

    cl_int err;
    cl_mem image = clCreateImage(
        context,
        flags,
        &format,
        &desc,
        host_ptr,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateImage");
    return image;
}

/**
 * @brief Create a 1d-image buffer of type CL_MEM_OBJECT_IMAGE3D.
 * @note Size of buffer that host_ptr points to >= slice_pitch * depth.
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
    void *host_ptr)
{
    /* Image descriptor describing type and dimensions of a 3d-image. */
    cl_image_desc desc{
        CL_MEM_OBJECT_IMAGE3D,          /* cl_mem_object_type image type */
        width,                          /* image width */
        height,                         /* image height */
        depth,                          /* image depth */
        0,                              /* image array size */
        row_pitch,                      /* image row pitch */
        slice_pitch,                    /* image slice pitch */
        0,                              /* num mip levels */
        0,                              /* num samples */
        0};                             /* cl_mem buffer, cl_mem mem_object */

    cl_int err;
    cl_mem image = clCreateImage(
        context,
        flags,
        &format,
        &desc,
        host_ptr,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateImage");
    return image;
}

/**
 * @brief Release the memory object and decrement its reference count.
 */
void ReleaseMemObject(const cl_mem &memobj)
{
    cl_int err = clReleaseMemObject(memobj);
    ito_assert(err == CL_SUCCESS, "clReleaseMemObject");
}

} /* cl */
} /* ito */
