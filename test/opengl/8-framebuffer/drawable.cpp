/*
 * drawable.cpp
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
#include "drawable.hpp"

/**
 * @brief Drawable constant parameters.
 */
static const std::string kImageFilename = "../common/equirectangular.png";
static const size_t kMeshNodes = 1024;

/**
 * @brief Create a new drawable.
 */
Drawable Drawable::Create()
{
    Drawable drawable;

    /*
     * Create the sphere drawable.
     */
    {
        /* Create the sphere shader program object. */
        std::vector<GLuint> shaders{
            gl::CreateShader(GL_VERTEX_SHADER, "data/sphere.vert"),
            gl::CreateShader(GL_FRAGMENT_SHADER, "data/sphere.frag")};
        drawable.sphere.program = gl::CreateProgram(shaders);
        gl::DestroyShader(shaders);
        std::cout << gl::GetProgramInfoString(drawable.sphere.program) << "\n";

        /* Load the 2d-image from the specified filename. */
        gl::Image image = gl::Image::Load(kImageFilename);
        drawable.sphere.texture = gl::CreateTexture2d(
            GL_RGBA,                    /* internal format */
            image.width,                /* texture width */
            image.height,               /* texture height */
            image.format,               /* pixel format */
            GL_UNSIGNED_BYTE,           /* pixel type */
            &image.bitmap[0]);          /* pixel data */
        glBindTexture(GL_TEXTURE_2D, drawable.sphere.texture);
        gl::SetTextureMipmap(GL_TEXTURE_2D);
        gl::SetTextureWrap(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        gl::SetTextureFilter(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        /* Create a mesh over a sphere. */
        drawable.sphere.mesh = gl::Mesh::Sphere(
            drawable.sphere.program,         /* shader program object */
            "sphere",                   /* vertex attributes prefix */
            kMeshNodes,                 /* n1 vertices */
            kMeshNodes,                 /* n2 vertices */
            1.0,                        /* radius */
            0.0,                        /* theta_lo */
            M_PI,                       /* theta_hi */
            -M_PI,                      /* phi_lo */
            M_PI);                      /* phi_hi */

        /* Initialize sphere view matrix. */
        drawable.sphere.mvp = math::mat4f::eye;
    }

    /*
     * Create the quad drawable.
     */
    {
        std::vector<GLuint> shaders{
            gl::CreateShader(GL_VERTEX_SHADER, "data/quad.vert"),
            gl::CreateShader(GL_FRAGMENT_SHADER, "data/quad.frag")};
        drawable.quad.program = gl::CreateProgram(shaders);
        gl::DestroyShader(shaders);
        std::cout << gl::GetProgramInfoString(drawable.quad.program) << "\n";

        /*
         * Create a mesh over a rectangle.
         */
        drawable.quad.mesh = gl::Mesh::Plane(
            drawable.quad.program,           /* shader program object */
            "quad",                     /* vertex attributes prefix */
            kMeshNodes,                 /* n1 vertices */
            kMeshNodes,                 /* n2 vertices */
            -1.0,                       /* xlo */
            1.0,                        /* xhi */
            -1.0,                       /* ylo */
            1.0);                       /* yhi */

        /* Initialize the quad view matrix */
        drawable.quad.mvp = math::mat4f::eye;
    }

    /*
     * Create a framebuffer with color and depth attachments.
     */
    {
        /* Set the fbo size equal to the sphere dimensions. */
        glBindTexture(GL_TEXTURE_2D, drawable.sphere.texture);
        drawable.fbo.width = gl::GetTextureWidth(GL_TEXTURE_2D);
        drawable.fbo.height = gl::GetTextureHeight(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        /* Create the fbo with color and depth attachments. */
        drawable.fbo.id = gl::CreateFramebuffer(
            drawable.fbo.width,
            drawable.fbo.height,
            1,                      /* 1 color attachment */
            GL_RGBA,                /* color buffer internal format */
            &drawable.fbo.color_texture,
            GL_DEPTH_COMPONENT24,   /* depth buffer internal format */
            &drawable.fbo.depth_texture,
            GL_LINEAR,
            GL_LINEAR_MIPMAP_LINEAR);
    }

    return drawable;
}

/**
 * @brief Destroy the drawable.
 */
void Drawable::Destroy(Drawable &drawable)
{
    /* Destroy sphere objects */
    gl::Mesh::Destroy(drawable.sphere.mesh);
    gl::DestroyTexture(drawable.sphere.texture);
    gl::DestroyProgram(drawable.sphere.program);

    /* Destroy quad objects */
    gl::Mesh::Destroy(drawable.quad.mesh);
    gl::DestroyProgram(drawable.quad.program);

    /* Destroy fbo objects. */
    gl::DestroyTexture(drawable.fbo.color_texture);
    gl::DestroyTexture(drawable.fbo.depth_texture);
    gl::DestroyFramebuffer(drawable.fbo.id);
}

/**
 * @brief Handle the event in the drawable.
 */
void Drawable::Handle(glfw::Event &event)
{}

/**
 * @brief Update the drawable.
 */
void Drawable::Update(void)
{
    /* Compute the orthographic projection matrix */
    float time = (float) glfwGetTime();

    /* Update the sphere modelviewprojection matrix */
    {
        float ang_x = 0.0f * time;
        float ang_y = 2.0f * time;
        float ang_z = 0.0f * time;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

        math::vec4f dir_y = math::dot(m, math::vec4f{0.0f,1.0f,0.0f,1.0f});
        m = math::rotate(m, math::vec3f{dir_y.x, dir_y.y, dir_y.z}, ang_y);

        math::vec4f dir_z = math::dot(m, math::vec4f{0.0f,0.0f,1.0f,1.0f});
        m = math::rotate(m, math::vec3f{dir_z.x, dir_z.y, dir_z.z}, ang_z);

        std::array<GLfloat,2> fbsize = {};
        glfw::GetFramebufferSize(fbsize);
        float ratio = fbsize[0] / fbsize[1];

        math::mat4f proj = math::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        sphere.mvp = math::dot(proj, m);
    }

    /* Update the quad modelviewprojection matrix */
    {
        float ang_x = 0.05f * time;
        float ang_y = 0.05f * time;
        float ang_z = 0.05f * time;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
        m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

        float ratio = (float) fbo.width / fbo.height;
        math::mat4f proj = math::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        quad.mvp = math::dot(proj, m);
    }
}

/**
 * @brief Render the drawable.
 */
void Drawable::Render(void)
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

    /* Render into the framebuffer rendertexture */
    {
        /* Bind the framebuffer */
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo.id);
        std::array<GLint, 4> viewport;
        glfw::GetViewport(viewport);
        glfw::SetViewport({0, 0, fbo.width, fbo.height});
        glfw::ClearBuffers(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

        /* Bind the sphere shader */
        glUseProgram(sphere.program);

        /* Set window dimensions. */
        // gl::SetUniform(sphere.program, "u_width", GL_FLOAT, &fbo.width);
        // gl::SetUniform(sphere.program, "u_height", GL_FLOAT, &fbo.height);
        gl::SetUniformMatrix(sphere.program, "u_mvp", GL_FLOAT_MAT4, true,
            sphere.mvp.data);

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::SetUniform(sphere.program, "u_texsampler", GL_SAMPLER_2D, &texunit);
        gl::ActiveBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            sphere.texture);

        /* Draw the sphere mesh */
        gl::Mesh::Render(sphere.mesh);

        /* Unbind the shader program object. */
        glUseProgram(0);

        /* Unbind the framebuffer */
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glfw::SetViewport(viewport);
    }

    /*
     * Render into the window framebuffer
     */
    {
        /* Bind the quad shader */
        glUseProgram(quad.program);

        // std::array<GLfloat,2> fbsize = {};
        // glfw::GetFramebufferSize(fbsize);
        // gl::SetUniform(quad.program, "u_width", GL_FLOAT, &fbsize[0]);
        // gl::SetUniform(quad.program, "u_height", GL_FLOAT, &fbsize[0]);
        gl::SetUniformMatrix(quad.program, "u_mvp", GL_FLOAT_MAT4, true,
            quad.mvp.data);

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::SetUniform(quad.program, "u_texsampler", GL_SAMPLER_2D, &texunit);
        gl::ActiveBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            fbo.color_texture);

        /* Draw the quad mesh */
        gl::Mesh::Render(quad.mesh);

        /* Unbind the shader program object. */
        glUseProgram(0);
    }
}
