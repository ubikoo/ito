/*
 * attribute.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_GLSL_ATTRIBUTE_H_
#define ITO_OPENGL_GLSL_ATTRIBUTE_H_

#include <string>
#include "../base.hpp"

namespace ito {
namespace gl {

/** ---------------------------------------------------------------------------
 * @brief Enable a generic vertex attribute specified by its location index in
 * the shader program object.
 */
bool EnableAttribute(const GLint location);

/**
 * @brief Enable a generic vertex attribute with the specified name.
 */
bool EnableAttribute(const GLuint &program, const std::string &name);

/**
 * @brief Disable a generic vertex attribute specified by its location index in
 * the shader program object.
 */
bool DisableAttribute(const GLint location);

/**
 * @brief Enable a generic vertex attribute with the specified name.
 */
bool DisableAttribute(const GLuint &program, const std::string &name);

/** ---------------------------------------------------------------------------
 * @brief Given a vertex buffer object bound to GL_ARRAY_BUFFER, specify the
 * location on the buffer and the data format of the generic vertex attribute
 * at the location index in the vertex shader.
 */
bool AttributePointer(
    const GLint location,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset,
    const GLboolean normalized);

/**
 * @brief Specify the location on the buffer and the data format of the generic
 * vertex attribute with the specified name in the shader program object.
 */
bool AttributePointer(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset,
    const GLboolean normalized);

/**
 * @brief Set the buffer object currently bound to GL_ARRAY_BUFFER from which
 * to fetch the vertex data and specify the format of what a single generic
 * vertex attribute looks like.
 */
bool AttributeIPointer(
    const GLint location,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset);

/**
 * @brief Specify the data format of a generic vertex attribute with the given
 * name in the shader program object.
 */
bool AttributeIPointer(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset);

/**
 * @brief Set the vertex buffer object from which to fetch the vertex data and
 * specify the format of what a single unit of data looks like.
 */
bool AttributeLPointer(
    const GLint location,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset);

/**
 * @brief Specify the data format of a generic vertex attribute with the given
 * name in the shader program object.
 */
bool AttributeLPointer(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const GLsizei stride,
    const GLsizeiptr offset);

/** ---------------------------------------------------------------------------
 * @brief Specify all values of a generic vertex attribute.
 */
bool AttributeValue(const GLint location, const GLenum type, const void *data);

/**
 * @brief Set the value of a generic vertex attribute with the given name in
 * the shader program object.
 */
bool AttributeValue(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data);

/**
 * @brief Specify all values of a generic vertex attribute.
 */
bool AttributeIValue(const GLint location, const GLenum type, const void *data);

/**
 * @brief Set the value of a generic vertex attribute with the given name in
 * the shader program object.
 */
bool AttributeIValue(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data);

/**
 * @brief Specify the all values of a generic vertex attribute.
 */
bool AttributeLValue(const GLint location, const GLenum type, const void *data);

/**
 * @brief Set the value of a generic vertex attribute with the given name in
 * the shader program object.
 */
bool AttributeLValue(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data);

/** ---------------------------------------------------------------------------
 * @brief Modify the rate at which the generic vertex attribute at the specified
 * location advances during instanced rendering.
 */
bool AttributeDivisor(const GLint location, const GLuint divisor);

/**
 * @brief Modify the rate at which the generic vertex attribute with the given
 * name in the shader program object advances during instanced rendering.
 */
bool AttributeDivisor(
    const GLuint &program,
    const std::string &name,
    const GLuint divisor);

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_GLSL_ATTRIBUTE_H_ */
