/*
 * image.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_IMAGE_H_
#define ITO_OPENCL_IMAGE_H_

#include <vector>
#include "base.hpp"

namespace ito {
namespace cl {

/**
 * @brief Convert image to normalized OpenCL floating point data type.
 */
std::vector<cl_float> ImageAsFloat(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp);

/**
 * @brief Convert image to normalized OpenCL 2d-vector data type.
 */
std::vector<cl_float2> ImageAsFloat2(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp);

/**
 * @brief Convert image to normalized OpenCL 3d-vector data type.
 */
std::vector<cl_float3> ImageAsFloat3(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp);

/**
 * @brief Convert image to normalized OpenCL 4d-vector data type.
 */
std::vector<cl_float4> ImageAsFloat4(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp);

} /* cl */
} /* ito */

#endif /* ITO_OPENCL_IMAGE_H_ */
