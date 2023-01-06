/*
 * map.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include <string>
#include <vector>
#include "ito/opengl.hpp"
using namespace ito;
#include "iobuffer.hpp"
#include "map.hpp"

/**
 * @brief Map constant parameters.
 */
static const std::string kImageFilename = "../common/monarch_512.png";
static const int kWidth = 1024;
static const int kHeight = 1024;

/**
 * @brief Create a new map.
 * Begin by rendering the image to the map framebuffer.
 * Run the map shader program over a double framebuffer.
 * End by rendering the map framebuffer to the screen.
 */
Map Map::Create()
{
    Map map;

    /*
     * Map begin shader
     */
    {
        /* Build the begin shader program. */
        std::vector<GLuint> shaders{
            gl::CreateShader(GL_VERTEX_SHADER, "data/map-begin.vert"),
            gl::CreateShader(GL_FRAGMENT_SHADER, "data/map-begin.frag")};
        map.begin.program = gl::CreateProgram(shaders);
        gl::DestroyShader(shaders);
        std::cout << gl::GetProgramInfoStr(map.begin.program) << "\n";

        /* Load the 2d-image from the specified filename. */
        gl::Image image = gl::Image::Load(kImageFilename, true);
        map.begin.texture = gl::CreateTexture2d(
            GL_RGBA8,                   /* internal format */
            image.width,                /* texture width */
            image.height,               /* texture height */
            image.format,               /* pixel format */
            GL_UNSIGNED_BYTE,           /* pixel type */
            &image.bitmap[0]);          /* pixel data */

        glBindTexture(GL_TEXTURE_2D, map.begin.texture);
        gl::SetTextureMipmap(GL_TEXTURE_2D);
        gl::SetTextureWrap(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        gl::SetTextureFilter(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        /* Create a mesh over a quad. */
        map.begin.quad = gl::Mesh::Plane(
            map.begin.program,          /* shader program object */
            "quad",                     /* vertex attributes prefix */
            2,                          /* n1 vertices */
            2,                          /* n2 vertices */
            -1.0,                       /* xlo */
             1.0,                       /* xhi */
            -1.0,                       /* ylo */
             1.0);                      /* yhi */
    }

    /*
     * Map run shader
     */
    {
        /* Build the run shader program. */
        std::vector<GLuint> shaders{
            gl::CreateShader(GL_VERTEX_SHADER, "data/map-run.vert"),
            gl::CreateShader(GL_FRAGMENT_SHADER, "data/map-run.frag")};
        map.run.program = gl::CreateProgram(shaders);
        gl::DestroyShader(shaders);
        std::cout << gl::GetProgramInfoStr(map.run.program) << "\n";

        /* Create a mesh over a quad. */
        map.run.quad = gl::Mesh::Plane(
            map.run.program,            /* shader program object */
            "quad",                     /* vertex attributes prefix */
            2,                          /* n1 vertices */
            2,                          /* n2 vertices */
            -1.0,                       /* xlo */
             1.0,                       /* xhi */
            -1.0,                       /* ylo */
             1.0);                      /* yhi */

        /* Create the map input/output framebuffer objects. */
        map.run.read_ix = 0;
        map.run.write_ix = 1;
        map.run.iterations = 0;
        map.run.buffer[0] = IOBuffer::Create(kWidth, kHeight, GL_RGB32F);
        map.run.buffer[1] = IOBuffer::Create(kWidth, kHeight, GL_RGB32F);
    }

    /*
     * Map end shader
     */
    {
        /* Build the run shader program. */
        std::vector<GLuint> shaders{
            gl::CreateShader(GL_VERTEX_SHADER, "data/map-end.vert"),
            gl::CreateShader(GL_FRAGMENT_SHADER, "data/map-end.frag")};
        map.end.program = gl::CreateProgram(shaders);
        gl::DestroyShader(shaders);
        std::cout << gl::GetProgramInfoStr(map.end.program) << "\n";

        /* Create a mesh over a quad. */
        map.end.quad = gl::Mesh::Plane(
            map.end.program,        /* shader program object */
            "quad",                 /* vertex attributes prefix */
            2,                      /* n1 vertices */
            2,                      /* n2 vertices */
            -1.0,                   /* xlo */
             1.0,                   /* xhi */
            -1.0,                   /* ylo */
             1.0);                  /* yhi */
    }

    return map;
}

/**
 * @brief Destroy the map.
 */
void Map::Destroy(Map &map)
{
    /* Map end shader */
    gl::Mesh::Destroy(map.end.quad);
    gl::DestroyProgram(map.end.program);

    /* Map run shader */
    IOBuffer::Destroy(map.run.buffer[0]);
    IOBuffer::Destroy(map.run.buffer[1]);
    gl::Mesh::Destroy(map.run.quad);
    gl::DestroyProgram(map.run.program);

    /* Map begin shader */
    gl::Mesh::Destroy(map.begin.quad);
    gl::DestroyTexture(map.begin.texture);
    gl::DestroyProgram(map.begin.program);
}

/**
 * @brief Handle the event in the map.
 */
void Map::Handle(glfw::Event &event)
{
    using glfw::Event;

    if (event.type == Event::Key && event.key.code == GLFW_KEY_UP) {
        run.iterations++;
        std::cout << "run.iterations " << run.iterations << "\n";
    }

    if (event.type == Event::Key && event.key.code == GLFW_KEY_DOWN) {
        if (run.iterations > 0) {
            run.iterations--;
        }
        std::cout << "run.iterations " << run.iterations << "\n";
    }
}

/**
 * @brief Update the map.
 */
void Map::Update(void)
{}

/**
 * @brief Render the map.
 */
void Map::Render(void)
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

    /*
     * Map begin shader.
     */
    {
        /* Swap read/write buffer indices. */
        std::swap(run.read_ix, run.write_ix);

        /* Bind the framebuffer for writing */
        run.buffer[run.write_ix].bind();
        std::array<GLint, 4> viewport;
        glfw::GetViewport(viewport);
        glfw::SetViewport({0, 0, kWidth, kHeight});
        glfw::ClearBuffers(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

        /* Bind the begin shader */
        glUseProgram(begin.program);

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::SetUniform(begin.program, "u_texsampler", GL_SAMPLER_2D, &texunit);
        gl::ActiveBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            begin.texture);

        /* Draw the quad mesh */
        gl::Mesh::Render(begin.quad);

        /* Unbind the shader program object. */
        glUseProgram(0);

        /* Unbind the framebuffer */
        run.buffer[run.write_ix].unbind();
        glfw::SetViewport(viewport);
    }

    /*
     * Map run shader.
     */
    for (size_t iter = 0; iter < run.iterations; ++iter) {
        /* Swap read/write buffer indices. */
        std::swap(run.read_ix, run.write_ix);

        /* Bind the framebuffer for writing */
        run.buffer[run.write_ix].bind();
        std::array<GLint, 4> viewport;
        glfw::GetViewport(viewport);
        glfw::SetViewport({0, 0, kWidth, kHeight});
        glfw::ClearBuffers(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);

        /* Bind the begin shader */
        glUseProgram(run.program);

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::SetUniform(run.program, "u_texsampler",  GL_SAMPLER_2D, &texunit);
        gl::ActiveBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            run.buffer[run.read_ix].color_texture);

        /* Draw the quad mesh */
        gl::Mesh::Render(begin.quad);

        /* Unbind the shader program object. */
        glUseProgram(0);

        /* Unbind the framebuffer */
        run.buffer[run.write_ix].unbind();
        glfw::SetViewport(viewport);
    }

    /*
     * Map end shader.
     */
    {
        /* Swap read/write buffer indices. */
        std::swap(run.read_ix, run.write_ix);

        /* Bind the begin shader */
        glUseProgram(end.program);

        /* Set the sampler uniform with the texture unit and bind the texture */
        GLenum texunit = 0;
        gl::SetUniform(end.program, "u_texsampler", GL_SAMPLER_2D, &texunit);
        gl::ActiveBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit,
            run.buffer[run.read_ix].color_texture);

        /* Draw the quad mesh */
        gl::Mesh::Render(begin.quad);

        /* Unbind the shader program object. */
        glUseProgram(0);
    }
}
