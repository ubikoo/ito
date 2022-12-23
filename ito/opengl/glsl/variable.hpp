/*
 * variable.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_GLSL_VARIABLE_H_
#define ITO_OPENGL_GLSL_VARIABLE_H_

#include <string>
#include <vector>
#include "../base.hpp"

namespace ito {
namespace gl {

/**
 * @brief Helper structure specifying the properties of an active variable in
 * a shader program object. Active variables represent both active uniform
 * variables and active attribute variables in the shader.
 *
 * @note If an active variable is an array, it should be set manually using its
 * name in the shader program object.
 *
 * @see https://stackoverflow.com/questions/19894839
 */
struct Variable {
    std::string name;       /* variable name in the shader program */
    GLint location;         /* variable location in the shader program */
    GLsizei count;          /* variable count in units of OpenGL type */
    GLenum type;            /* variable enumerated OpenGL type */

    /**
     * @brief Does the map contain the key of the GL data type?
     */
    static bool Contains(const GLenum datatype);

    /**
     * @brief Return the name of the GL data type.
     */
    static std::string Name(const GLenum datatype);

    /**
     * @brief Return the length of the GL data type in units of the primitive
     * data type.
     */
    static GLuint Length(const GLenum datatype);

    /**
     * @brief Return the size of the underlying primitive data type in bytes.
     */
    static GLuint Size(const GLenum datatype);

    /**
     * @brief Return the enumerated type of the underlying primitive data type.
     */
    static GLenum Type(const GLenum datatype);
};

/**
 * @brief Return all active uniforms in a shader program object.
 */
std::vector<Variable> GetUniformVariables(const GLuint &program);

/**
 * @brief Return all active attributes in a shader program object.
 */
std::vector<Variable> GetAttributeVariables(const GLuint &program);

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_GLSL_VARIABLE_H_ */
