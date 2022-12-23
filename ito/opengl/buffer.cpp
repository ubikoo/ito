/*
 * buffer.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "buffer.hpp"

namespace ito {
namespace gl {

/**
 * @brief Create a buffer object bound to a target, with a data store size in
 * bytes, and with a specified buffer usage. Buffer usage can be classified
 * down to two parts, frequency and nature of access:
 *  GL_STREAM_DRAW,  GL_STREAM_READ,  GL_STREAM_COPY
 *  GL_STATIC_DRAW,  GL_STATIC_READ,  GL_STATIC_COPY
 *  GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY
 *
 * @note A buffer object contains an array of unformatted data allocated by
 * OpenGL and nothing more. The meaning of the data is defined when the buffer
 * is bound to a target using glBindBuffer.
 * A buffer is only a vertex buffer object holding vertex array attributes when
 * bound to a GL_ARRAY_BUFFER target.
 * The exact same buffer is interpreted as an element buffer object with vertex
 * indices if bound to a GL_ELEMENT_ARRAY_BUFFER target.
 * The exact same buffer is interpreted as a pixel buffer object if bound to
 * GL_PIXEL_PACK_BUFFER.
 *
 * @see OpenGL buffer objects
 *      https://www.khronos.org/opengl/wiki/Buffer_Object
 *      http://www.songho.ca/opengl
 *      https://stackoverflow.com/questions/748162
 *
 * @see OpenGL direct state access
 *      https://www.khronos.org/opengl/wiki/Direct_State_Access
 *      https://stackoverflow.com/questions/33363490
 *
 * @see OpenGL persistent buffer mapping and immutable storage
 *      https://learnopengl.com/Advanced-OpenGL/Advanced-Data
 *      https://gamedev.stackexchange.com/questions/87074
 */
GLuint CreateBuffer(
    const GLenum target,
    const GLsizeiptr size,
    const GLenum usage)
{
    ito_assert(size > 0, "invalid buffer data store size");

    /*
     * Generate a new buffer object name and bind it to the target point.
     * No buffer object is associated with the name set by glGenBuffers
     * until it is bound to the target by a call to glBindBuffer.
     */
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    ito_assert(glIsBuffer(buffer), "failed to generate buffer object");

    /*
     * Create a new data store for a buffer object. Use the buffer object
     * currently bound to the target (glBufferData) or use a buffer object
     * associated with the name (glNamedBufferData).
     */
    glBufferData(target, size, NULL, usage);

    /*
     * Unbind the buffer from the target point and return.
     */
    glBindBuffer(target, 0);
    return buffer;
}

/**
 * @brief Delete a buffer object.
 */
void DestroyBuffer(const GLuint &buffer)
{
    glDeleteBuffers(1, &buffer);
}

/**
 * @brief Return the access policy set while mapping the buffer object.
 */
GLenum GetBufferAccess(const GLenum target)
{
    GLenum access;
    glGetBufferParameteriv(target, GL_BUFFER_ACCESS, (GLint *) &access);
    return access;
}

/**
 * @brief Return the parameters of the buffer currently bound to the target.
 */
GLsizeiptr GetBufferSize(const GLenum target)
{
    GLsizeiptr size;
    glGetBufferParameteriv(target, GL_BUFFER_SIZE, (GLint *) &size);
    return size;
}

/**
 * @brief Return the usage pattern of the buffer currently bound to the target.
 */
GLenum GetBufferUsage(const GLenum target)
{
    GLenum usage;
    glGetBufferParameteriv(target, GL_BUFFER_USAGE, (GLint *) &usage);
    return usage;
}

/**
 * @brief Return the flag indicating whether the buffer is currently mapped.
 */
GLboolean IsBufferMapped(const GLenum target)
{
    GLboolean mapped;
    glGetBufferParameteriv(target, GL_BUFFER_MAPPED, (GLint *) &mapped);
    return mapped;
}

} /* gl */
} /* ito */
