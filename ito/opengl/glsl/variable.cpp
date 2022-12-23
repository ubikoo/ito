/*
 * variable.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "variable.hpp"

namespace ito {
namespace gl {

/** ---------------------------------------------------------------------------
 * @brief TypeMap is a collection of key-value pairs of GL data types keyed
 * by their enumerated GL type. GL data types are used to represent the types
 * of active Uniform and Attribute variables. DataType is a helper structure
 * containing the attributes of a GL data type:
 *      name:       name of the OpenGL enumerated data type
 *      length:     length in units of the primitive data type
 *      size:       size of the primitive data type in bytes
 *      type:       enumerated value of the primitive data type
 *
 * Basic data types are fundamental types and non-basic types are aggregates.
 * For simplicity, only a subset of GLSL fundamental data types are used here:
 *
 * Vector
 *      GL_DOUBLE, GL_DOUBLE_VEC[2,3,4]
 *      GL_FLOAT, GL_FLOAT_VEC[2,3,4]
 *      GL_INT, GL_INT_VEC[2,3,4]
 *      GL_UNSIGNED_INT, GL_UNSIGNED_INT_VEC[2,3,4]
 *
 * Matrix
 *      GL_DOUBLE, GL_DOUBLE_VEC[2,3,4]
 *      GL_FLOAT, GL_FLOAT_VEC[2,3,4]
 *
 * Sampler
 *      GL_SAMPLER_[1,2,3]D,
 *      GL_SAMPLER_BUFFER, GL_SAMPLER_2D_RECT
 *      GL_INT_SAMPLER_[1,2,3]D,
 *      GL_INT_SAMPLER_BUFFER, GL_INT_SAMPLER_2D_RECT
 *      GL_UNSIGNED_INT_SAMPLER_[1,2,3]D,
 *      GL_UNSIGNED_INT_SAMPLER_BUFFER, GL_UNSIGNED_INT_SAMPLER_2D_RECT
 *
 * @see https://www.khronos.org/opengl/wiki/Data_Type_(GLSL)
 *      https://www.khronos.org/opengl/wiki/Sampler_(GLSL)
 */
struct DataType {
    std::string name;       /* name of the OpenGL enumeration data type */
    GLuint length;          /* length in units of the primitive data type */
    GLuint size;            /* size of the primitive data type in bytes */
    GLenum type;            /* enumerated type of the primitive data type */
};

