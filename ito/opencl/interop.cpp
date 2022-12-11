/*
 * interop.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "interop.hpp"
#include "context.hpp"

#if defined(ITO_ENABLE_CL_GL_INTEROP)

#if defined(__APPLE__)
#include <OpenGL/CGLTypes.h>
#include <OpenGL/CGLDevice.h>   // CGLGetShareGroup
#include <OpenGL/CGLCurrent.h>  // CGLGetCurrentContext
#elif defined(__linux__)
#include <GL/glx.h>
#endif

namespace ito {
namespace cl {

/** ---------------------------------------------------------------------------
 * @brief Create a shared OpenCL/OpenGL context based on the active OpenGL
 * context associated with the specified device.
 */
cl_context CreateFromGLContext(const cl_device_id &gl_device)
{
    /* Get the Core OpenGL context object and sharegroup. */
#if defined(__APPLE__)
    CGLContextObj cgl_context = CGLGetCurrentContext();
    CGLShareGroupObj cgl_sharegroup = CGLGetShareGroup(cgl_context);

    const cl_context_properties context_properties[] = {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties) cgl_sharegroup,
        (cl_context_properties) NULL};
#elif defined(__linux__)
    const cl_context_properties context_properties[] = {
        CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
        CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
        CL_CONTEXT_PLATFORM, (cl_context_properties) platform,
        (cl_context_properties) NULL};
#endif

    /* Create the OpenCL context based on the OpenGL context. */
    cl_int err;
    cl_context context = clCreateContext(
        context_properties,         /* specify the platform to use */
        1,                          /* number of device ids */
        &gl_device,                 /* pointer to the OpenGL device */
        &ContextCallback,           /* register log callback function */
        NULL,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateContext");
    return context;
}

/**
 * @brief Create a shared OpenCL/OpenGL context based on the active OpenGL
 * context with all associated devices.
 */
cl_context CreateFromGLContext(void)
{
    /* Get the Core OpenGL context object and sharegroup. */
#if defined(__APPLE__)
    CGLContextObj cgl_context = CGLGetCurrentContext();
    CGLShareGroupObj cgl_sharegroup = CGLGetShareGroup(cgl_context);

    const cl_context_properties context_properties[] = {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties) cgl_sharegroup,
        (cl_context_properties) NULL};
#elif defined(__linux__)
    const cl_context_properties context_properties[] = {
        CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
        CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
        CL_CONTEXT_PLATFORM, (cl_context_properties) platform,
        (cl_context_properties) NULL};
#endif

    /* Create the OpenCL context based on the OpenGL context. */
    cl_int err;
    cl_context context = clCreateContext(
        context_properties,         /* specify the platform to use */
        0,                          /* number of device ids */
        0,                          /* pointer to a list of device ids */
        &ContextCallback,           /* register log callback function */
        NULL,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateContext");
    return context;
}

/** ---------------------------------------------------------------------------
 * @brief Create an OpenCL buffer object from an OpenGL buffer object.
 */
