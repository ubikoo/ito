/*
 * texture.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_TEXTURE_H_
#define ITO_OPENGL_TEXTURE_H_

#include "base.hpp"

namespace ito {
namespace gl {

/**
 * @brief Create a 1d texture with specified size and internal format.
 */
GLuint CreateTexture1d(
    const GLint internalformat,
    const GLsizei width,
    const GLenum pixelformat,
    const GLenum pixeltype,
    const GLvoid *pixels);

/**
 * @brief Create a 2d texture with specified size and internal format.
 */
GLuint CreateTexture2d(
    const GLint internalformat,
    const GLsizei width,
    const GLsizei height,
    const GLenum pixelformat,
    const GLenum pixeltype,
    const GLvoid *pixels);

/**
 * @brief Create a 3d texture with specified size and internal format.
 */
GLuint CreateTexture3d(
    const GLint internalformat,
    const GLsizei width,
    const GLsizei height,
    const GLsizei depth,
    const GLenum pixelformat,
    const GLenum pixeltype,
    const GLvoid *pixels);

/**
 * @brief Create a 1-dimensional texture and attach the storage for the buffer
 * object to the newly active buffer texture.
 */
GLuint CreateTextureBuffer(GLint internalformat, GLuint buffer);

/**
 * @brief Delete a texture object.
 */
void DestroyTexture(const GLuint &texture);

/**
 * @brief Generate texture mipmaps.
 */
void SetTextureMipmap(
    const GLenum target,
    const GLint base_level = 0,
    const GLint max_level = 1000,
    const GLboolean generate = true);

/**
 * @brief Set texture minifying and magnifying functions.
 */
void SetTextureFilter(
    const GLenum target,
    const GLint filter_min = GL_NEAREST,
    const GLint filter_mag = GL_NEAREST);

/**
 * @brief Set wrap parameter for the texture coordinates.
 */
void SetTextureWrap(
    const GLenum target,
    const GLint wrap_s = GL_REPEAT,
    const GLint wrap_t = GL_REPEAT,
    const GLint wrap_r = GL_REPEAT);

/**
 * @brief Return the width for 0th level of detail of a texture currently bound
 * the specified target.
 */
GLsizei GetTextureWidth(const GLenum target);

/**
 * @brief Return the height for 0th level of detail of a texture currently bound
 * the specified target.
 */
GLsizei GetTextureHeight(const GLenum target);

/**
 * @brief Return the depth for 0th level of detail of a texture currently bound
 * the specified target.
 */
GLsizei GetTextureDepth(const GLenum target);

/**
 * @brief Return the internal format for 0th level of detail of a texture
 * currently bound the specified target.
 */
GLint GetTextureInternalformat(const GLenum target);

/**
 * @brief Is the texture internal format a valid GL texture format?
 */
bool IsValidTextureInternalformat(const GLint internalformat);

/**
 * @brief Is the texture internal format a valid GL texture format?
 */
bool IsValidTextureBufferInternalformat(const GLint internalformat);

/**
 * @brief Bind the texture to the target at the specified texture unit.
 */
void ActiveBindTexture(GLenum target, GLenum texunit, GLuint texture);

/**
 * @brief Bind the texture to the target of the specified texture unit and
 * attach the buffer storage to the texture.
 */
void ActiveBindTextureBuffer(
    GLenum target,
    GLenum texunit,
    GLuint texture,
    GLenum internalformat,
    GLuint buffer);

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_TEXTURE_H_ */
