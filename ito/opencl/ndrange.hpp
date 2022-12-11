/*
 * ndrange.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_NDRANGE_H_
#define ITO_OPENCL_NDRANGE_H_

#include <array>
#include "base.hpp"

namespace ito {
namespace cl {

/**
 * NDRange
 * @brief Class interface for specifying kernel NDRange values.
 */
struct NDRange {
    /** Dimension and range */
    cl_uint dim;
    std::array<size_t,3> range;

    /** Roundup global work size to the next multiple of local work size. */
    static size_t Roundup(size_t global_work_size, size_t local_work_size);

    /** Constant zero-dimensional range. */
    static const NDRange Null;

    /** Factory functions.*/
    static NDRange Make();
    static NDRange Make(size_t s0);
    static NDRange Make(size_t s0, size_t s1);
    static NDRange Make(size_t s0, size_t s1, size_t s2);
};

} /* cl */
} /* ito */

#endif /* ITO_OPENCL_NDRANGE_H_ */
