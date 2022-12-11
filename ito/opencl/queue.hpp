/*
 * queue.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_QUEUE_H_
#define ITO_OPENCL_QUEUE_H_

#include <array>
#include <vector>
#include <utility>
#include "base.hpp"
#include "ndrange.hpp"

namespace ito {
namespace cl {

/** ---------------------------------------------------------------------------
 * @brief Create a command_queue on the specified device.
 */
cl_command_queue CreateCommandQueue(
    const cl_context &context,
    const cl_device_id &device,
    cl_command_queue_properties properties = 0);

/**
 * @brief Decrement the queue reference count.
 */
cl_int ReleaseCommandQueue(const cl_command_queue &queue);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

/** ---------------------------------------------------------------------------
 * @brief Copy data from the specified buffer object to host memory.
 */
cl_int EnqueueCopyFrom(
    const cl_command_queue &queue,
    cl_mem &buffer,
    size_t size,
    void *ptr,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

/**
 * @brief Copy an data from the host memory to the specified buffer object.
 */
cl_int EnqueueCopyTo(
    const cl_command_queue &queue,
    cl_mem &buffer,
    size_t size,
    void *ptr,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL,
    cl_int *errcode = NULL);

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
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL,
    cl_int *errcode = NULL);

/**
 * @brief Unmap a previously mapped region of a memory object.
 */
cl_int EnqueueUnmapMemObject(
    const cl_command_queue &queue,
    cl_mem &mem_object,
    void *mapped_ptr,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

/**
 * @brief Indicate the device a set of memory objects should be associated with.
 */
cl_int EnqueueMigrateMemObjects(
    const cl_command_queue &queue,
    const std::vector<cl_mem> &mem_objects,
    cl_mem_migration_flags flags,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

/** ---------------------------------------------------------------------------
 * @brief Execute a marker command which waits for either a list of events to
 * complete, or all previously enqueued commands to complete.
 */
cl_int EnqueueMarkerWithWaitList(
    const cl_command_queue &queue,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

/**
 * @brief Execute a barrier command which waits for either a list of events to
 * complete, or if the list is empty, waits for all commands previously enqueued
 * in command_queue to complete before it completes.
 */
cl_int EnqueueBarrierWithWaitList(
    const cl_command_queue &queue,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

/** ---------------------------------------------------------------------------
 * @brief Execute a kernel on a device.
 */
cl_int EnqueueNDRangeKernel(
    const cl_command_queue &queue,
    const cl_kernel &kernel,
    const NDRange &global_work_offset,
    const NDRange &global_work_size,
    const NDRange &local_work_size,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

/**
 * @brief Execute a kernel on a device using a single work-item - equivalent to
 * calling clEnqueueNDRangeKernel with work_dim = 1, global_work_offset = NULL,
 * global_work_size[0] set to 1, and local_work_size[0] set to 1.
 */
cl_int EnqueueTask(
    const cl_command_queue &queue,
    const cl_kernel &kernel,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

/**
 * @brief Execute native C/C++ function not compiled using the OpenCL compiler.
 */
cl_int EnqueueNativeKernel(
    const cl_command_queue &queue,
    void (*user_func)(void *),
    std::pair<void *, size_t> &args,
    std::vector<cl_mem> *mem_objects,
    std::vector<const void *> *mem_locs,
    const std::vector<cl_event> *event_wait_list = NULL,
    cl_event *event = NULL);

/** ---------------------------------------------------------------------------
 * @brief Issues all previously queued OpenCL commands in a command-queue to the
 * device associated with the command-queue.
 */
cl_int Flush(const cl_command_queue &queue);

/**
 * @brief Blocks until all previously queued OpenCL commands in a command-queue
 * are issued to the associated device and have completed.
 */
cl_int Finish(const cl_command_queue &queue);

} /* cl */
} /* ito */

#endif /* ITO_OPENCL_QUEUE_H_ */
