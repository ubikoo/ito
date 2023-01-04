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
     * Load the image from the file, store it on a texture and create mesh.
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

        panorama.mesh = gl::Mesh::Sphere(
            panorama.program,           /* shader program object */
            "sphere",                   /* vertex attributes prefix */
            kMeshNodes,                 /* n1 vertices */
            kMeshNodes,                 /* n2 vertices */
            1.0,                        /* radius */
            0.0,                        /* theta_lo */
            M_PI,                       /* theta_hi */
            -M_PI,                      /* phi_lo */
            M_PI);                      /* phi_hi */
    }

    /*
     * Create panorama camera.
     */
    {
        panorama.camera = Camera::Create(
            math::vec3f{0.0f, 0.0f, 0.0f},  /* eye */
            math::vec3f{1.0f, 0.0f, 0.0f},  /* ctr */
            math::vec3f{0.0f, 0.0f, 1.0f}   /* up */
        );
    }

    return panorama;
}

/**
 * @brief Destroy the panorama.
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
void Panorama::Handle(gl::Renderer::Event &event)
{
    using gl::Renderer::Event;
    static const float speed = 0.01f;
    static const float step = 10.0f;

    if (event.type == Event::Key && event.key.code == GLFW_KEY_W) {
        camera.move(speed * step);
    }

    if (event.type == Event::Key && event.key.code == GLFW_KEY_S) {
        camera.move(-speed * step);
    }

    if (event.type == Event::Key && event.key.code == GLFW_KEY_A) {
        camera.strafe(-speed * step);
    }

    if (event.type == Event::Key && event.key.code == GLFW_KEY_D) {
        camera.strafe(speed * step);
    }

    if (event.type == Event::Key && event.key.code == GLFW_KEY_UP) {
        camera.pitch(speed * M_PI);
    }

    if (event.type == Event::Key && event.key.code == GLFW_KEY_DOWN) {
        camera.pitch(-speed * M_PI);
    }

    if (event.type == Event::Key && event.key.code == GLFW_KEY_LEFT) {
        camera.yaw(-2.0f * speed * M_PI);
    }

    if (event.type == Event::Key && event.key.code == GLFW_KEY_RIGHT) {
        camera.yaw(2.0f * speed * M_PI);
    }
}

/**
 * @brief Update the panorama.
 */
void Panorama::Update(void)
{
    /* Update the panorama camera */
    std::array<GLfloat,2> fsize = gl::Renderer::FramebufferSizef();
    const float fovy = 0.5*M_PI;
    const float aspect = fsize[0] / fsize[1];
    const float znear = 0.1f;
    const float zfar = 10.0f;
    math::mat4f proj = math::perspective(fovy, aspect, znear, zfar);
    math::mat4f view = camera.view();
    mvp = math::dot(proj, view);
}

/**
 * @brief Render the panorama.
 */
void Panorama::Render(void)
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
    glUseProgram(program);

    /* Set window dimensions. */
    std::array<GLfloat,2> fsize = gl::Renderer::FramebufferSizef();
    gl::SetUniform(program, "u_width", GL_FLOAT, &fsize[0]);
    gl::SetUniform(program, "u_height", GL_FLOAT, &fsize[1]);
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
