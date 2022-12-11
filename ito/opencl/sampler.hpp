/*
 * sampler.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_SAMPLER_H_
#define ITO_OPENCL_SAMPLER_H_

#include "base.hpp"

namespace ito {
namespace cl {

/**
 * @brief Create a sampler object.
 */
cl_sampler CreateSampler(
    const cl_context &context,
    cl_bool normalized_coords,
    cl_addressing_mode addressing_mode,
    cl_filter_mode filter_mode);

/**
 * @brief Decrement the sampler reference count.
 */
cl_int ReleaseSampler(const cl_sampler &sampler);

} /* cl */
} /* ito */

#endif /* ITO_OPENCL_SAMPLER_H_ */