cl_mem CreateFromGLBuffer(
    const cl_context &context,
    cl_mem_flags flags,
    GLuint buffer_object)
{
    ito_assert(
        flags == CL_MEM_READ_ONLY  ||
        flags == CL_MEM_WRITE_ONLY ||
        flags == CL_MEM_READ_WRITE,
        "invalid buffer usage flags");

    cl_int err;
    cl_mem buffer = clCreateFromGLBuffer(
        context,
        flags,
        buffer_object,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateFromGLBuffer");
    return buffer;
}

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
    GLuint texture_object)
{
    ito_assert(
        flags == CL_MEM_READ_ONLY  ||
        flags == CL_MEM_WRITE_ONLY ||
        flags == CL_MEM_READ_WRITE,
        "invalid buffer usage flags");
    ito_assert(
        texture_target == GL_TEXTURE_1D ||
        texture_target == GL_TEXTURE_2D ||
        texture_target == GL_TEXTURE_3D ||
        texture_target == GL_TEXTURE_BUFFER,
        "invalid texture target");
    ito_assert(miplevel == 0, "invalid mipmap level");

    cl_int err;
    cl_mem image = clCreateFromGLTexture(
        context,
        flags,
        texture_target,
        miplevel,
        texture_object,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateFromGLBuffer");
    return image;
}

/**
 * @brief Create an OpenCL 2D image object from an OpenGL renderbuffer object.
 */
cl_mem CreateFromGLRenderbuffer(
    const cl_context &context,
    cl_mem_flags flags,
    GLuint renderbuffer)
{
    ito_assert(
        flags == CL_MEM_READ_ONLY  ||
        flags == CL_MEM_WRITE_ONLY ||
        flags == CL_MEM_READ_WRITE,
        "invalid buffer usage flags");

    cl_int err;
    cl_mem image = clCreateFromGLRenderbuffer(
        context,
        flags,
        renderbuffer,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateFromGLBuffer");
    return image;
}

/** ---------------------------------------------------------------------------
 * @brief Query a OpenGL memory object used to create an OpenCL memory object.
 */
cl_int GetGLObjectInfo(
    const cl_mem &mem_object,
    cl_gl_object_type *gl_object_type,
    GLuint *gl_object_name)
{
    cl_int err = clGetGLObjectInfo(mem_object, gl_object_type, gl_object_name);
    ito_assert(err == CL_SUCCESS, "clGetGLObjectInfo");
    return err;
}

/**
 * @brief Query a OpenGL texture object associated with an OpenCL memory object.
 */
cl_int GetGLTextureInfo(
    const cl_mem &mem_object,
    cl_gl_texture_info param_name,
    size_t param_value_size,
    void *param_value,
    size_t *param_value_size_ret)
{
    ito_assert(
        param_name == CL_GL_TEXTURE_TARGET ||
        param_name == CL_GL_MIPMAP_LEVEL,
        "invalid GL texture param name");

    cl_int err = clGetGLTextureInfo(
        mem_object,
        param_name,
        param_value_size,
        param_value,
        param_value_size_ret);
    ito_assert(err == CL_SUCCESS, "clGetGLObjectInfo");
    return err;
}

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
    cl_event *event)
{
    /*
     * Ensure any OpenGL commands that might affect the shared OpenGL memory
     * objects are finished before acquiring them.
     */
    glFinish();

    /* Acquire the shared OpenGL memory objects. */
    cl_event tmp;
    cl_int err = clEnqueueAcquireGLObjects(
        queue,
        (mem_objects != NULL) ? num_objects : 0,
        (mem_objects != NULL && num_objects > 0) ? mem_objects : NULL,
        (event_wait_list != NULL) ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        (event_wait_list != NULL && event_wait_list->size() > 0) ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueAcquireGLObjects");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Overloaded function using a vector of mem objects.
 */
cl_int EnqueueAcquireGLObjects(
    const cl_command_queue &queue,
    const std::vector<cl_mem> *mem_objects,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    return EnqueueAcquireGLObjects(
        queue,
        (mem_objects != NULL) ? static_cast<cl_uint>(mem_objects->size()) : 0,
        (mem_objects != NULL && mem_objects->size() > 0) ? mem_objects->data() : NULL,
        event_wait_list,
        event);
}

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
    cl_event *event)
{
    /*
     * Ensure any OpenCL commands that might affect the shared OpenGL memory
     * objects are finished before releasing them.
     */
    Queue::finish(queue);

    /* Release the shared OpenGL memory objects. */
    cl_event tmp;
    cl_int err = clEnqueueReleaseGLObjects(
        queue,
        (mem_objects != NULL) ? num_objects : 0,
        (mem_objects != NULL && num_objects > 0) ? mem_objects : NULL,
        (event_wait_list != NULL) ? static_cast<cl_uint>(event_wait_list->size()) : 0,
        (event_wait_list != NULL && event_wait_list->size() > 0) ? event_wait_list->data() : NULL,
        (event != NULL) ? &tmp : NULL);
    ito_assert(err == CL_SUCCESS, "clEnqueueReleaseGLObjects");

    if (event != NULL && err == CL_SUCCESS) {
        *event = tmp;
    }
    return err;
}

/**
 * @brief Overloaded function using a vector of mem objects.
 */
cl_int EnqueueReleaseGLObjects(
    const cl_command_queue &queue,
    const std::vector<cl_mem> *mem_objects,
    const std::vector<cl_event> *event_wait_list,
    cl_event *event)
{
    return EnqueueReleaseGLObjects(
        queue,
        (mem_objects != NULL) ? static_cast<cl_uint>(mem_objects->size()) : 0,
        (mem_objects != NULL && mem_objects->size() > 0) ? mem_objects->data() : NULL,
        event_wait_list,
        event);
}

} /* cl */
} /* ito */
#endif /* ITO_ENABLE_CL_GL_INTEROP */
