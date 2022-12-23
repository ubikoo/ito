/*
 * shader.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include <fstream>
#include "shader.hpp"

namespace ito {
namespace gl {

/**
 * @brief Create a new shader object. Compile the shader and query the shader
 * info_log to check the compile status.
 *
 * @note In order to be linked in a program object, the newly created shader
 * must be attached the the program object (cf Program).
 *
 * It is permissible to attach a shader to a program object before source code
 * has been loaded into the shader or before the shader has been compiled.
 *
 * It is permissible to attach multiple shader objects of the same type because
 * each may contain a portion of the complete shader.
 *
 * It is also permissible to attach a shader to more than one program object.
 * If a shader is deleted while it is attached to a program object, it will be
 * flagged for deletion, but deletion occurs only when the shader is detached
 * from all program objects.
 */
GLuint CreateShader(const Shader &stage)
{
    /*
     * Create a shader object and compile.
     */
    GLuint shader = glCreateShader(stage.type);
    ito_assert(glIsShader(shader), "failed to create shader object");

    ito_assert(!stage.source.empty(), "invalid shader source");
    const GLchar *source = static_cast<const GLchar *>(stage.source.c_str());
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    /*
     * Query shader compilation status for errors. GL_COMPILE_STATUS returns
     * GL_TRUE if shader compilation was successful, and GL_FALSE otherwise.
     */
    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> info_log(log_length + 1, '\0');

        glGetShaderInfoLog(shader, log_length, nullptr, info_log.data());
        ito_throw(ito::str::format("failed to compile shader:\n%s\n\n%s\n\n",
            source, info_log.data()));
    }

    return shader;
}

/**
 * @brief Create a new shader object of a specified type from a file.
 */
GLuint CreateShader(const GLenum type, const std::string &filename)
{
    std::ifstream file(filename);
    ito_assert(file, "failed to open program source file");

    std::stringstream source(std::ios::out);
    source << file.rdbuf();
    file.close();

    return CreateShader(Shader(type, source.str()));
}

/**
 * @brief Delete a shader program object
 */
void DestroyShader(const GLuint &shader)
{
    if (shader == 0) {
        return;
    }
    glDeleteShader(shader);
}

/**
 * @brief Delete a shader program object.
 */
void DestroyShader(const std::vector<GLuint> &shaders)
{
    for (auto &shader : shaders) {
        DestroyShader(shader);
    }
}

} /* gl */
} /* ito */
