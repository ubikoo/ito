/*
 * base.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_BASE_H_
#define ITO_OPENGL_BASE_H_

#include "ito/core.hpp"
#include "ito/math.hpp"

/**
 * @brief Define GLFW_INCLUDE_NONE before the GLFW header to explicitly disable
 * inclusion of OpenGL environment header and use the header provided by glad.
 * @see https://www.glfw.org/docs/3.3
 */
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#endif /* ITO_OPENGL_BASE_H_ */
