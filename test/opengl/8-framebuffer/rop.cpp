/*
 * rop.cpp
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
#include "rop.hpp"

/**
 * @brief Rop constant parameters.
 */
static const std::string kImageFilename = "../common/equirectangular.png";
static const size_t kMeshNodes = 1024;

/**
 * @brief Create a new rop.
 */
Rop Rop::Create()
{
    Rop rop;

    /*
     * Create the sphere drawable.
     */
    {
        /* Create the sphere shader program object. */
        std::vector<GLuint> shaders{
            gl::CreateShader(GL_VERTEX_SHADER, "data/sphere.vert"),
            gl::CreateShader(GL_FRAGMENT_SHADER, "data/sphere.frag")};
        rop.sphere.program = gl::CreateProgram(shaders);
        gl::DestroyShader(shaders);
        std::cout << gl::GetProgramInfoStr(rop.sphere.program) << "\n";

        /* Load the 2d-image from the specified filename. */
        gl::Image image = gl::Image::Load(kImageFilename);
        rop.sphere.texture = gl::CreateTexture2d(
            GL_RGBA,                    /* internal format */
            image.width,                /* texture width */
            image.height,               /* texture height */
            image.format,               /* pixel format */
            GL_UNSIGNED_BYTE,           /* pixel type */
            &image.bitmap[0]);          /* pixel data */
        glBindTexture(GL_TEXTURE_2D, rop.sphere.texture);
        gl::SetTextureMipmap(GL_TEXTURE_2D);
        gl::SetTextureWrap(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        gl::SetTextureFilter(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        /* Create a mesh over a sphere. */
        rop.sphere.mesh = gl::Mesh::Sphere(
            rop.sphere.program,         /* shader program object */
            "sphere",                   /* vertex attributes prefix */
            kMeshNodes,                 /* n1 vertices */
            kMeshNodes,                 /* n2 vertices */
            1.0,                        /* radius */
            0.0,                        /* phi_lo */
            M_PI,                       /* phi_hi */
            -M_PI,                      /* theta_lo */
            M_PI);                      /* theta_hi */

        /* Initialize sphere view matrix. */
        rop.sphere.mvp = math::mat4f::eye;
    }

    /*
     * Create the quad drawable.
     */
    {
        std::vector<GLuint> shaders{
            gl::CreateShader(GL_VERTEX_SHADER, "data/quad.vert"),
            gl::CreateShader(GL_FRAGMENT_SHADER, "data/quad.frag")};
        rop.quad.program = gl::CreateProgram(shaders);
        gl::DestroyShader(shaders);
        std::cout << gl::GetProgramInfoStr(rop.quad.program) << "\n";

        /*
         * Create a mesh over a rectangle.
         */
        rop.quad.mesh = gl::Mesh::Plane(
            rop.quad.program,           /* shader program object */
            "quad",                     /* vertex attributes prefix */
            kMeshNodes,                 /* n1 vertices */
            kMeshNodes,                 /* n2 vertices */
            -1.0,                       /* xlo */
            1.0,                        /* xhi */
            -1.0,                       /* ylo */
            1.0);                       /* yhi */

        /* Initialize the quad view matrix */
        rop.quad.mvp = math::mat4f::eye;
    }

    /*
     * Create a framebuffer with color and depth attachments.
     */
    {
        /* Initialize the fbo mixing parameter. */
        rop.fbo.mix = 0.0f;

        /* Set the fbo size equal to the sphere dimensions. */
        glBindTexture(GL_TEXTURE_2D, rop.sphere.texture);
        rop.fbo.width = gl::GetTextureWidth(GL_TEXTURE_2D);
        rop.fbo.height = gl::GetTextureHeight(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        /* Create the fbo with color and depth attachments. */
        rop.fbo.id = gl::CreateFramebuffer(
            rop.fbo.width,
            rop.fbo.height,
            1,                      /* 1 color attachment */
            GL_RGBA,                /* color buffer internal format */
            &rop.fbo.color_texture,
            GL_DEPTH_COMPONENT24,   /* depth buffer internal format */
            &rop.fbo.depth_texture,
            GL_LINEAR,
            GL_LINEAR_MIPMAP_LINEAR);
    }

    return rop;
}

/**
 * @brief Destroy a rop.
 */
void Rop::Destroy(Rop &rop)
{
    /* Destroy sphere objects */
    gl::Mesh::Destroy(rop.sphere.mesh);
    gl::DestroyTexture(rop.sphere.texture);
    gl::DestroyProgram(rop.sphere.program);

    /* Destroy quad objects */
    gl::Mesh::Destroy(rop.quad.mesh);
    gl::DestroyProgram(rop.quad.program);

    /* Destroy fbo objects. */
    gl::DestroyTexture(rop.fbo.color_texture);
    gl::DestroyTexture(rop.fbo.depth_texture);
    gl::DestroyFramebuffer(rop.fbo.id);
}

/**
 * @brief Handle the event in the rop.
 */
void Rop::Handle(Rop &rop, gl::Renderer::Event &event)
{
    using gl::Renderer::Event;

    if (event.type == Event::MouseScroll) {
        rop.fbo.mix += 0.01f * event.mousescroll.yoffset;
        rop.fbo.mix = std::min(std::max(rop.fbo.mix, 0.0f), 1.0f);
    }
}

/**
 * @brief Update the rop.
 */
void Rop::Update(Rop &rop)
{
    /* Compute the orthographic projection matrix */
    float time = (float) glfwGetTime();

    /* Update the sphere modelviewprojection matrix */
    {
        float ang_x = 1.2f * (float) M_PI;
        float ang_y = 0.8f * time;
        float ang_z = 0.6f * time;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

        math::vec4f dir_y = math::dot(m, math::vec4f{0.0f,1.0f,0.0f,1.0f});
        m = math::rotate(m, math::vec3f{dir_y.x, dir_y.y, dir_y.z}, ang_y);

        math::vec4f dir_z = math::dot(m, math::vec4f{0.0f,0.0f,1.0f,1.0f});
        m = math::rotate(m, math::vec3f{dir_z.x, dir_z.y, dir_z.z}, ang_z);

        std::array<GLfloat,2> size = gl::Renderer::FramebufferSizef();
        float ratio = size[0] / size[1];

        math::mat4f proj = math::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        rop.sphere.mvp = math::dot(proj, m);
    }

    /* Update the quad modelviewprojection matrix */
    {
        float ang_x = 0.2f * time;
        float ang_y = 0.2f * time;
        float ang_z = 0.2f * time;

        math::mat4f m = math::mat4f::eye;
        m = math::rotate(m, math::vec3f{0.0f, 0.0f, 1.0f}, ang_z);
        m = math::rotate(m, math::vec3f{0.0f, 1.0f, 0.0f}, ang_y);
        m = math::rotate(m, math::vec3f{1.0f, 0.0f, 0.0f}, ang_x);

        float ratio = (float) rop.fbo.width / rop.fbo.height;
        math::mat4f proj = math::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
        rop.quad.mvp = math::dot(proj, m);
    }
}

/**
 * @brief Render the rop.
 */
void Rop::Render(const Rop &rop)
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

    /* Render into the framebuffer rendertexture */
    {
        /* Bind the framebuffer */
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rop.fbo.id);
        auto viewport = gl::Renderer::Viewport();
        gl::Renderer::Viewport({0, 0, rop.fbo.width, rop.fbo.height});
        gl::Renderer::ClearBuffers(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

        /* Bind the sphere shader */
        glUseProgram(rop.sphere.program);

        /* Set window dimensions. */
        gl::SetUniform(rop.sphere.program, "u_mix", GL_FLOAT,
            &rop.fbo.mix);
        gl::SetUniform(rop.sphere.program, "u_width", GL_FLOAT,
            &rop.fbo.width);
        gl::SetUniform(rop.sphere.program, "u_height", GL_FLOAT,
            &rop.fbo.height);
        gl::SetUniformMatrix(rop.sphere.program, "u_mvp", GL_FLOAT_MAT4, true,
            rop.sphere.mvp.data);

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::SetUniform(rop.sphere.program, "u_texsampler", GL_SAMPLER_2D,
            &texunit);
        gl::ActiveBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            rop.sphere.texture);

        /* Draw the sphere mesh */
        gl::Mesh::Render(rop.sphere.mesh);

        /* Unbind the shader program object. */
        glUseProgram(0);

        /* Unbind the framebuffer */
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        gl::Renderer::Viewport(viewport);
        gl::Renderer::ClearBuffers(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
    }

    /*
     * Render into the window framebuffer
     */
    {
        /* Get window dimensions. */
        std::array<GLfloat,2> sizef = gl::Renderer::FramebufferSizef();

        /* Bind the quad shader */
        glUseProgram(rop.quad.program);
        gl::SetUniform(rop.quad.program, "u_mix", GL_FLOAT, &rop.fbo.mix);
        gl::SetUniform(rop.quad.program, "u_width", GL_FLOAT, &sizef[0]);
        gl::SetUniform(rop.quad.program, "u_height", GL_FLOAT, &sizef[0]);
        gl::SetUniformMatrix(rop.quad.program, "u_mvp", GL_FLOAT_MAT4, true,
            rop.quad.mvp.data);

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::SetUniform(rop.quad.program, "u_texsampler", GL_SAMPLER_2D, &texunit);
        gl::ActiveBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit, rop.fbo.color_texture);

        /* Draw the quad mesh */
        gl::Mesh::Render(rop.quad.mesh);

        /* Unbind the shader program object. */
        glUseProgram(0);
    }
}
