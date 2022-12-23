/*
 * shader.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_GLSL_SHADER_H_
#define ITO_OPENGL_GLSL_SHADER_H_

#include <string>
#include <vector>
#include "../base.hpp"

namespace ito {
namespace gl {

/**
 * @brief Shader represents a stage in the rendering pipeline. It contains the
 * shader type and source which are used to create a shader program object.
 *
 * The following stages are recognised by the shader: GL_VERTEX_SHADER,
 * GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER and GL_COMPUTE_SHADER (version>=4.3).
 *
 * Vertex, geometry, and fragment stages have a fixed position in the pipeline.
 * Compute shader is a stage used entirely for computing arbitrary information.
 */
struct Shader {
    GLenum type;
    std::string source;

    explicit Shader(const GLenum type) : type(type) {}
    explicit Shader(const GLenum type, const std::string &source)
        : type(type)
        , source(source)
    {}
    ~Shader() = default;
};

/**
 * @brief Create a new shader object.
 */
GLuint CreateShader(const Shader &stage);

/**
 * @brief Create a new shader object of a specified type from a file.
 */
GLuint CreateShader(const GLenum type, const std::string &filename);

/**
 * @brief Delete a shader program object.
 */
void DestroyShader(const GLuint &shader);

/**
 * @brief Delete a shader program object.
 */
void DestroyShader(const std::vector<GLuint> &shaders);

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_GLSL_SHADER_H_ */
