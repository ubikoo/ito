/*
 * renderbuffer.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_RENDERBUFFER_H_
#define ITO_OPENGL_RENDERBUFFER_H_

#include "base.hpp"

namespace ito {
namespace gl {

/**
 * @brief Create a renderbuffer object with width and height using a data store
 * with the specified internal format.
 *
 * @see https://www.khronos.org/opengl/wiki/Renderbuffer_Object
 */
GLuint CreateRenderbuffer(
    const GLenum internalformat,
  	const GLsizei width,
  	const GLsizei height);

/**
 * @brief Delete a renderbuffer object.
 */
void DestroyRenderbuffer(const GLuint &renderbuffer);

/**
 * @brief Return the width of a renderbuffer currently bound to the target.
 */
GLsizei GetRenderbufferWidth(const GLenum target);

/**
 * @brief Return the height of a renderbuffer currently bound to the target.
 */
GLsizei GetRenderbufferHeight(const GLenum target);

/**
 * @brief Return the internal format of a renderbuffer currently bound to the
 * specified target.
 */
GLint GetRenderbufferInternalformat(const GLenum target);

/**
 * @brief Is the renderbuffer internal format a valid GL renderbuffer format?
 */
bool IsValidRenderbufferInternalformat(const GLint internalformat);

} /* gl */
} /* ito */


#endif /* ITO_OPENGL_RENDERBUFFER_H_ */
