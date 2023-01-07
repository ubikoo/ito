/*
 * kernel.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This Kernel is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_KERNEL_H_
#define ITO_OPENCL_KERNEL_H_

#include <string>
#include "base.hpp"

namespace ito {
namespace cl {

/**
 * @brief Create a kernel object.
 */
cl_kernel CreateKernel(
    const cl_program &program,
    const std::string &kernel_name);

/**
 * @brief Release the kernel and decrement its reference count.
 */
void ReleaseKernel(const cl_kernel &kernel);

/**
 * @brief Set the value for a specific argument of the kernel.
 */
cl_int SetKernelArg(
    const cl_kernel &kernel,
    cl_uint arg_index,
    size_t arg_size,
    const void *arg_value);

} /* cl */
} /* ito */

#endif /* ITO_OPENCL_KERNEL_H_ */
