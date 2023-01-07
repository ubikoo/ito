/*
 * program.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_GLSL_PROGRAM_H_
#define ITO_OPENGL_GLSL_PROGRAM_H_

#include <string>
#include <vector>
#include "../base.hpp"

namespace ito {
namespace gl {

/**
 * @brief Create a shader program object from a set of shader objects.
 */
GLuint CreateProgram(const std::vector<GLuint> &shaders);

/**
 * @brief Destroy a shader program object.
 */
void DestroyProgram(const GLuint &program);

/**
 * @brief Return an string containing the shader program object info.
 */
std::string GetProgramInfoString(const GLuint &program);

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_GLSL_PROGRAM_H_ */
