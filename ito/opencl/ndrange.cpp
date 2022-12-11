/*
 * ndrange.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "ndrange.hpp"

namespace ito {
namespace cl {

/**
 * @brief Round up global work size to the next multiple of local work size.
 */
size_t NDRange::Roundup(size_t global_work_size, size_t local_work_size)
{
    /*
     * Compute the remainder r of global_work_size mod(local_work_size).
     * If r is zero, global_work_size is a multiple of local_work_size.
     * Otherwise,
     *  (global_work_size - r) is the largest multiple of local_work_size
     *  smaller than global_work_size, and
     *  (global_work_size - r) + local_work_size is the smallest multiple
     *  of local_work_size larger than global_work_size.
     */
    size_t r = global_work_size % local_work_size;
    return (r == 0) ? global_work_size : global_work_size + local_work_size - r;
}

/**
 * @brief Constant zero-dimensional range.
 */
const NDRange NDRange::Null = {};

/**
 * @brief Factory functions.
 */
NDRange NDRange::Make()
{
    return {0, {0, 0, 0}};
}

NDRange NDRange::Make(size_t s0)
{
    return {1, {s0, 0, 0}};
}

NDRange NDRange::Make(size_t s0, size_t s1)
{
    return {2, {s0, s1, 0}};
}

NDRange NDRange::Make(size_t s0, size_t s1, size_t s2)
{
    return {3, {s0, s1, s2}};
}

} /* cl */
} /* ito */

