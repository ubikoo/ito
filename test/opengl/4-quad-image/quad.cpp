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
using namespace ito;
#include "quad.hpp"

/**
 * @brief Quad constant parameters.
 */
static const std::string kImageFilename = "../common/baboon_512.png";
static const size_t kMeshNodes = 1024;

/**
 * @brief Create a new quad.
 */
Quad Quad::Create()
{
    Quad quad;

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
     * Load the 2d-image from the specified filename
     */
    quad.image = gl::Image::Load(kImageFilename, true, 4);
    quad.texture = gl::CreateTexture2d(
        GL_RGBA8,                   /* internal format */
        quad.image.width,           /* texture width */
        quad.image.height,          /* texture height */
        quad.image.format,          /* pixel format */
        GL_UNSIGNED_BYTE,           /* pixel type */
        &quad.image.bitmap[0]);     /* pixel data */

    glBindTexture(GL_TEXTURE_2D, quad.texture);
    gl::SetTextureMipmap(GL_TEXTURE_2D);
    gl::SetTextureWrap(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    gl::SetTextureFilter(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    /*
     * Create a mesh over a rectangle.
     */
    quad.mesh = gl::Mesh::Plane(
        quad.program,               /* shader program object */
        "quad",                     /* vertex attributes prefix */
        kMeshNodes,                 /* n1 vertices */
        kMeshNodes,                 /* n2 vertices */
        -1.0,                       /* xlo */
         1.0,                       /* xhi */
        -1.0,                       /* ylo */
         1.0);                      /* yhi */

    return quad;
}

/**
 * @brief Destroy the quad.
 */
void Quad::Destroy(Quad &quad)
{
    gl::Mesh::Destroy(quad.mesh);
    gl::DestroyTexture(quad.texture);
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

    float ang_x = 0.8 * time;
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

    /* Set window dimensions. */
    std::array<GLfloat,2> fbsize = {};
    glfw::GetFramebufferSize(fbsize);
    gl::SetUniform(program, "u_width", GL_FLOAT, &fbsize[0]);
    gl::SetUniform(program, "u_height", GL_FLOAT, &fbsize[1]);
    gl::SetUniformMatrix(program, "u_mvp", GL_FLOAT_MAT4, true, mvp.data);

    /* Set the sampler uniform with the texture unit and bind the texture */
    GLenum texunit = 0;
    gl::SetUniform(program, "u_texsampler", GL_SAMPLER_2D, &texunit);
    gl::ActiveBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit, texture);

    /* Draw the mesh */
    gl::Mesh::Render(mesh);

    /* Unbind the shader program object. */
    glUseProgram(0);
}
