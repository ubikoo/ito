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

#include "device.hpp"
#include "platform.hpp"

namespace ito {
namespace cl {

/**
 * @brief Return a vector with the ids of all devices of a specified type in
 * the specified platform.
 */
std::vector<cl_device_id> GetDeviceIDs(
    const cl_platform_id &platform,
    cl_device_type type)
{
    /* Get the list of devices of the given type available on the platform. */
    cl_int err;

    cl_uint n_devices = 0;
    err = clGetDeviceIDs(platform, type, 0, NULL, &n_devices);
    ito_assert(err == CL_SUCCESS, "clGetDeviceIDs");

    ito_assert(n_devices > 0, "invalid number of devices");
    std::vector<cl_device_id> devices(n_devices);

    err = clGetDeviceIDs(platform, type, n_devices, devices.data(), NULL);
    ito_assert(err == CL_SUCCESS, "clGetDeviceIDs");

    return devices;
}

/**
 * @brief Create a vector of all devices of a given type currently available in
 * the first available platform.
 */
std::vector<cl_device_id> GetDeviceIDs(cl_device_type type)
{
    std::vector<cl_platform_id> platforms = GetPlatformIDs();
    return GetDeviceIDs(platforms[0], type);
}

/**
 * @brief Decrement the device reference count.
 */
cl_int ReleaseDevice(const cl_device_id &device)
{
    cl_int err = CL_INVALID_VALUE;
    if (device != NULL) {
        err = clReleaseDevice(device);
        ito_assert(err == CL_SUCCESS, "clReleaseDevice");
    }
    return err;
}

/**
 * @brief Return a string with device information.
 */
std::string GetDeviceInfoStr(const cl_device_id &device)
{
    std::ostringstream ss;
    cl_int err;

    /*
     * CL_DEVICE_TYPE
     */
    {
        cl_device_type type;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_TYPE,
            sizeof(cl_device_type),
            &type,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        ss << (type == CL_DEVICE_TYPE_CPU ? "CL_DEVICE_TYPE_CPU" :
               type == CL_DEVICE_TYPE_GPU ? "CL_DEVICE_TYPE_GPU" :
               type == CL_DEVICE_TYPE_ALL ? "CL_DEVICE_TYPE_ALL" :
               "CL_DEVICE_TYPE_UNKOWN");
        ss << "\n";
    }

    /*
     * CL_DEVICE_VENDOR
     */
    {
        size_t param_value_size;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_VENDOR,
            0,
            NULL,
            &param_value_size);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        size_t num_param_values = param_value_size / sizeof(char);
        ito_assert(num_param_values > 0, "invalid num_param_values");

        std::string param_value(num_param_values, '\0');
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_VENDOR,
            param_value_size,
            (void *) &param_value[0],
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        ss << "vendor "  << param_value  << "\n";
    }

    /*
     * CL_DEVICE_VERSION
     */
    {
        size_t param_value_size;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_VERSION,
            0,
            NULL,
            &param_value_size);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        size_t num_param_values = param_value_size / sizeof(char);
        ito_assert(num_param_values > 0, "invalid num_param_values");

        std::string param_value(num_param_values, '\0');
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_VERSION,
            param_value_size,
            (void *) &param_value[0],
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        ss << "version " << param_value << "\n";
    }

    /*
     * CL_DEVICE_GLOBAL_MEM_SIZE
     * CL_DEVICE_LOCAL_MEM_SIZE
     */
    {
        cl_ulong global_mem_size;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_GLOBAL_MEM_SIZE,
            sizeof(cl_ulong),
            &global_mem_size,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        cl_ulong local_mem_size;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_LOCAL_MEM_SIZE,
            sizeof(cl_ulong),
            &local_mem_size,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        ss << "global_mem_size " << global_mem_size << "\n";
        ss << "local_mem_size " << local_mem_size << "\n";
    }

