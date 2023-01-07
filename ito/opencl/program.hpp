/*
 * program.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_PROGRAM_H_
#define ITO_OPENCL_PROGRAM_H_

#include <string>
#include <vector>
#include "base.hpp"

namespace ito {
namespace cl {

/**
 * @brief Load program source from the specified filename.
 */
std::string LoadProgramSource(const std::string &filename);

/**
 * @brief Create a program object from the specified source code string.
 */
cl_program CreateProgramWithSource(
    const cl_context &context,
    const std::string &source);

/**
 * @brief Create a program object from the specified source filename.
 */
cl_program CreateProgramFromFile(
    const cl_context &context,
    const std::string &filename);

/**
 * @brief Release the program object and decrement its reference count.
 */
void ReleaseProgram(const cl_program &program);

/**
 * @brief Build the program object for the specified device. Options parameter
 * specifies build options, including preprocessor defines, optimization and
 * code generation options.
 */
cl_int BuildProgram(
    const cl_program &program,
    const cl_device_id &device,
    const std::string &options = "");

/**
 * @brief Query CL_PROGRAM_NUM_DEVICES and return the number of devices
 * associated with program.
 */
cl_uint GetProgramNumDevices(const cl_program &program);

/**
 * @brief Query CL_PROGRAM_DEVICES and return the list of devices associated
 * with the program object.
 */
std::vector<cl_device_id> GetProgramDevices(const cl_program &program);

/**
 * @brief Query CL_PROGRAM_SOURCE and return the program source code specified
 * by clCreateProgramWithSource.
 */
std::string GetProgramSource(const cl_program &program);

/**
 * @brief Query CL_PROGRAM_NUM_KERNELS and return the number of kernels declared
 * in program that can be created with clCreateKernel.
 */
size_t GetProgramNumKernels(const cl_program &program);

/**
 * @brief Query CL_PROGRAM_KERNEL_NAMES and returns a semi-colon separated list
 * of kernel names in program that can be created with clCreateKernel.
 */
std::string GetProgramKernelNames(const cl_program &program);

} /* cl */
} /* ito */

#endif /* ITO_OPENCL_PROGRAM_H_ */
