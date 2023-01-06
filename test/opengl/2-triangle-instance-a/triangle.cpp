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
static const size_t kNumTriangles = 20;

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
     * Compute the triangle offset along each dimension.
     */
    for (size_t i = 0; i < kNumTriangles; ++i) {
        triangle.mvp.push_back(math::mat4f::eye);
    }

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
{
    /* Update the modelviewprojection matrix */
    std::array<GLfloat,2> fbsize = {};
    glfw::GetFramebufferSize(fbsize);
    float ratio = fbsize[0] / fbsize[1];
    float length = 0.5 * (float) mvp.size();
    math::mat4f project = math::ortho(
        -ratio*length, ratio*length,
        -length, length,
        -length, length);

    /* Update triangle modelviewprojection matrices */
    float time = (float) glfwGetTime();
    for (size_t i = 0; i < mvp.size(); ++i) {
        math::mat4f &m = mvp[i];

        /* rotate the matrix */
        float ang_x = 0.6 * time * (float) (i+1) / (float) mvp.size();
        float ang_y = 0.4 * time * (float) (i+1) / (float) mvp.size();
        float ang_z = 0.2 * time * (float) (i+1) / (float) mvp.size();

        m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
        m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

        /* translate the matrix */
        float radius = std::cos(0.5*time) * (float) mvp.size() / M_PI;
        float u1 = 2.0f * M_PI * (float) i / mvp.size();
        float u2 = 2.0f * (float) i / mvp.size() - 1.0;
        math::vec3f r_trans{radius * std::cos(u1), radius * std::sin(u1), u2};

        m = math::translate(m, r_trans);
        m = dot(project, m);
    }
}

/**
 * @brief Render the
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

    for (auto &m : mvp) {
        gl::SetUniformMatrix(program, "u_mvp", GL_FLOAT_MAT4, true, m.data);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glBindVertexArray(0);

    /* Unbind the shader program object. */
    glUseProgram(0);
}
