/*
 * buffer.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_BUFFER_H_
#define ITO_OPENGL_BUFFER_H_

#include "base.hpp"

namespace ito {
namespace gl {

/**
 * @brief Create a buffer object.
 */
GLuint CreateBuffer(
    const GLenum target,
    const GLsizeiptr size,
    const GLenum usage);

/**
 * @brief Delete a buffer object.
 */
void DestroyBuffer(const GLuint &buffer);

/**
 * @brief Return the access policy set while mapping the buffer object.
 */
GLenum GetBufferAccess(const GLenum target);

/**
 * @brief Return the parameters of the buffer currently bound to the target.
 */
GLsizeiptr GetBufferSize(const GLenum target);

/**
 * @brief Return the usage pattern of the buffer currently bound to the target.
 */
GLenum GetBufferUsage(const GLenum target);

/**
 * @brief Return the flag indicating whether the buffer is currently mapped.
 */
GLboolean IsBufferMapped(const GLenum target);

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_BUFFER_H_ */
