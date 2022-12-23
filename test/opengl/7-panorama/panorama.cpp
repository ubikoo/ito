/*
 * panorama.cpp
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
#include "panorama.hpp"
#include "camera.hpp"

/**
 * @brief Panorama constant parameters.
 */
static const std::string kImageFilename = "../common/equirectangular.png";
static const size_t kMeshNodes = 1024;

/**
 * @brief Create a new panorama.
 */
Panorama Panorama::Create()
{
    Panorama panorama;

    /*
     * Create the shader program object.
     */
    {
        std::vector<GLuint> shaders{
            gl::CreateShader(GL_VERTEX_SHADER, "data/panorama.vert"),
            gl::CreateShader(GL_FRAGMENT_SHADER, "data/panorama.frag")};
        panorama.program = gl::CreateProgram(shaders);
        gl::DestroyShader(shaders);
        std::cout << gl::GetProgramInfoStr(panorama.program) << "\n";
    }

    /*
     * Load the 2d-image from the specified filename
     */
    {
        panorama.image = gl::Image::Load(kImageFilename, true, 4);
        panorama.texture = gl::CreateTexture2d(
            GL_RGBA8,                           /* internal format */
            panorama.image.width,               /* texture width */
            panorama.image.height,              /* texture height */
            panorama.image.format,              /* pixel format */
            GL_UNSIGNED_BYTE,                   /* pixel type */
            &panorama.image.bitmap[0]);         /* pixel data */

        glBindTexture(GL_TEXTURE_2D, panorama.texture);
        gl::SetTextureMipmap(GL_TEXTURE_2D);
        gl::SetTextureWrap(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        gl::SetTextureFilter(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        /*
         * Create a mesh over a rectangle.
         */
        panorama.mesh = gl::Mesh::Plane(
            panorama.program,           /* shader program object */
            "quad",                     /* vertex attributes prefix */
            kMeshNodes,                 /* n1 vertices */
            kMeshNodes,                 /* n2 vertices */
            -1.0,                       /* xlo */
            1.0,                        /* xhi */
            -1.0,                       /* ylo */
            1.0);                       /* yhi */
    }

    /*
     * Initialize panorama camera.
     */
    {
        panorama.camera = Camera{
             0.0f,                      /* theta */
             0.0f,                      /* phi */
             0.01f * M_PI,              /* theta_step */
             0.02f * M_PI};             /* phi_step */
    }

    return panorama;
}

/**
 * @brief Destroy a panorama.
 */
void Panorama::Destroy(Panorama &panorama)
{
    gl::Mesh::Destroy(panorama.mesh);
    gl::DestroyTexture(panorama.texture);
    gl::DestroyProgram(panorama.program);
}

/**
 * @brief Handle the event in the panorama.
 */
void Panorama::Handle(Panorama &panorama, gl::Renderer::Event &event)
{
    using gl::Renderer::Event;

    if (event.type == Event::Key && event.key.code == GLFW_KEY_LEFT) {
        panorama.camera.rotate_left();
    }

    if (event.type == Event::Key && event.key.code == GLFW_KEY_RIGHT) {
        panorama.camera.rotate_right();
    }

    if (event.type == Event::Key && event.key.code == GLFW_KEY_UP) {
        panorama.camera.rotate_up();
    }

    if (event.type == Event::Key && event.key.code == GLFW_KEY_DOWN) {
        panorama.camera.rotate_down();
    }
}

/**
 * @brief Update the panorama.
 */
void Panorama::Update(Panorama &panorama)
{
    /* Update the panorama camera */
    std::array<GLfloat,2> sizef = gl::Renderer::FramebufferSizef();
    const float fovy = 0.5*M_PI;
    const float aspect = sizef[0] / sizef[1];
    const float znear = 0.1f;
    const float zfar = 10.0f;
    math::mat4f proj = math::perspective(fovy, aspect, znear, zfar);
    math::mat4f view = panorama.camera.lookat();
    panorama.mvp = math::dot(proj, view);
}

/**
 * @brief Render the panorama.
 */
void Panorama::Render(const Panorama &panorama)
{
    GLFWwindow *window = gl::Renderer::Window();
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
    glUseProgram(panorama.program);

    /* Set window dimensions. */
    std::array<GLfloat,2> sizef = gl::Renderer::FramebufferSizef();
    gl::SetUniform(panorama.program, "u_width", GL_FLOAT, &sizef[0]);
    gl::SetUniform(panorama.program, "u_height", GL_FLOAT, &sizef[1]);
    gl::SetUniformMatrix(panorama.program,
        "u_mvp", GL_FLOAT_MAT4, true, panorama.mvp.data);

    /* Set the sampler uniform with the texture unit and bind the texture */
    GLenum texunit = 0;
    gl::SetUniform(panorama.program, "u_texsampler", GL_SAMPLER_2D, &texunit);
    gl::ActiveBindTexture(GL_TEXTURE_2D,
        GL_TEXTURE0 + texunit, panorama.texture);

    /* Draw the mesh */
    gl::Mesh::Render(panorama.mesh);

    /* Unbind the shader program object. */
    glUseProgram(0);
}