    /*
     * CL_DEVICE_MAX_MEM_ALLOC_SIZE
     * CL_DEVICE_MAX_COMPUTE_UNITS
     */
    {
        cl_ulong max_mem_alloc_size;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_MAX_MEM_ALLOC_SIZE,
            sizeof(cl_ulong),
            &max_mem_alloc_size,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        cl_uint max_compute_units;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_MAX_COMPUTE_UNITS,
            sizeof(cl_uint),
            &max_compute_units,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        ss << "max_mem_alloc_size " << max_mem_alloc_size << "\n";
        ss << "max_compute_units " << max_compute_units << "\n";
    }

    /*
     * CL_DEVICE_MAX_WORK_GROUP_SIZE
     * CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS
     * CL_DEVICE_MAX_WORK_ITEM_SIZES
     */
    {
        size_t max_work_group_size;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_MAX_WORK_GROUP_SIZE,
            sizeof(size_t),
            &max_work_group_size,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        cl_uint max_work_item_dimensions;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
            sizeof(cl_uint),
            &max_work_item_dimensions,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        std::vector<size_t> max_work_item_sizes(max_work_item_dimensions);
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_MAX_WORK_ITEM_SIZES,
            max_work_item_dimensions * sizeof(size_t),
            (void *) &max_work_item_sizes[0],
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        ss << "max_work_group_size " << max_work_group_size << "\n";
        ss << "max_work_item_dimensions " << max_work_item_dimensions << "\n";
        ss << "max_work_item_sizes ";
        for (auto &it : max_work_item_sizes) {
            ss << it << " ";
        }
        ss << "\n";
    }

    /*
     * CL_DEVICE_IMAGE_SUPPORT
     */
    {
        cl_bool image_support;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_IMAGE_SUPPORT,
            sizeof(cl_bool),
            &image_support,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        ss << (image_support ? "image supported\n" : "image not supported\n");
    }

    /*
     * CL_DEVICE_IMAGE2D_MAX_WIDTH,
     * CL_DEVICE_IMAGE2D_MAX_HEIGHT
     */
    {
        size_t image2d_max_width;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_IMAGE2D_MAX_WIDTH,
            sizeof(size_t),
            &image2d_max_width,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        size_t image2d_max_height;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_IMAGE2D_MAX_HEIGHT,
            sizeof(size_t),
            &image2d_max_height,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        ss << "image2d_max_width "
           << std::to_string(image2d_max_width)  << "\n";
        ss << "image2d_max_height "
           << std::to_string(image2d_max_height) << "\n";
    }

    /*
     * CL_DEVICE_IMAGE3D_MAX_WIDTH,
     * CL_DEVICE_IMAGE3D_MAX_HEIGHT,
     * CL_DEVICE_IMAGE3D_MAX_DEPTH
     */
    {
        size_t image3d_max_width;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_IMAGE3D_MAX_WIDTH,
            sizeof(size_t),
            &image3d_max_width,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        size_t image3d_max_height;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_IMAGE3D_MAX_HEIGHT,
            sizeof(size_t),
            &image3d_max_height,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        size_t image3d_max_depth;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_IMAGE3D_MAX_DEPTH,
            sizeof(size_t),
            &image3d_max_depth,
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        ss << "image3d_max_width "
           << std::to_string(image3d_max_width)  << "\n";
        ss << "image3d_max_height "
           << std::to_string(image3d_max_height) << "\n";
        ss << "image3d_max_depth "
           << std::to_string(image3d_max_depth)  << "\n";
    }

    /*
     * CL_DEVICE_EXTENSIONS
     */
    {
        size_t param_value_size;
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_EXTENSIONS,
            0,
            NULL,
            &param_value_size);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        size_t num_param_values = param_value_size / sizeof(char);
        ito_assert(num_param_values > 0, "invalid num_param_values");

        std::string param_value(num_param_values, '\0');
        err = clGetDeviceInfo(
            device,
            CL_DEVICE_EXTENSIONS,
            param_value_size,
            (void *) &param_value[0],
            NULL);
        ito_assert(err == CL_SUCCESS, "clGetDeviceInfo");

        ss << "extensions "  << param_value  << "\n";
    }

    return ss.str();
}

} /* cl */
} /* ito */
