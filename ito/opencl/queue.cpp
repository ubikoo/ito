/*
 * queue.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "queue.hpp"

namespace ito {
namespace cl {

/** ---------------------------------------------------------------------------
 * @brief Create a command queue on the specified device. properties argument
 * specifies execution order. If set to CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
 * there is no guarantee kernel A finishes before kernel B starts execution.
 */
cl_command_queue CreateCommandQueue(
    const cl_context &context,
    const cl_device_id &device,
    cl_command_queue_properties properties)
{
    ito_assert(
        properties == 0                                     ||
        properties == CL_QUEUE_PROFILING_ENABLE             ||
        properties == CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
        "invalid queue properties");

    cl_int err;
    cl_command_queue queue = clCreateCommandQueue(
        context,
        device,
        properties,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateCommandQueue");
    return queue;
}

/**
 * @brief Release the command queue and decrement its reference count.
 */
void ReleaseCommandQueue(const cl_command_queue &queue)
{
    cl_int err = clReleaseCommandQueue(queue);
    ito_assert(err == CL_SUCCESS, "clReleaseCommandQueue");
}

/** ---------------------------------------------------------------------------
 * @brief Read from a buffer object to host memory.
 */
cl_int EnqueueReadBuffer(
    const cl_command_queue &queue,
    cl_mem &buffer,
    cl_bool blocking,
    size_t offset,
    size_t size,
    void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueReadBuffer(
        queue,
        buffer,
        blocking,
        offset,
        size,
        ptr,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueReadBuffer");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Write to a buffer object from host memory.
 */
cl_int EnqueueWriteBuffer(
    const cl_command_queue &queue,
    cl_mem &buffer,
    cl_bool blocking,
    size_t offset,
    size_t size,
    void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueWriteBuffer(
        queue,
        buffer,
        blocking,
        offset,
        size,
        ptr,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueWriteBuffer");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Copy from one buffer object to another buffer object.
 */
cl_int EnqueueCopyBuffer(
    const cl_command_queue &queue,
  	cl_mem &src_buffer,
  	cl_mem &dst_buffer,
  	size_t src_offset,
  	size_t dst_offset,
  	size_t size,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueCopyBuffer(
        queue,
        src_buffer,
        dst_buffer,
        src_offset,
        dst_offset,
        size,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueCopyBuffer");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Read from a rectangular region in a buffer object to host memory.
 */
cl_int EnqueueReadBufferRect(
    const cl_command_queue &queue,
  	cl_mem &buffer,
  	cl_bool blocking,
    const std::array<size_t,3> &buffer_origin,
    const std::array<size_t,3> &host_origin,
    const std::array<size_t,3> &region,
  	size_t buffer_row_pitch,
  	size_t buffer_slice_pitch,
  	size_t host_row_pitch,
  	size_t host_slice_pitch,
  	void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueReadBufferRect(
        queue,
        buffer,
        blocking,
        buffer_origin.data(),
        host_origin.data(),
        region.data(),
        buffer_row_pitch,
        buffer_slice_pitch,
        host_row_pitch,
        host_slice_pitch,
        ptr,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueReadBufferRect");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Write a rectangular region in a buffer object from host memory.
 */
cl_int EnqueueWriteBuffer_rect(
    const cl_command_queue &queue,
  	cl_mem &buffer,
  	cl_bool blocking,
    const std::array<size_t,3> &buffer_origin,
    const std::array<size_t,3> &host_origin,
    const std::array<size_t,3> &region,
  	size_t buffer_row_pitch,
  	size_t buffer_slice_pitch,
  	size_t host_row_pitch,
  	size_t host_slice_pitch,
  	void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueWriteBufferRect(
        queue,
        buffer,
        blocking,
        buffer_origin.data(),
        host_origin.data(),
        region.data(),
        buffer_row_pitch,
        buffer_slice_pitch,
        host_row_pitch,
        host_slice_pitch,
        ptr,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueWriteBufferRect");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Copy a rectangular region in a buffer object to another buffer object.
 */
cl_int EnqueueCopyBuffer_rect(
    const cl_command_queue &queue,
  	cl_mem &src_buffer,
  	cl_mem &dst_buffer,
    const std::array<size_t,3> &src_origin,
    const std::array<size_t,3> &dst_origin,
    const std::array<size_t,3> &region,
 	size_t src_row_pitch,
  	size_t src_slice_pitch,
  	size_t dst_row_pitch,
  	size_t dst_slice_pitch,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueCopyBufferRect(
        queue,
        src_buffer,
        dst_buffer,
        src_origin.data(),
        dst_origin.data(),
        region.data(),
        src_row_pitch,
        src_slice_pitch,
        dst_row_pitch,
        dst_slice_pitch,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueCopyBufferRect");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Fill a buffer object with a pattern of a given pattern size.
 * Pattern type must be an accepted OpenCL data type.
 */
cl_int EnqueueFillBuffer(
    const cl_command_queue &queue,
    cl_mem &buffer,
    const void *pattern,
    size_t pattern_size,
    size_t offset,
    size_t size,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueFillBuffer(
        queue,
        buffer,
        pattern,
        pattern_size,
        offset,
        size,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueFillBuffer");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/** ---------------------------------------------------------------------------
 * @brief Copy data from the specified buffer object to host memory.
 */
cl_int EnqueueCopyFrom(
    const cl_command_queue &queue,
    cl_mem &buffer,
    size_t size,
    void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    return EnqueueReadBuffer(
        queue,
        buffer,
        CL_TRUE,
        0,
        size,
        ptr,
        event_wait_list,
        event);
}

/**
 * @brief Copy an data from the host memory to the specified buffer object.
 */
cl_int EnqueueCopyTo(
    const cl_command_queue &queue,
    cl_mem &buffer,
    size_t size,
    void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    return EnqueueWriteBuffer(
        queue,
        buffer,
        CL_TRUE,
        0,
        size,
        ptr,
        event_wait_list,
        event);
}

/** ---------------------------------------------------------------------------
 * @brief Read from an image or image array object to host memory.
 */
cl_int EnqueueReadImage(
    const cl_command_queue &queue,
    cl_mem &image,
 	cl_bool blocking,
    const std::array<size_t,3> &origin,
    const std::array<size_t,3> &region,
    size_t row_pitch,
    size_t slice_pitch,
    void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueReadImage(
        queue,
        image,
        blocking,
        origin.data(),
        region.data(),
        row_pitch,
        slice_pitch,
        ptr,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueReadImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Write to an image or image array object from host memory.
 */
cl_int EnqueueWriteImage(
    const cl_command_queue &queue,
    cl_mem &image,
 	cl_bool blocking,
    const std::array<size_t,3> &origin,
    const std::array<size_t,3> &region,
    size_t row_pitch,
    size_t slice_pitch,
    void *ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueWriteImage(
        queue,
        image,
        blocking,
        origin.data(),
        region.data(),
        row_pitch,
        slice_pitch,
        ptr,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueWriteImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Copy from a src image object to a dst image object.
 */
cl_int EnqueueCopyImage(
    const cl_command_queue &queue,
    cl_mem &src_image,
    cl_mem &dst_image,
    const std::array<size_t,3> &src_origin,
    const std::array<size_t,3> &dst_origin,
    const std::array<size_t,3> &region,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueCopyImage(
        queue,
        src_image,
        dst_image,
        src_origin.data(),
        dst_origin.data(),
        region.data(),
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueCopyImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Fill an image object with a specified color.
 * @param fill_color is the color to use to fill the image. This is a four
 * component RGBA floating-point color value if the image channel data type
 * is not an unnormalized signed or unsigned data type.
 */
cl_int EnqueueFillImage(
    const cl_command_queue &queue,
    cl_mem &image,
    const cl_float4 fill_color,
    const std::array<size_t,3> &origin,
    const std::array<size_t,3> &region,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueFillImage(
        queue,
        image,
        static_cast<const void *>(&fill_color),
        origin.data(),
        region.data(),
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueFillImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Fill an image object with a specified color.
 * @param fill_color is the color to use to fill the image. This is a four
 * component RGBA signed integer color value if the image channel data type
 * is an unnormalized signed integer type.
 */
cl_int EnqueueFillImage(
    const cl_command_queue &queue,
    cl_mem &image,
    const cl_int4 fill_color,
    const std::array<size_t,3> &origin,
    const std::array<size_t,3> &region,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueFillImage(
        queue,
        image,
        static_cast<const void *>(&fill_color),
        origin.data(),
        region.data(),
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueFillImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Fill an image object with a specified color.
 * @param fill_color is the color to use to fill the image. This is a four
 * component RGBA unsigned integer color value if the image channel data type
 * is an unnormalized signed integer type.
 */
cl_int EnqueueFillImage(
    const cl_command_queue &queue,
    cl_mem &image,
    const cl_uint4 fill_color,
    const std::array<size_t,3> &origin,
    const std::array<size_t,3> &region,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueFillImage(
        queue,
        image,
        static_cast<const void *>(&fill_color),
        origin.data(),
        region.data(),
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueFillImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/** ---------------------------------------------------------------------------
 * @brief Copy from an image object to a buffer object.
 */
cl_int EnqueueCopyImageToBuffer(
    const cl_command_queue &queue,
    cl_mem &src_image,
    cl_mem &dst_buffer,
    const std::array<size_t,3> &src_origin,
    const std::array<size_t,3> &region,
    size_t dst_offset,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueCopyImageToBuffer(
        queue,
        src_image,
        dst_buffer,
        src_origin.data(),
        region.data(),
        dst_offset,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueCopyImageToBuffer");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Copy from a buffer object to an image object.
 */
cl_int EnqueueCopyBufferToImage(
    const cl_command_queue &queue,
    cl_mem &src_buffer,
    cl_mem &dst_image,
    size_t src_offset,
    const std::array<size_t,3> &dst_origin,
    const std::array<size_t,3> &region,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueCopyBufferToImage(
        queue,
        src_buffer,
        dst_image,
        src_offset,
        dst_origin.data(),
        region.data(),
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueCopyBufferToImage");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/** ---------------------------------------------------------------------------
 * @brief Map a regionof the buffer object given by buffer into the host address
 * space and returns a pointer to this mapped region.
 */
void *EnqueueMapBuffer(
    const cl_command_queue &queue,
    cl_mem &buffer,
    cl_bool blocking,
    cl_map_flags flags,
  	size_t offset,
  	size_t size,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event,
    cl_int *errcode)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err;
    void *result = clEnqueueMapBuffer(
        queue,
        buffer,
        blocking,
        flags,
        offset,
        size,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL,
        &err);
    ito_assert(err == CL_SUCCESS, "clEnqueueMapBuffer");

    if (errcode != NULL) {
        *errcode = err;
    }

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return result;
}

/**
 * @brief Map a region of an image object into the host address space and
 * returns a pointer to this mapped region.
 */
void *EnqueueMapImage(
    const cl_command_queue &queue,
    cl_mem &image,
    cl_bool blocking,
    cl_map_flags flags,
    const std::array<size_t,3> &origin,
    const std::array<size_t,3> &region,
  	size_t *row_pitch,
  	size_t *slice_pitch,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event,
    cl_int *errcode)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err;
    void *result = clEnqueueMapImage(
        queue,
        image,
        blocking,
        flags,
        origin.data(),
        region.data(),
        row_pitch,
        slice_pitch,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL,
        &err);
    ito_assert(err == CL_SUCCESS, "clEnqueueMapImage");

    if (errcode != NULL) {
        *errcode = err;
    }

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return result;
}

/**
 * @brief Unmap a previously mapped region of a memory object.
 */
cl_int EnqueueUnmapMemObject(
    const cl_command_queue &queue,
    cl_mem &mem_object,
    void *mapped_ptr,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueUnmapMemObject(
        queue,
        mem_object,
        mapped_ptr,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueUnmapMemObject");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Indicate the device a set of memory objects should be associated with.
 */
cl_int EnqueueMigrateMemObjects(
    const cl_command_queue &queue,
    const std::vector<cl_mem> &mem_objects,
    cl_mem_migration_flags flags,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueMigrateMemObjects(
        queue,
        static_cast<cl_uint>(mem_objects.size()),
        mem_objects.data(),
        flags,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueMigrateMemObjects");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/** ---------------------------------------------------------------------------
 * @brief Execute a marker command which waits for either a list of events to
 * complete, or all previously enqueued commands to complete.
 */
cl_int EnqueueMarkerWithWaitList(
    const cl_command_queue &queue,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueMarkerWithWaitList(
        queue,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueMarkerWithWaitList");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Execute a barrier command which waits for either a list of events to
 * complete, or if the list is empty, waits for all commands previously enqueued
 * in command_queue to complete before it completes.
 */
cl_int EnqueueBarrierWithWaitList(
    const cl_command_queue &queue,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueBarrierWithWaitList(
        queue,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueBarrierWithWaitList");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/** ---------------------------------------------------------------------------
 * @brief Execute a kernel on a device.
 */
cl_int EnqueueNDRangeKernel(
    const cl_command_queue &queue,
    const cl_kernel &kernel,
    const NDRange &global_work_offset,
    const NDRange &global_work_size,
    const NDRange &local_work_size,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueNDRangeKernel(
        queue,
        kernel,
        global_work_size.dim,
        global_work_offset.dim != 0 ? global_work_offset.range.data() : NULL,
        global_work_size.range.data(),
        local_work_size.dim != 0 ? local_work_size.range.data() : NULL,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueNDRangeKernel");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Execute a kernel on a device using a single work-item - equivalent to
 * calling clEnqueueNDRangeKernel with work_dim = 1, global_work_offset = NULL,
 * global_work_size[0] set to 1, and local_work_size[0] set to 1.
 */
cl_int EnqueueTask(
    const cl_command_queue &queue,
    const cl_kernel &kernel,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueTask(
        queue,
        kernel,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueTask");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Execute native C/C++ function not compiled using the OpenCL compiler.
 */
cl_int EnqueueNativeKernel(
    const cl_command_queue &queue,
    void (*user_func)(void *),
    std::pair<void *, size_t> &args,
    std::vector<cl_mem> *mem_objects,
    std::vector<const void *> *mem_locs,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    bool has_mem_objects = (mem_objects && !mem_objects->empty());
    bool has_mem_locs = (mem_locs && !mem_locs->empty());
    bool has_event_wait_list = (event_wait_list && !event_wait_list->empty());
    cl_event tmp;
    cl_int err = clEnqueueNativeKernel(
        queue,
        user_func,
        args.first,
        args.second,
        has_mem_objects ? static_cast<cl_uint>(mem_objects->size()) : 0,
        has_mem_objects ? mem_objects->data() : NULL,
        has_mem_locs ? mem_locs->data() : NULL,
        has_event_wait_list ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        has_event_wait_list ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueNativeKernel");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/** ---------------------------------------------------------------------------
 * @brief Issues all previously queued OpenCL commands in a command-queue to the
 * device associated with the command-queue.
 */
cl_int Flush(const cl_command_queue &queue)
{
    cl_int err = clFlush(queue);
    ito_assert(err == CL_SUCCESS, "clFlush");
    return err;
}

/**
 * @brief Blocks until all previously queued OpenCL commands in a command-queue
 * are issued to the associated device and have completed.
 */
cl_int Finish(const cl_command_queue &queue)
{
    cl_int err = clFinish(queue);
    ito_assert(err == CL_SUCCESS, "clFinish");
    return err;
}

} /* cl */
} /* ito */
