/*
 * interop.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_INTEROP_H_
#define ITO_OPENCL_INTEROP_H_

#include <vector>
#include "base.hpp"

#if defined(ITO_ENABLE_CL_GL_INTEROP)
namespace ito {
namespace cl {

/** ---------------------------------------------------------------------------
 * @brief Create a shared OpenCL/OpenGL context based on the active OpenGL
 * context associated with the specified device.
 */
cl_context CreateFromGLContext(const cl_device_id &gl_device);

/**
 * @brief Create a shared OpenCL/OpenGL context based on the active OpenGL
 * context with all associated devices.
 */
cl_context CreateFromGLContext(void);

/** ---------------------------------------------------------------------------
 * @brief Create an OpenCL buffer object from an OpenGL buffer object.
 */
cl_mem CreateFromGLBuffer(
    const cl_context &context,
    cl_mem_flags flags,
    GLuint buffer_object);

/**
 * @brief Create an OpenCL image object, image array object, or image buffer
 * object from an OpenGL texture object, texture array object, texture buffer
 * object, or a single face of an OpenGL cubemap texture object.
 */
cl_mem CreateFromGLTexture(
    const cl_context &context,
  	cl_mem_flags flags,
    GLenum texture_target,
    GLint miplevel,
    GLuint texture_object);

/**
 * @brief Create an OpenCL 2D image object from an OpenGL renderbuffer object.
 */
cl_mem CreateFromGLRenderbuffer(
    const cl_context &context,
    cl_mem_flags flags,
    GLuint renderbuffer);

/** ---------------------------------------------------------------------------
 * @brief Query a OpenGL memory object used to create an OpenCL memory object.
 */
cl_int GetGLObjectInfo(
    const cl_mem &mem_object,
    cl_gl_object_type *gl_object_type,
    GLuint *gl_object_name);

/**
 * @brief Query a OpenGL texture object associated with an OpenCL memory object.
 */
cl_int GetGLTextureInfo(
    const cl_mem &mem_object,
    cl_gl_texture_info param_name,
    size_t param_value_size,
    void *param_value,
    size_t *param_value_size_ret);

/** ---------------------------------------------------------------------------
 * @brief Acquire OpenCL memory objects that were created from OpenGL objects.
 * @return CL_INVALID_VALUE if num_objects is zero and mem_objects is not a
 * NULL value or if num_objects > 0 and mem_objects is NULL.
 */
cl_int EnqueueAcquireGLObjects(
    const cl_command_queue &queue,
    const cl_uint num_objects,
    const cl_mem *mem_objects,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event);

/**
 * @brief Overloaded function using a vector of mem objects.
 */
cl_int EnqueueAcquireGLObjects(
    const cl_command_queue &queue,
    const std::vector<cl_mem> *mem_objects,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event);

/**
 * @brief Release OpenCL memory objects that were created from OpenGL objects.
 * @return CL_INVALID_VALUE if num_objects is zero and mem_objects is not a
 * NULL value or if num_objects > 0 and mem_objects is NULL.
 */
cl_int EnqueueReleaseGLObjects(
    const cl_command_queue &queue,
    const cl_uint num_objects,
    const cl_mem *mem_objects,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event);
/**
 * @brief Overloaded function using a vector of mem objects.
 */
cl_int EnqueueReleaseGLObjects(
    const cl_command_queue &queue,
    const std::vector<cl_mem> *mem_objects,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event);

} /* cl */
} /* ito */
#endif /* ITO_ENABLE_CL_GL_INTEROP */

#endif /* ITO_OPENCL_INTEROP_H_ */