static const std::map<GLenum, DataType> TypeMap = {
    /* ---- Vector double, dvec2, dvec3, dvec4 ----------------------------- */
    {GL_DOUBLE,             {"GL_DOUBLE",       1, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_VEC2,        {"GL_DOUBLE_VEC2",  2, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_VEC3,        {"GL_DOUBLE_VEC3",  3, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_VEC4,        {"GL_DOUBLE_VEC4",  4, sizeof(GLdouble), GL_DOUBLE}},

    /* ---- Vector float, vec2, vec3, vec4 --------------------------------- */
    {GL_FLOAT,              {"GL_FLOAT",        1, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_VEC2,         {"GL_FLOAT_VEC2",   2, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_VEC3,         {"GL_FLOAT_VEC3",   3, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_VEC4,         {"GL_FLOAT_VEC4",   4, sizeof(GLfloat), GL_FLOAT}},

    /* ---- Vector int, ivec2, ivec3, ivec4 -------------------------------- */
    {GL_INT,                {"GL_INT",          1, sizeof(GLint), GL_INT}},
    {GL_INT_VEC2,           {"GL_INT_VEC2",     2, sizeof(GLint), GL_INT}},
    {GL_INT_VEC3,           {"GL_INT_VEC3",     3, sizeof(GLint), GL_INT}},
    {GL_INT_VEC4,           {"GL_INT_VEC4",     4, sizeof(GLint), GL_INT}},

    /* ---- Vector unsigned int, uvec2, uvec3, uvec4 ----------------------- */
    {GL_UNSIGNED_INT,       {"GL_UNSIGNED_INT",      1, sizeof(GLuint), GL_UNSIGNED_INT}},
    {GL_UNSIGNED_INT_VEC2,  {"GL_UNSIGNED_INT_VEC2", 2, sizeof(GLuint), GL_UNSIGNED_INT}},
    {GL_UNSIGNED_INT_VEC3,  {"GL_UNSIGNED_INT_VEC3", 3, sizeof(GLuint), GL_UNSIGNED_INT}},
    {GL_UNSIGNED_INT_VEC4,  {"GL_UNSIGNED_INT_VEC4", 4, sizeof(GLuint), GL_UNSIGNED_INT}},

    /* ---- Matrix dmat2, dmat2x3, dmat2x4 --------------------------------- */
    {GL_DOUBLE_MAT2,        {"GL_DOUBLE_MAT2",       4, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT2x3,      {"GL_DOUBLE_MAT2x3",     6, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT2x4,      {"GL_DOUBLE_MAT2x4",     8, sizeof(GLdouble), GL_DOUBLE}},

    /* ---- Matrix dmat3x2, dmat3, dmat3x4 --------------------------------- */
    {GL_DOUBLE_MAT3x2,      {"GL_DOUBLE_MAT3x2",     6, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT3,        {"GL_DOUBLE_MAT3",       9, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT3x4,      {"GL_DOUBLE_MAT3x4",    12, sizeof(GLdouble), GL_DOUBLE}},

    /* ---- Matrix dmat4x2, dmat4x3, dmat4 --------------------------------- */
    {GL_DOUBLE_MAT4x2,      {"GL_DOUBLE_MAT4x2",     8, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT4x3,      {"GL_DOUBLE_MAT4x3",    12, sizeof(GLdouble), GL_DOUBLE}},
    {GL_DOUBLE_MAT4,        {"GL_DOUBLE_MAT4",      16, sizeof(GLdouble), GL_DOUBLE}},

    /* ---- Matrix mat2, mat2x3, mat2x4 ------------------------------------ */
    {GL_FLOAT_MAT2,         {"GL_FLOAT_MAT2",       4, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT2x3,       {"GL_FLOAT_MAT2x3",     6, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT2x4,       {"GL_FLOAT_MAT2x4",     8, sizeof(GLfloat), GL_FLOAT}},

    /* ---- ---- Matrix mat3x2, mat3, mat3x4 ------------------------------- */
    {GL_FLOAT_MAT3x2,       {"GL_FLOAT_MAT3x2",      6, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT3,         {"GL_FLOAT_MAT3",        9, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT3x4,       {"GL_FLOAT_MAT3x4",     12, sizeof(GLfloat), GL_FLOAT}},

    /* ---- Matrix mat4x2, mat4x3, mat4 ------------------------------------ */
    {GL_FLOAT_MAT4x2,       {"GL_FLOAT_MAT4x2",      8, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT4x3,       {"GL_FLOAT_MAT4x3",     12, sizeof(GLfloat), GL_FLOAT}},
    {GL_FLOAT_MAT4,         {"GL_FLOAT_MAT4",       16, sizeof(GLfloat), GL_FLOAT}},

    /* ---- Sampler data types --------------------------------------------- */
    {GL_SAMPLER_1D,         {"GL_SAMPLER_1D",           1, sizeof(GLint), GL_INT}},
    {GL_SAMPLER_2D,         {"GL_SAMPLER_2D",           1, sizeof(GLint), GL_INT}},
    {GL_SAMPLER_3D,         {"GL_SAMPLER_3D",           1, sizeof(GLint), GL_INT}},
    {GL_SAMPLER_BUFFER,     {"GL_SAMPLER_BUFFER",       1, sizeof(GLint), GL_INT}},
    {GL_SAMPLER_2D_RECT,    {"GL_SAMPLER_2D_RECT",      1, sizeof(GLint), GL_INT}},

    {GL_INT_SAMPLER_1D,     {"GL_INT_SAMPLER_1D",       1, sizeof(GLint), GL_INT}},
    {GL_INT_SAMPLER_2D,     {"GL_INT_SAMPLER_2D",       1, sizeof(GLint), GL_INT}},
    {GL_INT_SAMPLER_3D,     {"GL_INT_SAMPLER_3D",       1, sizeof(GLint), GL_INT}},
    {GL_INT_SAMPLER_BUFFER, {"GL_INT_SAMPLER_BUFFER",   1, sizeof(GLint), GL_INT}},
    {GL_INT_SAMPLER_2D_RECT,{"GL_INT_SAMPLER_2D_RECT",  1, sizeof(GLint), GL_INT}},

    {GL_UNSIGNED_INT_SAMPLER_1D,     {"GL_UNSIGNED_INT_SAMPLER_1D",      1, sizeof(GLint), GL_INT}},
    {GL_UNSIGNED_INT_SAMPLER_2D,     {"GL_UNSIGNED_INT_SAMPLER_2D",      1, sizeof(GLint), GL_INT}},
    {GL_UNSIGNED_INT_SAMPLER_3D,     {"GL_UNSIGNED_INT_SAMPLER_3D",      1, sizeof(GLint), GL_INT}},
    {GL_UNSIGNED_INT_SAMPLER_BUFFER, {"GL_UNSIGNED_INT_SAMPLER_BUFFER",  1, sizeof(GLint), GL_INT}},
    {GL_UNSIGNED_INT_SAMPLER_2D_RECT,{"GL_UNSIGNED_INT_SAMPLER_2D_RECT", 1, sizeof(GLint), GL_INT}},
};

/**
 * @brief Does the map contain the key of the GL data type?
 */
bool Variable::Contains(const GLenum datatype)
{
    return (TypeMap.find(datatype) != TypeMap.end());
}

/**
 * @brief Return the name of the GL data type.
 */
std::string Variable::Name(const GLenum datatype)
{
    static const std::string empty_name;
    if (!Contains(datatype)) {
        return empty_name;
    }
    return (TypeMap.at(datatype)).name;
}

/**
 * @brief Return the length of the GL data type in units of the primitive
 * data type.
 */
GLuint Variable::Length(const GLenum datatype)
{
    if (!Contains(datatype)) {
        return 0;
    }
    return (TypeMap.at(datatype)).length;
}

/**
 * @brief Return the size of the underlying primitive data type in bytes.
 */
GLuint Variable::Size(const GLenum datatype)
{
    if (!Contains(datatype)) {
        return 0;
    }
    return (TypeMap.at(datatype)).size;
}

/**
 * @brief Return the enumerated type of the underlying primitive data type.
 */
GLenum Variable::Type(const GLenum datatype)
{
    if (!Contains(datatype)) {
        return 0;
    }
    return (TypeMap.at(datatype)).type;
}

/** ---------------------------------------------------------------------------
 * @brief Return all active uniforms in a shader program object.
 *
 * Call glGetProgramiv with parameter GL_ACTIVE_UNIFORMS to get the number of
 * active uniform variables in the shader program object.
 *
 * Call glGetProgramiv with parameter GL_ACTIVE_UNIFORM_MAX_LENGTH to get the
 * length of the longest active uniform name in the shader program.
 *
 * Call glGetActiveUniform for each active uniform variable to query its name,
 * type and length.
 *
 * Call glGetUniformLocation to get the location of the uniform variable name
 * in the shader program object. This function returns -1 if name is not an
 * active uniform variable. Possible data types are (cf. glGetActiveUniform):
 *      GL_FLOAT, GL_FLOAT_VEC{2,3,4}
 *      GL_DOUBLE, GL_DOUBLE_VEC{2,3,4}
 *
 *      GL_INT, GL_INT_VEC{2,3,4}
 *      GL_UNSIGNED_INT, GL_UNSIGNED_INT_VEC{2,3,4}
 *
 *      GL_FLOAT_MAT{2,2x3,2x4}
 *      GL_FLOAT_MAT{3x2,3,3x4}
 *      GL_FLOAT_MAT{4x2,4x3,4}
 *
 *      GL_DOUBLE_MAT{2,2x3,2x4}
 *      GL_DOUBLE_MAT{3x2,3,3x4}
 *      GL_DOUBLE_MAT{4x2,4x3,4}
 *
 *      GL_SAMPLER_{1D,2D,3D}
 *      GL_INT_SAMPLER_{1D,2D,3D}
 *      GL_UNSIGNED_INT_SAMPLER_{1D,2D,3D}
 */
std::vector<Variable> GetUniformVariables(const GLuint &program)
{
    std::vector<Variable> uniforms;
    if (program == 0) {
        return uniforms;
    }

    GLint n_uniforms = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &n_uniforms);
    if (n_uniforms == 0) {
        return uniforms;
    }

    GLint max_length;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);

    GLint count;
    GLenum type;
    std::vector<GLchar> name(max_length);
    for (GLuint i = 0; i < static_cast<GLuint>(n_uniforms); ++i) {
        glGetActiveUniform(
            program,
            i,
            static_cast<GLsizei>(max_length),
            nullptr,  /* don't return num of chars written */
            &count,
            &type,
            name.data());

        GLint location = glGetUniformLocation(program, name.data());
        ito_assert(location != -1, "uniform name is inactive or invalid");

        uniforms.push_back({std::string(name.data()), location, count, type});
    }

    return uniforms;
}

/**
 * @brief Retrieve all active attributes in a shader program object.
 *
 * Call glGetProgramiv with parameter GL_ACTIVE_ATTRIBUTES to get the number of
 * active attribute variables in the shader program object.
 *
 * Call glGetProgramiv with parameter GL_ACTIVE_ATTRIBUTE_MAX_LENGTH to get the
 * length of the longest active attribute name in the shader program.
 *
 * Call glGetActiveAttrib for each active attribute variable to query its name,
 * type and size.
 *
 * Call glGetAttribLocation to get the location of the attribute variable name
 * in the shader program object. This function returns -1 if name is not an
 * active attribute variable. Possible data types are (cf. glGetActiveAttrib):
 *      GL_FLOAT, GL_FLOAT_VEC{2,3,4}
 *      GL_DOUBLE, GL_DOUBLE_VEC{2,3,4}
 *
 *      GL_INT, GL_INT_VEC{2,3,4}
 *      GL_UNSIGNED_INT, GL_UNSIGNED_INT_VEC{2,3,4}
 *
 *      GL_FLOAT_MAT{2,2x3,2x4}
 *      GL_FLOAT_MAT{3x2,3,3x4}
 *      GL_FLOAT_MAT{4x2,4x3,4}
 *
 *      GL_DOUBLE_MAT{2,2x3,2x4}
 *      GL_DOUBLE_MAT{3x2,3,3x4}
 *      GL_DOUBLE_MAT{4x2,4x3,4}
 */
std::vector<Variable> GetAttributeVariables(const GLuint &program)
{
    std::vector<Variable> attributes;
    if (program == 0) {
        return attributes;
    }

    GLint n_attributes = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &n_attributes);
    if (n_attributes == 0) {
        return attributes;
    }

    GLint max_length;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);

    GLint count;
    GLenum type;
    std::vector<GLchar> name(max_length);
    for (GLuint i = 0; i < static_cast<GLuint>(n_attributes); ++i) {
        glGetActiveAttrib(
            program,
            i,
            static_cast<GLsizei>(max_length),
            nullptr,  /* don't return num of chars written */
            &count,
            &type,
            name.data());

        GLint location = glGetAttribLocation(program, name.data());
        ito_assert(location != -1, "attribute name is inactive or invalid");

        attributes.push_back({std::string(name.data()), location, count, type});
    }

    return attributes;
}

} /* gl */
} /* ito */
