/*
 * clfw.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include <string>
#include <vector>
#include "context.hpp"
#include "device.hpp"
#include "queue.hpp"
#include "interop.hpp"
#include "clfw.hpp"

/** ---------------------------------------------------------------------------
 * @brief Interface to OpenCL. Maintain an OpenCL context with a command queue
 * on a specified device.
 *
 * For simplicity the default platform is the first one, ie platform id 0.
 * If necessary, it is trivial to make that value a function argument.
 *
 * The default initialization function takes two arguments specifying the type
 * and the index of the device.
 *
 * The alternative initialization function takes the index of the GPU device
 * associated with the OpenGL context and creates a shared context.
 *
 * The termination function takes care of releasing the queue, the associated
 * device and the OpenCL context.
 */

namespace ito {
namespace clfw {

static cl_context gContext = NULL;
static cl_device_id gDevice = NULL;
static cl_command_queue gQueue = NULL;
static std::string gInfoString;

/**
 * @brief Setup OpenCL context with a command queue on the specified device.
 * @param type identifies the type of OpenCL device (CL_​DEVICE_​TYPE_​CPU,
 * CL_DEVICE_TYPE_GPU or CL_DEVICE_TYPE_ALL).
 * @param index specifies the index of the device in the context.
 */
void Init(
    const cl_device_type device_type,
    const size_t device_index,
    cl_command_queue_properties queue_properties)
{
    ito_assert(!IsInit(), "OpenCL context is already initialized");

    gContext = cl::CreateContext(device_type);
    gDevice = cl::GetContextDevice(gContext, device_index);
    gQueue = cl::CreateCommandQueue(gContext, gDevice, queue_properties);
    gInfoString = cl::GetDeviceInfoString(gDevice);
}

#if defined(ITO_ENABLE_CL_GL_INTEROP)
/**
 * @brief Setup OpenCL context with a command queue based on the OpenGL context
 * in the specified device.
 */
void InitFromGLContext(
    const size_t device_index,
    cl_command_queue_properties queue_properties)
{
    ito_assert(!IsInit(), "OpenCL context is already initialized");

    std::vector<cl_device_id> devices = cl::GetDeviceIDs(CL_DEVICE_TYPE_GPU);
    ito_assert(device_index < devices.size(), "device index overflow");

    gDevice = devices[device_index];
    gContext = cl::CreateFromGLContext(gDevice);
    gQueue = cl::CreateCommandQueue(gContext, gDevice, queue_properties);
    gInfoString = cl::GetDeviceInfoString(gDevice);
}
#endif /* ITO_ENABLE_CL_GL_INTEROP */

/**
 * @brief Release the OpenCL context, and the command queue on its assiated
 * device by decreasing their reference count.
 */
void Terminate(void)
{
    ito_assert(IsInit(), "OpenCL context is not initialized");

    cl::ReleaseCommandQueue(gQueue);
    cl::ReleaseDevice(gDevice);
    cl::ReleaseContext(gContext);

    gContext = NULL;
    gDevice = NULL;
    gQueue = NULL;
    gInfoString = {};
}

/**
 * @brief Is the OpenCL context initialized?
 */
bool IsInit(void)
{
    return (gContext != NULL && gDevice != NULL && gQueue != NULL);
}

/**
 * @brief Return a reference to OpenCL context.
 */
cl_context &Context(void)
{
    return gContext;
}

/**
 * @brief Return a reference to OpenCL device.
 */
cl_device_id &Device(void)
{
    return gDevice;
}

/**
 * @brief Return a reference to OpenCL command queue.
 */
cl_command_queue &Queue(void)
{
    return gQueue;
}

/**
 * @brief Return a string with OpenCL information.
 */
const std::string &InfoString()
{
    return gInfoString;
}

} /* clfw */
} /* ito */
