/*
 * framebuffer.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_FRAMEBUFFER_H_
#define ITO_OPENGL_FRAMEBUFFER_H_

#include "base.hpp"

namespace ito {
namespace gl {

/**
 * @brief Create a framebuffer object with width and height, using textures
 * as rendering surfaces for the color and depth attachments.
 *
 * @note There must be at least one color render target attached to the
 * framebuffer. The depth buffer attachment is optional.
 *
 * @see https://docs.gl/gl4/glFramebufferRenderbuffer
 *      https://docs.gl/gl4/glFramebufferTexture
 *      https://www.khronos.org/opengl/wiki/Renderbuffer_Object
 *      https://www.khronos.org/opengl/wiki/Texture
 *      https://paroj.github.io/gltut/Texturing/Tutorial%2014.html
 *      http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture
 *      http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping
 */
GLuint CreateFramebuffer(
    const GLsizei width,
    const GLsizei height,
    const GLsizei n_color_attachments,
    const GLenum color_internalformat,
    GLuint *color_textures,
    const GLenum depth_internalformat,
    GLuint *depth_texture,
    const GLint filter_min,
    const GLint filter_mag);

/**
 * @brief Create a framebuffer object with width and height, using textures
 * as rendering surfaces and no depth attachment.
 */
GLuint CreateFramebufferTexture(
    const GLsizei width,
    const GLsizei height,
    const GLsizei n_color_attachments,
    const GLenum color_internalformat,
    GLuint *color_textures,
    const GLint filter_min,
    const GLint filter_mag);

/**
 * @brief Create a framebuffer object with width and height, using only the
 * depth attachment. The depth texture stores the depth buffer which can be used
 * for further use - depth maps, etc.
 */
GLuint CreateFramebufferDepth(
    const GLsizei width,
    const GLsizei height,
    const GLenum depth_internalformat,
    GLuint *depth_texture,
    const GLint filter_min,
    const GLint filter_mag);

/**
 * @brief Create a framebuffer object with width and height, using
 * renderbuffers as rendering surfaces for the color and depth attachments.
 */
GLuint CreateFramebufferRenderbuffer(
    const GLsizei width,
    const GLsizei height,
    const GLsizei n_color_attachments,
    const GLenum color_internalformat,
    GLuint *color_renderbuffers,
    const GLenum depth_internalformat,
    GLuint *depth_renderbuffer);

/**
 * @brief Delete a framebuffer object.
 */
void DestroyFramebuffer(const GLuint &framebuffer);

/**
 * @brief Return the maximum number of color attachment points in a framebuffer
 * object (must be at least 4).
 */
GLint GetFramebufferMaxColorAttachments(void);

/**
 * GetFramebufferMaxDrawBuffers
 * @brief Return the maximum number of simultaneous outputs that may be written
 * in a fragment shader (must be at least 4).
 */
GLint GetFramebufferMaxDrawBuffers(void);

/**
 * IsValidFramebufferColorInternalformat
 * @brief Is format a valid color-renderable internal format?
 */
bool IsValidFramebufferColorInternalformat(const GLenum internalformat);

/**
 * IsValidFramebufferDepthInternalformat
 * @brief Is format a valid depth-renderable internal format?
 */
bool IsValidFramebufferDepthInternalformat(const GLenum internalformat);

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_FRAMEBUFFER_H_ */
