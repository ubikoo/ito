/*
 * program.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "program.hpp"
#include "shader.hpp"
#include "variable.hpp"
#include "uniform.hpp"
#include "attribute.hpp"

namespace ito {
namespace gl {

/**
 * @brief Create a shader program object from a set of shader objects.
 * The program object requires at least two shader stages - vertex and fragment.
 * Each shader stage is compiled and the resulting objects are attached to the
 * program and linked.
 *
 * The sole purpose of the shader objects is to be linked to a shader program
 * object, after which time, they are detached from the program and deleted.
 * Each shader object is assumed to be uniquely attached to a single program.
 *
 *  1. For each shader stage (glCreateShader, glShaderSource, glCompileShader)
 *  2. Create a new program (glCreateProgram)
 *  3. For each shader object (glAttachShader)
 *  4. Link program:: (glLinkProgram)
 *  5. For each shader object (glDetachShader, glDeleteShader)
 *  6. Setup map of active uniform variables (optional).
 *  7. Setup map of active attribute variables (optional).
 *
 * @par Vertex shader input attributes must have their locations specified via:
 *
 *      layout(location = attribute_index) in vec3 value;
 *
 * The location is then queried via glGetAttribLocation. With this syntax, use
 * of glBindAttribLocation is forgone entirely. Attributes that take up multiple
 * attribute slots will be given a sequential block in the order starting with
 * the given attribute. For example, the layout specifier
 *
 *      layout(location = 2) in vec3 values[4];
 *
 * will allocate four attribute indices 2, 3, 4, and 5.
 *
 * @par Fragment shader outputs must have their location indices specified via:
 *
 *      layout(location = output_index) out vec4 output_color;
 *
 * @note Consider tagging variables with an index, such that they correspond by
 * the index instead of by name. For example, given a vertex shader providing:
 *
 *      layout(location = 0) out vec4 out_color;
 *      layout(location = 1) out vec2 out_position;
 *      layout(location = 2) out vec3 out_normal;
 *
 * The consuming shader can match the interface using different names:
 *
 *      layout(location = 0) out vec4 inp_color;
 *      layout(location = 1) out vec2 inp_position;
 *      layout(location = 2) out vec3 inp_normal;
 *
 * @see https://www.khronos.org/opengl/wiki/Shader
 *      https://www.khronos.org/opengl/wiki/Shader_Compilation
 *      https://www.khronos.org/opengl/wiki/Layout_Qualifier_(GLSL)
 *      https://www.khronos.org/opengl/wiki/GLSL_:_common_mistakes
 *      https://stackoverflow.com/questions/440144
 *      https://stackoverflow.com/questions/39923583
 *      https://stackoverflow.com/questions/9113154
 *      https://stackoverflow.com/questions/4635913
 *      https://gamedev.stackexchange.com/questions/22216
 *      https://gamedev.stackexchange.com/questions/47910
 */
GLuint CreateProgram(const std::vector<GLuint> &shaders)
{
    /* Create a new program object. */
    GLuint program = glCreateProgram();
    ito_assert(glIsProgram(program), "failed to create program object");

    /* Attach the shader objects to the program and link. */
    for (auto &it : shaders) {
        glAttachShader(program, it);
    }

    glLinkProgram(program);
    GLint status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        /* Get the number of characters in the program information log. */
        GLsizei infolen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infolen);
        std::vector<GLchar> infolog(infolen+1, '\0');

        /* Get the information log for the specified program object. */
        glGetProgramInfoLog(program, infolen, nullptr, infolog.data());
        ito_throw(ito::str::format(
            "failed to link program:\n\n%s\n", infolog.data()));
    }


    /* Detach the shader objects from the program so they can be deleted. */
    for (auto &it : shaders) {
        glDetachShader(program, it);
    }

    /* Bind the program before return. */
    glUseProgram(program);

    return program;
}

/**
 * @brief Destroy a shader program object. Free the memory and invalidate the
 * identifier associated with the program object.
 */
void DestroyProgram(const GLuint &program)
{
    /* Ignore if program name is 0. */
    if (program == 0) {
        return;
    }

    /* Bind the program and get the number of attached shaders. */
    glUseProgram(program);

    GLint n_shaders;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &n_shaders);
    if (n_shaders > 0) {
        /* Get the handles of each attached shader object. */
        GLsizei max_count = static_cast<GLsizei>(n_shaders);
        std::vector<GLuint> shaders(max_count);
        GLsizei count = 0;
        glGetAttachedShaders(program, max_count, &count, shaders.data());

        /* Detach and delete each shader object. */
        for (GLsizei i = 0; i < count; ++i) {
            glDetachShader(program, shaders[i]);
            glDeleteShader(shaders[i]);
        }
    }

    /* Delete the program. */
    glDeleteProgram(program);
}

/**
 * @brief Return an string containing the shader program object info:
 * 1. Link status.
 * 2. Number of attached shaders.
 * 3. Number of active uniform variables and the name, location,
 *    count and type of each active uniform stored in the map.
 * 4. Number of active attribute variables and the name, location,
 *    count and type of each active attribute stored in the map.
 */
std::string GetProgramInfoString(const GLuint &program)
{
    /*
     * Return an empty vector if program is invalid
     */
    if (!program) {
        std::cerr << ito::str::format("invalid shader program object\n");
        return std::string();
    }

    /*
     * Print shader program header information.
     */
    GLint param;
    std::ostringstream ss;
    glGetProgramiv(program, GL_LINK_STATUS, &param);
    ss << ito::str::format("GL_LINK_STATUS = %d\n", param);
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &param);
    ss << ito::str::format("GL_ATTACHED_SHADERS = %d\n", param);

    /*
     * Print shader program active uniforms information map.
     */
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &param);
    ss << ito::str::format("GL_ACTIVE_UNIFORMS = %d\n", param);
    for (auto &it : GetUniformVariables(program)) {
        ss << ito::str::format(
            "%16s (loc=%d): count %d, type %16s (%5d), length %2u, size %u\n",
            it.name.c_str(),
            it.location,
            it.count,
            Variable::Name(it.type).c_str(),
            it.type,
            Variable::Length(it.type),
            Variable::Size(it.type));
    }

    /*
     * Print shader program active attributes information map.
     */
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &param);
    ss << ito::str::format("GL_ACTIVE_ATTRIBUTES = %d\n", param);
    for (auto &it : GetAttributeVariables(program)) {
        ss << ito::str::format(
            "%16s (loc=%d): count %d, type %16s (%5d), length %2u, size %u\n",
            it.name.c_str(),
            it.location,
            it.count,
            Variable::Name(it.type).c_str(),
            it.type,
            Variable::Length(it.type),
            Variable::Size(it.type));
    }

    return ss.str();
}

} /* gl */
} /* ito */
