/*
 * uniform.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "uniform.hpp"
#include "variable.hpp"

namespace ito {
namespace gl {

/**
 * @brief Update the uniform in the current shader program object using the
 * corresponding glUniform* function.
 */
bool SetUniform(const GLint location, const GLenum type, const void *data)
{
    if (location == -1) {
        std::cerr << ito::str::format("invalid uniform location\n");
        return false;
    }

    if (data == nullptr) {
        std::cerr << ito::str::format("invalid uniform data\n");
        return false;
    }

    /* Update the uniform in the shader program. */
    switch (type) {
    /* Vector double, dvec2, dvec3, dvec4 */
    case GL_DOUBLE:
        glUniform1dv(location, 1, static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_VEC2:
        glUniform2dv(location, 1, static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_VEC3:
        glUniform3dv(location, 1, static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_VEC4:
        glUniform4dv(location, 1, static_cast<const GLdouble *>(data));
        break;

    /* Vector float, vec2, vec3, vec4 */
    case GL_FLOAT:
        glUniform1fv(location, 1, static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_VEC2:
        glUniform2fv(location, 1, static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_VEC3:
        glUniform3fv(location, 1, static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_VEC4:
        glUniform4fv(location, 1, static_cast<const GLfloat *>(data));
        break;

    /* Vector int, ivec2, ivec3, ivec4 */
    case GL_INT:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_VEC2:
        glUniform2iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_VEC3:
        glUniform3iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_VEC4:
        glUniform4iv(location, 1, static_cast<const GLint *>(data));
        break;

    /* Vector unsigned int, uvec2, uvec3, uvec4 */
    case GL_UNSIGNED_INT:
        glUniform1uiv(location, 1, static_cast<const GLuint *>(data));
        break;
    case GL_UNSIGNED_INT_VEC2:
        glUniform2uiv(location, 1, static_cast<const GLuint *>(data));
        break;
    case GL_UNSIGNED_INT_VEC3:
        glUniform3uiv(location, 1, static_cast<const GLuint *>(data));
        break;
    case GL_UNSIGNED_INT_VEC4:
        glUniform4uiv(location, 1, static_cast<const GLuint *>(data));
        break;

    /*
     * Sampler types [g]sampler1D, [g]sampler2D, [g]sampler3D, [g]samplerBuffer,
     * [g]sampler2DRect, where [g] is none for float, i for int and u for uint.
     */
    case GL_SAMPLER_1D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_SAMPLER_2D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_SAMPLER_3D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_SAMPLER_BUFFER:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_SAMPLER_2D_RECT:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_SAMPLER_1D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_SAMPLER_2D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_SAMPLER_3D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_SAMPLER_BUFFER:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_INT_SAMPLER_2D_RECT:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_UNSIGNED_INT_SAMPLER_1D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_UNSIGNED_INT_SAMPLER_2D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_UNSIGNED_INT_SAMPLER_3D:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_UNSIGNED_INT_SAMPLER_BUFFER:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;
    case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
        glUniform1iv(location, 1, static_cast<const GLint *>(data));
        break;

    /* Invalid uniform type. */
    default:
        std::cerr << ito::str::format("invalid uniform type: %d\n", type);
        return false;
    }
    return true;
}

/**
 * @brief Update the uniform with the specified name in the current shader
 * program object using the corresponding glUniform* function.
 */
bool SetUniform(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const void *data)
{
    GLint location = glGetUniformLocation(program, name.data());
    if (!SetUniform(location, type, data)) {
        std::cerr << ito::str::format("invalid uniform: %s\n", name.c_str());
        return false;
    }
    return true;
}

/**
 * @brief Update the uniform matrix in the current shader program object
 * using the corresponding glUniform* function.
 */
bool SetUniformMatrix(
    const GLint location,
    const GLenum type,
    const GLboolean transpose,
    const void *data)
{
    if (location == -1) {
        std::cerr << ito::str::format("invalid uniform location\n");
        return false;
    }

    if (data == nullptr) {
        std::cerr << ito::str::format("invalid uniform data\n");
        return false;
    }

    /* Update the uniform in the shader program. */
    switch (type) {
    /* Matrix dmat2, dmat2x3, dmat2x4 */
    case GL_DOUBLE_MAT2:
        glUniformMatrix2dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_MAT2x3:
        glUniformMatrix2x3dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_MAT2x4:
        glUniformMatrix2x4dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;

    /* Matrix dmat3x2, dmat3, dmat3x4 */
    case GL_DOUBLE_MAT3x2:
        glUniformMatrix3x2dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_MAT3:
        glUniformMatrix3dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_MAT3x4:
        glUniformMatrix3x4dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;

    /* Matrix dmat4x2, dmat4x3, dmat4 */
    case GL_DOUBLE_MAT4x2:
        glUniformMatrix4x2dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_MAT4x3:
        glUniformMatrix4x3dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;
    case GL_DOUBLE_MAT4:
        glUniformMatrix4dv(location, 1, transpose,
            static_cast<const GLdouble *>(data));
        break;

    /* Matrix mat2, mat2x3, mat2x4 */
    case GL_FLOAT_MAT2:
        glUniformMatrix2fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_MAT2x3:
        glUniformMatrix2x3fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_MAT2x4:
        glUniformMatrix2x4fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;

    /* Matrix mat3x2, mat3, mat3x4 */
    case GL_FLOAT_MAT3x2:
        glUniformMatrix3x2fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_MAT3:
        glUniformMatrix3fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_MAT3x4:
        glUniformMatrix3x4fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;

    /* Matrix mat4x2, mat4x3, mat4 */
    case GL_FLOAT_MAT4x2:
        glUniformMatrix4x2fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_MAT4x3:
        glUniformMatrix4x3fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;
    case GL_FLOAT_MAT4:
        glUniformMatrix4fv(location, 1, transpose,
            static_cast<const GLfloat *>(data));
        break;

    /* Invalid uniform type. */
    default:
        std::cerr << ito::str::format("invalid uniform type: %d\n", type);
        return false;
    }
    return true;
}

/**
 * SetUniformMatrix
 * @brief Update the uniform matrix in the current shader program object
 * using the corresponding glUniform* function.
 */
bool SetUniformMatrix(
    const GLuint &program,
    const std::string &name,
    const GLenum type,
    const GLboolean transpose,
    const void *data)
{
    GLint location = glGetUniformLocation(program, name.data());
    if (!SetUniformMatrix(location, type, transpose, data)) {
        std::cerr << ito::str::format("invalid uniform: %s\n", name.c_str());
        return false;
    }
    return true;
}

} /* gl */
} /* ito */
