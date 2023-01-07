/*
 * platform.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_PLATFORM_H_
#define ITO_OPENCL_PLATFORM_H_

#include <string>
#include <vector>
#include "base.hpp"

namespace ito {
namespace cl {

/**
 * @brief Return a vector of all platforms currently available.
 */
std::vector<cl_platform_id> GetPlatformIDs(void);

/**
 * @brief Return a string with basic platform information.
 */
std::string GetPlatformInfoString(const cl_platform_id &platform);

} /* cl */
} /* ito */

#endif /* ITO_OPENCL_PLATFORM_H_ */
