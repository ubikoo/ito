/*
 * clfw.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_CLFW_H_
#define ITO_OPENCL_CLFW_H_

#include "base.hpp"

namespace ito {
namespace clfw {

/** @brief Setup OpenCL context with a command queue on the specified device. */
void Init(
    const cl_device_type device_type,
    const size_t device_index,
    cl_command_queue_properties queue_properties = 0);

#if defined(ITO_ENABLE_CL_GL_INTEROP)
/** @brief Setup OpenCL context based on the OpenGL context. */
void InitFromGLContext(
    const size_t device_index,
    cl_command_queue_properties queue_properties = 0);
#endif /* ITO_ENABLE_CL_GL_INTEROP */

/** @brief Release the OpenCL context, and the command queue on the device. */
void Terminate(void);

/** @brief Is the OpenCL context initialized? */
bool IsInit(void);

/** @brief Return a reference to OpenCL context. */
cl_context &Context(void);

/** @brief Return a reference to OpenCL device. */
cl_device_id &Device(void);

/** @brief Return a reference to OpenCL command queue. */
cl_command_queue &Queue(void);

/** @brief Return a string with OpenCL information. */
const std::string &InfoString();

} /* clfw */
} /* ito */

#endif /* ITO_OPENCL_CLFW_H_ */
