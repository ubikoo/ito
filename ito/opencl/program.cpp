/*
 * program.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include <fstream>
#include "program.hpp"

namespace ito {
namespace cl {

/**
 * @brief Load program source from the specified filename.
 */
std::string LoadProgramSource(const std::string &filename)
{
    std::ifstream file(filename);
    ito_assert(file, "failed to open program source file");

    std::stringstream ss(std::ios::out);
    ss << file.rdbuf();
    file.close();

    return ss.str();
}

/**
 * @brief Create a program object from the specified source code string.
 */
cl_program CreateProgramWithSource(
    const cl_context &context,
    const std::string &source)
{
    ito_assert(!source.empty(), "empty source string");

    cl_int err;
    const char *src = source.c_str();
    size_t length = source.length();
    cl_program program = clCreateProgramWithSource(
        context,        /* OpenCL context */
        1,              /* one null-terminated character string */
        &src,           /* pointer to the character string */
        &length,        /* length of the character string */
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateProgramWithSource");
    ito_assert(program != NULL, "null cl_program");

    return program;
}

/**
 * @brief Create a program object from the specified source filename.
 */
cl_program CreateProgramFromFile(
    const cl_context &context,
    const std::string &filename)
{
    return CreateProgramWithSource(context, LoadProgramSource(filename));
}

/**
 * @brief Release the program object and decrement its reference count.
 */
void ReleaseProgram(const cl_program &program)
{
    cl_int err = clReleaseProgram(program);
    ito_assert(err == CL_SUCCESS, "clReleaseProgram");
}

/**
 * @brief Build the program object for the specified device. Options parameter
 * specifies build options, including preprocessor defines, optimization and
 * code generation options.
 */
cl_int BuildProgram(
    const cl_program &program,
    const cl_device_id &device,
    const std::string &options)
{
    cl_int err = clBuildProgram(
        program,
        1,
        &device,
        options.c_str(),
        NULL,               /* no callback, wait until completion */
        NULL);              /* don't pass callback user data */

    if (err != CL_SUCCESS) {
        size_t infolen;
        clGetProgramBuildInfo(
            program,
            device,
            CL_PROGRAM_BUILD_LOG,
            0,
            NULL,
            &infolen);

        std::string infolog(infolen+1, '\0');
        clGetProgramBuildInfo(
            program,
            device,
            CL_PROGRAM_BUILD_LOG,
            infolen,
            (void *) &infolog[0],
            NULL);

        ito_assert(err == CL_SUCCESS,
            std::string("failed to link program:\n\n") + infolog);
    }

    return err;
}

/**
 * @brief Query CL_PROGRAM_NUM_DEVICES and return the number of devices
 * associated with program.
 */
cl_uint GetProgramNumDevices(const cl_program &program)
{
    cl_uint num_devices;

    cl_int err = clGetProgramInfo(
        program,
  	    CL_PROGRAM_NUM_DEVICES,
        sizeof(cl_uint),
        &num_devices,
        NULL);
    ito_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_NUM_DEVICES");

    return num_devices;
}

/**
 * @brief Query CL_PROGRAM_DEVICES and return the list of devices associated
 * with the program object.
 */
std::vector<cl_device_id> GetProgramDevices(const cl_program &program)
{
    cl_int err;

    size_t param_value_size;
    err = clGetProgramInfo(
        program,
  	    CL_PROGRAM_DEVICES,
        0,
        NULL,
        &param_value_size);
    ito_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_DEVICES");

    size_t num_devices = param_value_size / sizeof(cl_device_id);
    ito_assert(num_devices > 0, "invalid num_devices");

    std::vector<cl_device_id> devices(num_devices);
    err = clGetProgramInfo(
        program,
  	    CL_PROGRAM_DEVICES,
        param_value_size,
        (void *) &devices[0],
        NULL);
    ito_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_DEVICES");

    return devices;
}

/**
 * @brief Query CL_PROGRAM_SOURCE and return the program source code specified
 * by clCreateProgramWithSource.
 */
std::string GetProgramSource(const cl_program &program)
{
    cl_int err;

    size_t param_value_size;
    err = clGetProgramInfo(
        program,
        CL_PROGRAM_SOURCE,
        0,
        NULL,
        &param_value_size);
    ito_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_SOURCE");

    size_t num_source_chars = param_value_size / sizeof(char);
    ito_assert(num_source_chars > 0, "invalid num_source_chars");

    std::string source(num_source_chars, '\0');
    err = clGetProgramInfo(
        program,
        CL_PROGRAM_SOURCE,
        param_value_size,
        (void *) &source[0],
        NULL);
    ito_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_SOURCE");

    return source;
}

/**
 * @brief Query CL_PROGRAM_NUM_KERNELS and return the number of kernels declared
 * in program that can be created with clCreateKernel.
 */
size_t GetProgramNumKernels(const cl_program &program)
{
    cl_uint num_kernels;

    cl_int err = clGetProgramInfo(
        program,
  	    CL_PROGRAM_NUM_KERNELS,
        sizeof(size_t),
        &num_kernels,
        NULL);
    ito_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_NUM_KERNELS");

    return num_kernels;
}

/**
 * @brief Query CL_PROGRAM_KERNEL_NAMES and returns a semi-colon separated list
 * of kernel names in program that can be created with clCreateKernel.
 */
std::string GetProgramKernelNames(const cl_program &program)
{
    cl_int err;

    size_t param_value_size;
    err = clGetProgramInfo(
        program,
        CL_PROGRAM_KERNEL_NAMES,
        0,
        NULL,
        &param_value_size);
    ito_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_KERNEL_NAMES");

    size_t num_kernel_names_chars = param_value_size / sizeof(char);
    ito_assert(num_kernel_names_chars > 0, "invalid num_kernel_names_chars");

    std::string kernel_names(num_kernel_names_chars, '\0');
    err = clGetProgramInfo(
        program,
        CL_PROGRAM_KERNEL_NAMES,
        param_value_size,
        (void *) &kernel_names[0],
        NULL);
    ito_assert(err == CL_SUCCESS, "clGetProgramInfo CL_PROGRAM_KERNEL_NAMES");

    return kernel_names;
}

} /* cl */
} /* ito */
