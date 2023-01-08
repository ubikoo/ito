/*
 * quad.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "ito/opengl.hpp"
#include "quad.hpp"

using namespace ito;

/**
 * @brief Create a new quad.
 */
Quad Quad::Create()
{
    Quad quad;

    /*
     * Vertex positions and color attributes with layout:
     * {(xyzw)_1,
     *  (xyzw)_2,
     *  ...
     *  (rgba)_1,
     *  (rgba)_2}
     */
    std::vector<GLfloat> vertex_data = {
        /* positions */
        -0.5f, -0.5f, 0.0f, 1.0f,   /* bottom left */
         0.5f, -0.5f, 0.0f, 1.0f,   /* bottom right */
        -0.5f,  0.5f, 0.0f, 1.0f,   /* top left */
         0.5f,  0.5f, 0.0f, 1.0f,   /* top right */
        /* colors */
         0.0f,  0.0f, 1.0f, 1.0f,   /* bottom left */
         0.0f,  1.0f, 0.0f, 1.0f,   /* bottom right */
         1.0f,  1.0f, 0.0f, 1.0f,   /* top left */
         1.0f,  0.0f, 0.0f, 1.0f};  /* top right */
    GLsizeiptr vertex_data_size = vertex_data.size() * sizeof(GLfloat);

    /*
     * Create the index data as a shared vertex representation.
     */
    std::vector<GLuint> index_data = {
        0, 1, 2,                    /* first triangle */
        3, 2, 1};                   /* second triangle */
    GLsizeiptr index_data_size = index_data.size() * sizeof(GLuint);

    /*
     * Create the shader program object.
     */
    std::vector<GLuint> shaders{
        gl::CreateShader(GL_VERTEX_SHADER, "data/quad.vert"),
        gl::CreateShader(GL_FRAGMENT_SHADER, "data/quad.frag")};
    quad.program = gl::CreateProgram(shaders);
    gl::DestroyShader(shaders);
    std::cout << gl::GetProgramInfoString(quad.program) << "\n";

    /*
     * Create vertex array object.
     */
    quad.vao = gl::CreateVertexArray();
    glBindVertexArray(quad.vao);

    /*
     * Create a buffer storage for the vertex position and color attributes.
     */
    quad.vbo = gl::CreateBuffer(
        GL_ARRAY_BUFFER,
        vertex_data_size,
        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,            /* target binding point */
        0,                          /* offset in data store */
        vertex_data_size,           /* data store size in bytes */
        vertex_data.data());        /* pointer to data source */

    /*
     * Create a buffer storage for the vertex indices.
     */
    quad.ebo = gl::CreateBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        index_data_size,
        GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.ebo);
    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,    /* target binding point */
        0,                          /* offset in data store */
        index_data_size,            /* data store size in bytes */
        index_data.data());         /* pointer to data source */

    /*
     * Specify how OpenGL interprets the vertex attributes.
     */
    GLsizeiptr offset_pos = 0;
    gl::EnableAttribute(quad.program, "a_pos");
    gl::AttributePointer(
        quad.program,
        "a_pos",
        GL_FLOAT_VEC4,
        4*sizeof(GLfloat),      /* offset between consecutive attributes */
        offset_pos,             /* offset of first element in the buffer */
        false);                 /* normalized flag */

    GLsizeiptr offset_col = vertex_data_size / 2;
    gl::EnableAttribute(quad.program, "a_col");
    gl::AttributePointer(
        quad.program,
        "a_col",
        GL_FLOAT_VEC4,
        4*sizeof(GLfloat),      /* offset between consecutive attributes */
        offset_col,             /* offset of first element in the buffer */
        false);                 /* normalized flag */

    /*
     * Unbind vertex array object.
     */
    glBindVertexArray(0);

    return quad;
}

/**
 * @brief Destroy the quad.
 */
void Quad::Destroy(Quad &quad)
{
    gl::DestroyBuffer(quad.ebo);
    gl::DestroyBuffer(quad.vbo);
    gl::DestroyVertexArray(quad.vao);
    gl::DestroyProgram(quad.program);
}

/**
 * @brief Handle the event in the quad.
 */
void Quad::Handle(glfw::Event &event)
{}

/**
 * @brief Update the quad.
 */
void Quad::Update(void)
{
    /* Update the modelviewprojection matrix */
    float time = (float) glfwGetTime();
    float ang_x = 0.6 * time;
    float ang_y = 0.4 * time;
    float ang_z = 0.2 * time;
    math::mat4f m = math::mat4f::eye;
    m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
    m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
    m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

    std::array<GLfloat,2> fbsize = {};
    glfw::GetFramebufferSize(fbsize);
    float ratio = fbsize[0] / fbsize[1];

    math::mat4f p = math::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
    mvp = math::dot(p, m);
}

/**
 * @brief Render the quad.
 */
void Quad::Render(void)
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
    gl::SetUniformMatrix(program, "u_mvp", GL_FLOAT_MAT4, true, mvp.data);

    glDrawElements(
        GL_TRIANGLES,       /* what kind of primitives to render */
        6,                  /* number of elements to be rendered (3*2 faces) */
        GL_UNSIGNED_INT,    /* type of the values in indices */
        (GLvoid *) 0);      /* offset of the first index in array */

    glBindVertexArray(0);

    /* Unbind the shader program object. */
    glUseProgram(0);
}
