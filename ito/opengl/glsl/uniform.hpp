/*
 * uniform.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_GLSL_UNIFORM_H_
#define ITO_OPENGL_GLSL_UNIFORM_H_

#include <string>
#include "../base.hpp"

namespace ito {
namespace gl {

/**
 * @brief Update the uniform in the current shader program object using the
 * corresponding glUniform* function.
 */
bool SetUniform(const GLint location, const GLenum type, const void *data);

/**
 * @brief Update the uniform with the specified name in the current shader
 * program object using the corresponding glUniform* function.
 */
bool SetUniform(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data);

/**
 * @brief Update the uniform matrix in the current shader program object
 * using the corresponding glUniform* function.
 */
bool SetUniformMatrix(
    const GLint location,
    const GLenum type,
    const GLboolean transpose,
    const void *data);

/**
 * SetUniformMatrix
 * @brief Update the uniform matrix in the current shader program object
 * using the corresponding glUniform* function.
 */
bool SetUniformMatrix(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const GLboolean transpose,
    const void *data);

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_GLSL_UNIFORM_H_ */
