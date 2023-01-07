/*
 * triangle.cpp
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
#include "triangle.hpp"

/**
 * @brief Number of triangle instances.
 */
static const size_t kNumCells = 100;

/**
 * @brief Create a new triangle.
 */
Triangle Triangle::Create()
{
    Triangle triangle;

    /*
     * Vertex positions and color attributes with layout:
     * {(xyzw)_1,
     *  (xyzw)_2,
     *  ...
     *  (rgba)_1,
     *  (rgba)_2}
     */
    const GLfloat scale = 1.0 / static_cast<GLfloat>(kNumCells);
    std::vector<GLfloat> vertex_data = {
        /* positions */
        -1.0f*scale, -1.0f*scale, 0.0f*scale, 1.0f, /* bottom left */
         1.0f*scale, -1.0f*scale, 0.0f*scale, 1.0f, /* bottom right */
        -0.0f*scale,  1.0f*scale, 0.0f*scale, 1.0f, /* top */
        /* colors */
         0.0f,  0.0f, 1.0f, 1.0f,                   /* bottom left */
         0.0f,  1.0f, 0.0f, 1.0f,                   /* bottom right */
         1.0f,  0.0f, 0.0f, 1.0f};                  /* top  */

    /*
     * Compute the rop offset along each dimension.
     */
    triangle.offset.data.clear();
    for (size_t i = 0; i < kNumCells; ++i) {
        for (size_t j = 0; j < kNumCells; ++j) {
            for (size_t k = 0; k < kNumCells; ++k) {
                float x = -1.0f + 2.0f * scale * static_cast<GLfloat>(i);
                float y = -1.0f + 2.0f * scale * static_cast<GLfloat>(j);
                float z = -1.0f + 2.0f * scale * static_cast<GLfloat>(k);
                triangle.offset.data.push_back(x);
                triangle.offset.data.push_back(y);
                triangle.offset.data.push_back(z);
            }
        }
    }

    /*
     * Create the shader program object.
     */
    std::vector<GLuint> shaders{
        gl::CreateShader(GL_VERTEX_SHADER, "data/triangle.vert"),
        gl::CreateShader(GL_FRAGMENT_SHADER, "data/triangle.frag")};
    triangle.program = gl::CreateProgram(shaders);
    gl::DestroyShader(shaders);
    std::cout << gl::GetProgramInfoString(triangle.program) << "\n";

    /*
     * Create vertex array object.
     */
    triangle.vao = gl::CreateVertexArray();
    glBindVertexArray(triangle.vao);

    /*
     * Create a buffer storage for the vertex position and color attributes.
     */
    GLsizeiptr vertex_data_size = vertex_data.size() * sizeof(GLfloat);
    triangle.vbo = gl::CreateBuffer(
        GL_ARRAY_BUFFER,
        vertex_data_size,
        GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, triangle.vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,            /* target binding point */
        0,                          /* offset in data store */
        vertex_data_size,           /* data store size in bytes */
        vertex_data.data());        /* pointer to data source */

    /*
     * Specify how OpenGL interprets the vertex attributes.
     */
    GLsizeiptr offset_pos = 0;
    gl::EnableAttribute(triangle.program, "a_pos");
    gl::AttributePointer(
        triangle.program,
        "a_pos",
        GL_FLOAT_VEC4,
        4 * sizeof(GLfloat),        /* offset between consecutive attributes */
        offset_pos,                 /* offset of first element in the buffer */
        false);                     /* normalized flag */

    GLsizeiptr offset_col = vertex_data_size / 2;
    gl::EnableAttribute(triangle.program, "a_col");
    gl::AttributePointer(
        triangle.program,
        "a_col",
        GL_FLOAT_VEC4,
        4 * sizeof(GLfloat),        /* offset between consecutive attributes */
        offset_col,                 /* offset of first element in the buffer */
        false);                     /* normalized flag */

    /*
     * Create a buffer storage for the rop offsets.
     */
    GLsizeiptr offset_data_size = triangle.offset.data.size() * sizeof(GLfloat);
    triangle.offset.vbo = gl::CreateBuffer(
        GL_ARRAY_BUFFER,
        offset_data_size,
        GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, triangle.offset.vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,                /* target binding point */
        0,                              /* offset in data store */
        offset_data_size,               /* data store size in bytes */
        triangle.offset.data.data());   /* pointer to data source */

    gl::EnableAttribute(triangle.program, "a_offset");
    gl::AttributePointer(
        triangle.program,
        "a_offset",
        GL_FLOAT_VEC3,
        sizeof(GLfloat),            /* offset between consecutive attributes */
        0,                          /* offset of first element in the buffer */
        false);                     /* normalized flag */
    gl::AttributeDivisor(triangle.program, "a_offset", 1);

    /*
     * Unbind vertex array object.
     */
    glBindVertexArray(0);

    return triangle;
}

/**
 * @brief Destroy the triangle.
 */
void Triangle::Destroy(Triangle &triangle)
{
    gl::DestroyBuffer(triangle.vbo);
    gl::DestroyVertexArray(triangle.vao);
    gl::DestroyProgram(triangle.program);
}

/**
 * @brief Handle the event in the triangle.
 */
void Triangle::Handle(glfw::Event &event)
{}

/**
 * @brief Update the triangle.
 */
void Triangle::Update(void)
{
    /* Update the modelviewprojection matrix */
    float time = (float) glfwGetTime();
    float ang_x = 0.3f * time;
    float ang_y = 0.2f * time;
    float ang_z = 0.1f * time;

    math::mat4f m = math::mat4f::eye;
    m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
    m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
    m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);
    m = math::scale(m, math::vec3f::ones);

    std::array<GLfloat,2> fbsize = {};
    glfw::GetFramebufferSize(fbsize);
    float ratio = fbsize[0] / fbsize[1];
    math::mat4f p = math::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
    mvp = math::dot(p, m);
}

/**
 * @brief Render the triangle.
 */
void Triangle::Render(void)
{
    GLFWwindow *window = glfw::Window();
    if (window == nullptr) {
        return;
    }

    /* Specify draw state modes. */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /* Bind the shader program object. */
    glUseProgram(program);

    /* Get window dimensions and set corresponding uniforms. */
    glBindVertexArray(vao);
    gl::SetUniformMatrix(program, "u_mvp", GL_FLOAT_MAT4, true, mvp.data);

    /* Draw multiple instances of a range of elements. */
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, offset.data.size());
    glBindVertexArray(0);

    /* Unbind the shader program object. */
    glUseProgram(0);
}