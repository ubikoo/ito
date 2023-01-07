/*
 * sphere.cpp
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
#include "sphere.hpp"

/**
 * @brief Sphere constant parameters.
 */
static const std::string kImageFilename = "../common/equirectangular.png";
static const size_t kMeshNodes = 1024;

/**
 * @brief Create a new sphere.
 */
Sphere Sphere::Create()
{
    Sphere sphere;

    /*
     * Create the shader program object.
     */
    {
        std::vector<GLuint> shaders{
            gl::CreateShader(GL_VERTEX_SHADER, "data/sphere.vert"),
            gl::CreateShader(GL_FRAGMENT_SHADER, "data/sphere.frag")};
        sphere.program = gl::CreateProgram(shaders);
        gl::DestroyShader(shaders);
        std::cout << gl::GetProgramInfoString(sphere.program) << "\n";
    }

    /*
     * Load the 2d-image from the specified filename
     */
    {
        sphere.image = gl::Image::Load(kImageFilename, true, 4);
        sphere.texture = gl::CreateTexture2d(
            GL_RGBA8,                   /* internal format */
            sphere.image.width,         /* texture width */
            sphere.image.height,        /* texture height */
            sphere.image.format,        /* pixel format */
            GL_UNSIGNED_BYTE,           /* pixel type */
            &sphere.image.bitmap[0]);   /* pixel data */

        glBindTexture(GL_TEXTURE_2D, sphere.texture);
        gl::SetTextureMipmap(GL_TEXTURE_2D);
        gl::SetTextureWrap(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        gl::SetTextureFilter(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        /*
         * Create a sphere mesh and set the mesh vertex attributes in the program.
        */
        sphere.mesh = gl::Mesh::Sphere(
            sphere.program,             /* shader program object */
            "sphere",                   /* vertex attributes prefix */
            kMeshNodes,                 /* n1 vertices */
            kMeshNodes,                 /* n2 vertices */
            1.0,                        /* radius */
            0.0,                        /* theta_lo */
            M_PI,                       /* theta_hi */
            -M_PI,                      /* phi_lo */
            M_PI);                      /* phi_hi */
    }

    return sphere;
}

/**
 * @brief Destroy the sphere.
 */
void Sphere::Destroy(Sphere &sphere)
{
    gl::Mesh::Destroy(sphere.mesh);
    gl::DestroyTexture(sphere.texture);
    gl::DestroyProgram(sphere.program);
}

/**
 * @brief Handle the event in the sphere.
 */
void Sphere::Handle(glfw::Event &event)
{}

/**
 * @brief Update the sphere.
 */
void Sphere::Update(void)
{
    /* Update the modelviewprojection matrix */
    float time = (float) glfwGetTime();

    math::mat4f m = math::mat4f::eye;
    m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, (float) (0.5*M_PI));

    math::vec4f dir_y = math::dot(m, math::vec4f{0.0, 1.0, 0.0, 1.0});
    m = math::rotate(m, math::vec3f{dir_y.x, dir_y.y, dir_y.z}, 0.2f*time);

    math::vec4f dir_z = math::dot(m, math::vec4f{0.0, 0.0, 1.0, 1.0});
    m = math::rotate(m, math::vec3f{dir_z.x, dir_z.y, dir_z.z}, 0.8f*time);

    std::array<GLfloat,2> fbsize = {};
    glfw::GetFramebufferSize(fbsize);
    float ratio = fbsize[0] / fbsize[1];

    math::mat4f p = math::ortho(-ratio, ratio, -1.0f, 1.0f, 0.1f, 1.0f);
    mvp = math::dot(p, m);
}

/**
 * @brief Render the sphere.
 */
void Sphere::Render(void)
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
