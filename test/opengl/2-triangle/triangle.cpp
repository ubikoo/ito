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
 * @brief Create a new triangle.
 */
Triangle Triangle::Create()
{
    Triangle triangle;

    /*
     * Vertex position and color attributes with layout:
     * {(xyzw)_1,
     *  (xyzw)_2,
     *  ...
     *  (rgba)_1,
     *  (rgba)_2}
     */
    const std::vector<GLfloat> vertex_data = {
        /* positions */
        -0.5f, -0.5f, 0.0f, 1.0f,   /* bottom left */
         0.5f, -0.5f, 0.0f, 1.0f,   /* bottom right */
        -0.5f,  0.5f, 0.0f, 1.0f,   /* top left */
         0.5f,  0.5f, 0.0f, 1.0f,   /* top right */
        /* colors */
         0.0f,  0.0f, 1.0f, 1.0f,   /* bottom left */
         0.0f,  1.0f, 0.0f, 1.0f,   /* bottom right */
         1.0f,  0.0f, 0.0f, 1.0f,   /* top left */
         1.0f,  0.0f, 0.0f, 1.0f};  /* top right */
    const GLsizeiptr vertex_data_size = vertex_data.size() * sizeof(GLfloat);

    /*
     * Triangle shader program object.
     */
    std::vector<GLuint> shaders{
        gl::CreateShader(GL_VERTEX_SHADER, "data/triangle.vert"),
        gl::CreateShader(GL_FRAGMENT_SHADER, "data/triangle.frag")};
    triangle.program = gl::CreateProgram(shaders);
    gl::DestroyShader(shaders);
    std::cout << gl::GetProgramInfoStr(triangle.program) << "\n";

    /*
     * Create vertex array object.
     */
    triangle.vao = gl::CreateVertexArray();
    glBindVertexArray(triangle.vao);

    /*
     * Create buffer storage for vertex position and color attributes.
     */
    triangle.vbo = gl::CreateBuffer(
        GL_ARRAY_BUFFER,
        vertex_data_size,
        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, triangle.vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,        /* target binding point */
        0,                      /* offset in data store */
        vertex_data_size,       /* data store size in bytes */
        vertex_data.data());    /* pointer to data source */

    /*
     * Specify how OpenGL interprets the vertex attributes.
     */
    GLsizeiptr offset_pos = 0;
    gl::EnableAttribute(triangle.program, "a_pos");
    gl::AttributePointer(
        triangle.program,
        "a_pos",
        GL_FLOAT_VEC4,
        4 * sizeof(GLfloat),    /* offset between consecutive attributes */
        offset_pos,             /* offset of first element in the buffer */
        false);                 /* normalized flag */

    GLsizeiptr offset_col = vertex_data_size / 2;
    gl::EnableAttribute(triangle.program, "a_col");
    gl::AttributePointer(
        triangle.program,
        "a_col",
        GL_FLOAT_VEC4,
        4 * sizeof(GLfloat),    /* offset between consecutive attributes */
        offset_col,             /* offset of first element in the buffer */
        false);                 /* normalized flag */

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
{}

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

    std::array<GLfloat,2> fbsize = {};
    glfw::GetFramebufferSize(fbsize);
    gl::SetUniform(program, "u_width", GL_FLOAT, &fbsize[0]);
    gl::SetUniform(program, "u_height", GL_FLOAT, &fbsize[1]);

    glDrawArrays(
        GL_TRIANGLES,           /* what kind of primitives to render */
        0,                      /* starting index in the enabled arrays */
        3);                     /* number of indices to be rendered */

    glBindVertexArray(0);

    /* Unbind the shader program object. */
    glUseProgram(0);
}
