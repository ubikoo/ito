/*
 * iobuffer.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "ito/opengl.hpp"
using namespace ito;
#include "iobuffer.hpp"

/**
 * @brief Create a IOBuffer with specified width and internal format.
 */
IOBuffer IOBuffer::Create(
    const GLint width,
    const GLint height,
    const GLenum internal_format)
{
    IOBuffer iobuffer;

    iobuffer.width = width;
    iobuffer.height = height;
    iobuffer.fbo = gl::CreateFramebuffer(
        iobuffer.width,
        iobuffer.height,
        1,                              /* num color attachments */
        internal_format,                /* color buffer internal format */
        &iobuffer.color_texture,
        GL_DEPTH_COMPONENT32F,          /* depth buffer internal format */
        &iobuffer.depth_texture,
        GL_NEAREST,
        GL_NEAREST);

    return iobuffer;
}

/**
 * @brief Destroy the IOBuffer GL objects.
 */
void IOBuffer::Destroy(const IOBuffer &iobuffer)
{
    glDeleteTextures(1, &iobuffer.color_texture);
    glDeleteTextures(1, &iobuffer.depth_texture);
    glDeleteFramebuffers(1, &iobuffer.fbo);
}

/**
 * @brief Bind the IObuffer for writing.
 */
void IOBuffer::bind(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

/**
 * @brief Unbind the IObuffer for writing.
 */
void IOBuffer::unbind(void)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
