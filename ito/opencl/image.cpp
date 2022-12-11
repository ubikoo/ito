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

#include "image.hpp"

namespace ito {
namespace cl {

/**
 * @brief Convert image to normalized OpenCL floating point data type.
 */
std::vector<cl_float> ImageAsFloat(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp)
{
    const uint32_t n_channels = bpp / 8;
    const uint8_t *px = bitmap;

    std::vector<cl_float> data;
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            for (uint32_t c = 0; c < n_channels; ++c) {
                cl_float v = bitmap[c] / 255.0f;
                data.emplace_back(v);
            }
            px += n_channels;
        }
    }
    return data;
}

/**
 * @brief Convert image to normalized OpenCL 2d-vector data type.
 */
std::vector<cl_float2> ImageAsFloat2(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp)
{
    const uint32_t n_channels = bpp / 8;
    const uint8_t *px = bitmap;

    std::vector<cl_float2> data;
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            cl_float2 v = {
                n_channels > 0 ? px[0] / 255.0f : 0.0f,
                n_channels > 1 ? px[1] / 255.0f : 0.0f};
            data.emplace_back(v);
            px += n_channels;
        }
    }
    return data;
}

/**
 * @brief Convert image to normalized OpenCL 3d-vector data type.
 */
std::vector<cl_float3> ImageAsFloat3(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp)
{
    const uint32_t n_channels = bpp / 8;
    const uint8_t *px = bitmap;

    std::vector<cl_float3> data;
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            cl_float3 v = {
                n_channels > 0 ? px[0] / 255.0f : 0.0f,
                n_channels > 1 ? px[1] / 255.0f : 0.0f,
                n_channels > 2 ? px[2] / 255.0f : 0.0f};
            data.emplace_back(v);
            px += n_channels;
        }
    }
    return data;
}

/**
 * @brief Convert image to normalized OpenCL 4d-vector data type.
 */
std::vector<cl_float4> ImageAsFloat4(
    const uint8_t *bitmap,
    const uint32_t width,
    const uint32_t height,
    const uint32_t bpp)
{
    const uint32_t n_channels = bpp / 8;
    const uint8_t *px = bitmap;

    std::vector<cl_float4> data;
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            cl_float4 v = {
                n_channels > 0 ? px[0] / 255.0f : 0.0f,
                n_channels > 1 ? px[1] / 255.0f : 0.0f,
                n_channels > 2 ? px[2] / 255.0f : 0.0f,
                n_channels > 3 ? px[3] / 255.0f : 0.0f};
            data.emplace_back(v);
            px += n_channels;
        }
    }
    return data;
}

} /* cl */
} /* ito */
