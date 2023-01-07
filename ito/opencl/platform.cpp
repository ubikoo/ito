/*
 * platform.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "platform.hpp"

namespace ito {
namespace cl {

/**
 * @brief Return a vector of all platforms currently available.
 */
std::vector<cl_platform_id> GetPlatformIDs(void)
{
    /* Get the list of platform ids. */
    cl_int err;
    cl_uint n_platforms = 0;
    err = clGetPlatformIDs(0, NULL, &n_platforms);
    ito_assert(err == CL_SUCCESS, "clGetPlatformIDs");
    ito_assert(n_platforms > 0, "invalid number of platforms");

    std::vector<cl_platform_id> platforms(n_platforms);
    err = clGetPlatformIDs(n_platforms, platforms.data(), NULL);
    ito_assert(err == CL_SUCCESS, "clGetPlatformIDs");

    return platforms;
}

/**
 * @brief Return a string with basic platform information.
 */
std::string GetPlatformInfoString(const cl_platform_id &platform)
{
    /**
     * get_platform_info
     */
    auto get_platform_info = [&] (
        cl_platform_info param_name,
        const std::string param_string) -> std::string {
        cl_int err;
        size_t size;
        err = clGetPlatformInfo(platform, param_name, 0, NULL, &size);
        ito_assert(err == CL_SUCCESS, "clGetPlatformInfo");
        ito_assert(size > 0, "invalid parameter size");

        std::string info(size, '\0');
        err = clGetPlatformInfo(platform, param_name, size, &info[0], NULL);
        ito_assert(err == CL_SUCCESS, "clGetPlatformInfo");

        return (param_string + " " + info + "\n");
    };

    /* Get platform name string. */
    std::ostringstream ss;
    ss << get_platform_info(CL_PLATFORM_NAME, "CL_PLATFORM_NAME");
    ss << get_platform_info(CL_PLATFORM_VENDOR, "CL_PLATFORM_VENDOR");
    ss << get_platform_info(CL_PLATFORM_VERSION, "CL_PLATFORM_VERSION");

    return ss.str();
}

} /* cl */
} /* ito */
