/*
 * iobuffer.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ITO_OPENGL_IOBUFFER_H_
#define TEST_ITO_OPENGL_IOBUFFER_H_

#include "ito/opengl.hpp"

/**
 * @brief IOBuffer holds a framebuffer object with a specified size, a single
 * color attachment and and optionally a depth attachment.
 */
struct IOBuffer {
    GLint width;
    GLint height;
    GLuint color_texture;
    GLuint depth_texture;
    GLuint fbo;

    /** Bind the IOBuffer for reading / writing. */
    void bind(void);
    void unbind(void);

    /* IOBuffer factory function. */
    static IOBuffer Create(
        const GLint width,
        const GLint height,
        const GLenum internal_format);
    static void Destroy(const IOBuffer &iobuffer);
};

#endif /* TEST_ITO_OPENGL_IOBUFFER_H_ */
