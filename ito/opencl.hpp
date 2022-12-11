/*
 * opencl.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_H_
#define ITO_OPENCL_H_

/**
 * @brief OpenCL interface
 * - Platform layer:
 *      Query platform
 *      Query devices
 *      Create command queue
 * - Runtime layer:
 *      Create buffers
 *      Compile program
 *
 *      Build kernel_a
 *      Set kernel_a arguments
 *      Build kernel_b
 *      Set kernel_b arguments
 *
 *      Enqueue write buffer data
 *      Enqueue kernel execution
 *      Enqueue read buffer data
 */
#include "opencl/base.hpp"
#include "opencl/platform.hpp"
#include "opencl/device.hpp"
#include "opencl/context.hpp"

#include "opencl/program.hpp"
#include "opencl/kernel.hpp"
#include "opencl/ndrange.hpp"
#include "opencl/event.hpp"
#include "opencl/queue.hpp"

#include "opencl/memory.hpp"
#include "opencl/sampler.hpp"
#include "opencl/interop.hpp"

#include "opencl/image.hpp"
#include "opencl/math.hpp"

#endif /* ITO_OPENCL_H_ */