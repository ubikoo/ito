/*
 * context.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "context.hpp"
#include "platform.hpp"
#include "device.hpp"

namespace ito {
namespace cl {

/**
 * @brief Context error callback function.
 */
static void CL_CALLBACK ContextCallback(
    const char *error_info,
    const void *private_info,
    size_t cb,
    void *user_data)
{
    std::cerr << "OpenCL context error: " << error_info << std::endl;
}

/**
 * @brief Create a context with the list of devices on the specified platform.
 * @note Context properties can be NULL in which case the platform that is
 * selected is implementation-defined.
 */
cl_context CreateContext(
    const cl_platform_id &platform,
    const std::vector<cl_device_id> &devices)
{
    ito_assert(!devices.empty(), "empty device list");

    const cl_context_properties context_properties[] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties) platform,
        (cl_context_properties) NULL};

    cl_int err;
    cl_context context = clCreateContext(
        context_properties,         /* specify the platform to use */
        (cl_uint) devices.size(),   /* number of device ids */
        devices.data(),             /* pointer to a list of device ids */
        &ContextCallback,           /* register log callback function */
        NULL,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateContext");
    return context;
}

/**
 * @brief Create a context with all devices of the specified type in the first
 * available platform.
 */
cl_context CreateContext(cl_device_type type)
{
    std::vector<cl_platform_id> platforms = GetPlatformIDs();
    std::vector<cl_device_id> devices = GetDeviceIDs(platforms[0], type);
    return CreateContext(platforms[0], devices);
}

/**
 * @brief Release the context and decrement its reference count.
 */
void ReleaseContext(const cl_context &context)
{
    cl_int err = clReleaseContext(context);
    ito_assert(err == CL_SUCCESS, "clReleaseContext");
}

/**
 * @brief Return a list of devices in the context.
 */
std::vector<cl_device_id> GetContextDevices(const cl_context &context)
{
    cl_int err;

    /* Get the list of devices in the context. */
    size_t size;
    err = clGetContextInfo(
        context,
        CL_CONTEXT_DEVICES,
        0,
        NULL,
        &size);
    ito_assert(err == CL_SUCCESS, "clGetContextInfo");

    size_t n_devices = size / sizeof(cl_device_id);
    std::vector<cl_device_id> devices(n_devices);
    err = clGetContextInfo(
        context,
        CL_CONTEXT_DEVICES,
        n_devices * sizeof(cl_device_id),
        (void *) devices.data(),
        NULL);
    ito_assert(err == CL_SUCCESS, "clGetContextInfo");

    return devices;
}

/**
 * @brief Return the device with the specified index in the context.
 */
cl_device_id GetContextDevice(const cl_context &context, const size_t index)
{
    std::vector<cl_device_id> devices = GetContextDevices(context);
    ito_assert(index < devices.size(), "device index out of range");
    return devices[index];
}

} /* cl */
} /* ito */
