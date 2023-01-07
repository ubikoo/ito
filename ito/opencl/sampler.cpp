/*
 * sampler.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "sampler.hpp"

namespace ito {
namespace cl {

/**
 * @brief Create a sampler object.
 * @param normalized_coords Determine if image coordinates are normalized
 * (CL_TRUE) or not (CL_FALSE).
 * @param addressing_mode Specify how out-of-range image coordinates are
 * handled when reading from an image:
 *  CL_ADDRESS_MIRRORED_REPEAT
 *  CL_ADDRESS_REPEAT
 *  CL_ADDRESS_CLAMP_TO_EDGE
 *  CL_ADDRESS_CLAMP
 *  CL_ADDRESS_NONE
 * @param filter_mode Specify the filter type when reading an image:
 *  CL_FILTER_NEAREST or CL_FILTER_LINEAR.
 */
cl_sampler CreateSampler(
    const cl_context &context,
    cl_bool normalized_coords,
    cl_addressing_mode addressing_mode,
    cl_filter_mode filter_mode)
{
    cl_int err;
    cl_sampler sampler = clCreateSampler(
        context,
        normalized_coords,
        addressing_mode,
        filter_mode,
        &err);
    ito_assert(err == CL_SUCCESS, "clCreateSampler");
    return sampler;
}

/**
 * @brief Release the sampler object and decrement its reference count.
 */
void ReleaseSampler(const cl_sampler &sampler)
{
    cl_int err = clReleaseSampler(sampler);
    ito_assert(err == CL_SUCCESS, " clReleaseSampler");
}

} /* cl */
} /* ito */
