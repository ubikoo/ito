/*
 * framebuffer.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "framebuffer.hpp"
#include "imageformat.hpp"
#include "renderbuffer.hpp"
#include "texture.hpp"

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
    const GLint filter_mag)
{
    ito_assert(width > 0, "invalid framebuffer width");
    ito_assert(height > 0, "invalid framebuffer height");
    ito_assert(n_color_attachments > 0, "invalid color attachments");
    ito_assert(IsValidFramebufferColorInternalformat(color_internalformat),
        "invalid color attachment internal format");
    ito_assert(color_textures != nullptr, "null color textures");
    ito_assert(IsValidFramebufferDepthInternalformat(depth_internalformat),
        "invalid depth attachment internal format");

    /*
     * Generate a new framebuffer object and bind it to the target point.
     */
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    ito_assert(glIsFramebuffer(framebuffer),
        "failed to generate framebuffer object");

    /*
     * Attach n-color-renderable textures to the framebuffer.
     */
    for (GLsizei i = 0; i < n_color_attachments; ++i) {
        GLuint texture = CreateTexture2d(
            color_internalformat,
            width,
            height,
            ImageFormat::BaseFormat(color_internalformat),
            ImageFormat::DataType(color_internalformat),
            nullptr);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i,
            GL_TEXTURE_2D,
            texture,
            0);
        glBindTexture(GL_TEXTURE_2D, 0);
        color_textures[i] = texture;
    }

    /*
     * Attach a depth-renderable texture to the framebuffer.
     */
    if (depth_texture != nullptr) {
        *depth_texture = CreateTexture2d(
            depth_internalformat,
            width,
            height,
            ImageFormat::BaseFormat(depth_internalformat),
            ImageFormat::DataType(depth_internalformat),
            nullptr);
        glBindTexture(GL_TEXTURE_2D, *depth_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            *depth_texture,
            0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    /*
     * Specify which attached color buffers will be used for rendering.
     */
    std::vector<GLenum> color_attachments;
    for (GLsizei i = 0; i < n_color_attachments; ++i) {
        color_attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers(color_attachments.size(), color_attachments.data());

    /*
     * Assert framebuffer completeness status after adding the attachments.
     */
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    ito_assert(status == GL_FRAMEBUFFER_COMPLETE,
        ito::str::format("incomplete framebuffer, status: 0x%x\n", status));

    /*
     * Bind the framebuffer target to the default framebuffer and return.
     */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return framebuffer;
}

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
    const GLint filter_mag)
{
    return CreateFramebuffer(
        width,
        height,
        n_color_attachments,
        color_internalformat,
        color_textures,
        GL_DEPTH_COMPONENT,
        nullptr,
        filter_min,
        filter_mag);
}

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
    const GLint filter_mag)
{
    ito_assert(width > 0, "invalid framebuffer width");
    ito_assert(height > 0, "invalid framebuffer height");
    ito_assert(IsValidFramebufferDepthInternalformat(depth_internalformat),
        "invalid depth attachment internal format");
    ito_assert(depth_texture != nullptr, "null depth texture");

    /*
     * Generate a new framebuffer object and bind it to the target point.
     */
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    ito_assert(glIsFramebuffer(framebuffer),
        "failed to generate framebuffer object");

    /*
     * Attach a depth-renderable texture to the framebuffer.
     */
    *depth_texture = CreateTexture2d(
        depth_internalformat,
        width,
        height,
        ImageFormat::BaseFormat(depth_internalformat),
        ImageFormat::DataType(depth_internalformat),
        nullptr);
    glBindTexture(GL_TEXTURE_2D, *depth_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        *depth_texture,
        0);
    glBindTexture(GL_TEXTURE_2D, 0);

    /*
     * Framebuffer object is not complete without an attached color buffer.
     * Tell OpenGL to not render any color data by setting glDrawBuffer and
     * glReadbuffer to GL_NONE.
     */
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    /*
     * Assert framebuffer completeness status after adding the attachments.
     */
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    ito_assert(status == GL_FRAMEBUFFER_COMPLETE,
        ito::str::format("incomplete framebuffer, status: 0x%x\n", status));

    /*
     * Bind the framebuffer target to the default framebuffer and return.
     */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return framebuffer;
}

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
    GLuint *depth_renderbuffer)
{
    ito_assert(width > 0, "invalid framebuffer width");
    ito_assert(height > 0, "invalid framebuffer height");
    ito_assert(n_color_attachments > 0, "invalid color attachments");
    ito_assert(IsValidFramebufferColorInternalformat(color_internalformat),
        "invalid color attachment internal format");
    ito_assert(color_renderbuffers != nullptr, "null color renderbuffers");
    ito_assert(IsValidFramebufferDepthInternalformat(depth_internalformat),
        "invalid depth attachment internal format");

    /*
     * Generate a new framebuffer object and bind it to the target point.
     */
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    ito_assert(glIsFramebuffer(framebuffer),
        "failed to generate framebuffer object");

    /*
     * Attach n color-renderable renderbuffers to the framebuffer.
     */
    for (GLsizei i = 0; i < n_color_attachments; ++i) {
        GLuint renderbuffer = CreateRenderbuffer(
            color_internalformat,
            width,
            height);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i,
            GL_RENDERBUFFER,
            renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        color_renderbuffers[i] = renderbuffer;
    }

    /*
     * Attach a depth-renderable renderbuffer to the framebuffer.
     */
    if (depth_renderbuffer != nullptr) {
        *depth_renderbuffer = CreateRenderbuffer(
            depth_internalformat,
            width,
            height);
        glBindRenderbuffer(GL_RENDERBUFFER, *depth_renderbuffer);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER,
            *depth_renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    /*
     * Specify which attached color buffers will be used for rendering.
     */
    std::vector<GLenum> color_attachments;
    for (GLsizei i = 0; i < n_color_attachments; ++i) {
        color_attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers(color_attachments.size(), color_attachments.data());

    /*
     * Ensure framebuffer is complete after adding the attachments.
     */
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    ito_assert(status == GL_FRAMEBUFFER_COMPLETE,
        ito::str::format("incomplete framebuffer, status: 0x%x\n", status));

    /*
     * Unbind from the framebuffer target before returning the handle.
     */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return framebuffer;
}

/**
 * @brief Delete a framebuffer object.
 */
void DestroyFramebuffer(const GLuint &framebuffer)
{
    glDeleteFramebuffers(1, &framebuffer);
}

/**
 * @brief Return the maximum number of color attachment points in a framebuffer
 * object (must be at least 4).
 */
GLint GetFramebufferMaxColorAttachments(void)
{
    GLint max_color_attachments;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_color_attachments);
    return max_color_attachments;
}

/**
 * GetFramebufferMaxDrawBuffers
 * @brief Return the maximum number of simultaneous outputs that may be written
 * in a fragment shader (must be at least 4).
 */
GLint GetFramebufferMaxDrawBuffers(void)
{
    GLint max_draw_buffers;
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &max_draw_buffers);
    return max_draw_buffers;
}

