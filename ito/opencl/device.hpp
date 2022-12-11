/*
 * device.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_DEVICE_H_
#define ITO_OPENCL_DEVICE_H_

#include <vector>
#include "base.hpp"

namespace ito {
namespace cl {

/**
 * @brief Return a vector with the ids of all devices of a specified type in
 * the specified platform.
 */
std::vector<cl_device_id> GetDeviceIDs(
    const cl_platform_id &platform,
    cl_device_type type);

/**
 * @brief Create a vector of all devices of a given type currently available in
 * the first available platform.
 */
std::vector<cl_device_id> GetDeviceIDs(cl_device_type type);

/**
 * @brief Decrement the device reference count.
 */
cl_int ReleaseDevice(const cl_device_id &device);

/**
 * @brief Return a string with device information.
 */
std::string GetDeviceInfoStr(const cl_device_id &device);

} /* cl */
} /* ito */

#endif /* ITO_OPENCL_DEVICE_H_ */
