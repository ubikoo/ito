/*
 * kernel.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "kernel.hpp"

namespace ito {
namespace cl {

/**
 * @brief Create a kernel object.
 */
cl_kernel CreateKernel(
    const cl_program &program,
    const std::string &kernel_name)
{
    ito_assert(!kernel_name.empty(), "empty kernel name string");

    cl_int err;
    cl_kernel kernel = clCreateKernel(program, kernel_name.c_str(), &err);
    ito_assert(err == CL_SUCCESS, "clCreateKernel");
    ito_assert(kernel != NULL, "null kernel");

    return kernel;
}

/**
 * @brief Decrements the kernel reference count.
 */
cl_int ReleaseKernel(const cl_kernel &kernel)
{
    cl_int err = CL_INVALID_VALUE;
    if (kernel != NULL) {
        err = clReleaseKernel(kernel);
        ito_assert(err == CL_SUCCESS, "clReleaseKernel");
    }
    return err;
}

/**
 * @brief Set the value for a specific argument of the kernel.
 * @param arg_index The argument index going from 0 for the leftmost argument
 * to (n-1) where n is the total number of arguments.
 * @param arg_size Size of the argument value:
 *  __local qualified: number of bytes required for the buffer used to
 *  store the local argument.
 *  memory object: size of the object, e.g. sizeof(cl_mem).
 *  sampler object: size of the object, e.g. sizeof(cl_sampler).
 *  regular type: size of the argument type, e.g. sizeof(cl_int).
 * @param arg_value Pointer to the argument to be passed to the kernel. Data
 * pointed by arg_value is copied and the arg_value pointer can then be reused
 * by the application after clSetKernelArg returns.
 */
cl_int SetKernelArg(
    const cl_kernel &kernel,
    cl_uint arg_index,
    size_t arg_size,
    const void *arg_value)
{
    cl_int err = clSetKernelArg(kernel, arg_index, arg_size, arg_value);
    ito_assert(err == CL_SUCCESS, "clSetKernelArg");
    return err;
}

} /* cl */
} /* ito */