/**
 * IsValidFramebufferColorInternalformat
 * @brief Is format a valid color-renderable internal format?
 */
bool IsValidFramebufferColorInternalformat(const GLenum internalformat)
{
    if (!ImageFormat::Contains(internalformat)) {
        return false;
    }

    return (
        /* Texel normalized base internal formats */
        internalformat == GL_RED               ||
        internalformat == GL_RG                ||
        internalformat == GL_RGB               ||
        internalformat == GL_RGBA              ||
        internalformat == GL_DEPTH_COMPONENT   ||
        internalformat == GL_DEPTH_STENCIL     ||
        /* Texel 8- and 16-bit normalized sized internal formats */
        internalformat == GL_R8                ||
        internalformat == GL_RG8               ||
        internalformat == GL_RGB8              ||
        internalformat == GL_RGBA8             ||
        internalformat == GL_R16               ||
        internalformat == GL_RG16              ||
        internalformat == GL_RGB16             ||
        internalformat == GL_RGBA16            ||
        /* Texel 16- and 32-bit floating-point sized internal formats */
        internalformat == GL_R16F              ||
        internalformat == GL_RG16F             ||
        internalformat == GL_RGB16F            ||
        internalformat == GL_RGBA16F           ||
        internalformat == GL_R32F              ||
        internalformat == GL_RG32F             ||
        internalformat == GL_RGB32F            ||
        internalformat == GL_RGBA32F           ||
        /* Texel 8-, 16-, and 32-bit integer sized internal formats */
        internalformat == GL_R8I               ||
        internalformat == GL_RG8I              ||
        internalformat == GL_RGB8I             ||
        internalformat == GL_RGBA8I            ||
        internalformat == GL_R16I              ||
        internalformat == GL_RG16I             ||
        internalformat == GL_RGB16I            ||
        internalformat == GL_RGBA16I           ||
        internalformat == GL_R8UI              ||
        internalformat == GL_RG8UI             ||
        internalformat == GL_RGB8UI            ||
        internalformat == GL_RGBA8UI           ||
        internalformat == GL_R16UI             ||
        internalformat == GL_RG16UI            ||
        internalformat == GL_RGB16UI           ||
        internalformat == GL_RGBA16UI);
}

/**
 * IsValidFramebufferDepthInternalformat
 * @brief Is format a valid depth-renderable internal format?
 */
bool IsValidFramebufferDepthInternalformat(const GLenum internalformat)
{
    if (!ImageFormat::Contains(internalformat)) {
        return false;
    }

    return (
        /* Texel depth sized internal formats */
        internalformat == GL_DEPTH_COMPONENT    ||
        internalformat == GL_DEPTH_COMPONENT16  ||
        internalformat == GL_DEPTH_COMPONENT24  ||
        internalformat == GL_DEPTH_COMPONENT32  ||
        internalformat == GL_DEPTH_COMPONENT32F ||
        internalformat == GL_DEPTH24_STENCIL8);
}

} /* gl */
} /* ito */
