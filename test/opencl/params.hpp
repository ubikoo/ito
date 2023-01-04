/*
 * params.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_OPENCL_PARAMS_H_
#define TEST_OPENCL_PARAMS_H_

#include "ito/opencl.hpp"

namespace Params {

static const cl_ulong kDeviceIndex = 2;
static const cl_ulong kWorkGroupSize1d = 256;
static const cl_ulong kWorkGroupSize2d = 16;

} /* Params */

#endif /* TEST_OPENCL_PARAMS_H_ */
