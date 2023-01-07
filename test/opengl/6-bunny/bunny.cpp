/*
 * bunny.cpp
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
#include "bunny.hpp"

/**
 * @brief Bunny constant parameters.
 */
static const std::string kImageFilename = "../common/bunny.ply";
static const size_t kMeshNodes = 1024;

/**
 * @brief Create a new bunny.
 */
Bunny Bunny::Create()
{
    Bunny bunny;

    /*
     * Create the shader program object.
     */
    std::vector<GLuint> shaders{
        gl::CreateShader(GL_VERTEX_SHADER, "data/bunny.vert"),
        gl::CreateShader(GL_FRAGMENT_SHADER, "data/bunny.frag")};
    bunny.program = gl::CreateProgram(shaders);
    gl::DestroyShader(shaders);
    std::cout << gl::GetProgramInfoString(bunny.program) << "\n";

    /*
     * Load the bunny model meshes.
     */
    bunny.model = gl::Mesh::Load(bunny.program, "bunny", kImageFilename);

    return bunny;
}

/**
 * @brief Destroy the bunny.
 */
void Bunny::Destroy(Bunny &bunny)
{
    for (auto &mesh : bunny.model) {
        gl::Mesh::Destroy(mesh);
    }
    gl::DestroyProgram(bunny.program);
}

/**
 * @brief Handle the event in the bunny.
 */
void Bunny::Handle(glfw::Event &event)
{}

/**
 * @brief Update the bunny.
 */
void Bunny::Update(void)
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
    m = math::scale(m, math::vec3f{5.0f, 5.0f, 5.0f});

    std::array<GLfloat,2> fbsize = {};
    glfw::GetFramebufferSize(fbsize);
    float ratio = fbsize[0] / fbsize[1];

    math::mat4f p = math::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
    mvp = math::dot(p, m);
}

/**
 * @brief Render the bunny.
 */
void Bunny::Render(void)
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

    /* Set uniform and draw. */
    gl::SetUniformMatrix(program, "u_mvp", GL_FLOAT_MAT4, true, mvp.data);

    /* Draw the mesh */
    for (auto &mesh : model) {
        gl::Mesh::Render(mesh);
    }

    /* Unbind the shader program object. */
    glUseProgram(0);
}
