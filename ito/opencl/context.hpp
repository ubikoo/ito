/*
 * context.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_CONTEXT_H_
#define ITO_OPENCL_CONTEXT_H_

#include <string>
#include <vector>
#include "base.hpp"

namespace ito {
namespace cl {

/**
 * @brief Create a context with the list of devices on the specified platform.
 */
cl_context CreateContext(
    const cl_platform_id &platform,
    const std::vector<cl_device_id> &devices);

/**
 * @brief Create a context with all devices of the specified type in the first
 * available platform.
 */
cl_context CreateContext(cl_device_type type);

/**
 * @brief Release the context and decrement its reference count.
 */
void ReleaseContext(const cl_context &context);

/**
 * @brief Return a list of devices in the context.
 */
std::vector<cl_device_id> GetContextDevices(const cl_context &context);

/**
 * @brief Return the device with the specified index in the context.
 */
cl_device_id GetContextDevice(const cl_context &context, const size_t index);

} /* cl */
} /* ito */

#endif /* ITO_OPENCL_CONTEXT_H_ */
