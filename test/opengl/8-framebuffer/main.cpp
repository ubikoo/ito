/*
 * main.cpp
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

/** ---------------------------------------------------------------------------
 * @brief Constants and globals.
 */
static const int kWidth = 800;
static const int kHeight = 800;
static const char kTitle[] = "Test quadsph";
static const double kTimeout = 0.001;

Rop gRop;

/** ---------------------------------------------------------------------------
 * @brief Handle events.
 */
static void Handle(void)
{
    /* Poll events and handle. */
    gl::Renderer::PollEvent(kTimeout);
    while (gl::Renderer::HasEvent()) {
        gl::Renderer::Event event = gl::Renderer::PopEvent();

        if (event.type == gl::Renderer::Event::FramebufferSize) {
            int w = event.framebuffersize.width;
            int h = event.framebuffersize.height;
            gl::Renderer::Viewport({0, 0, w, h});
        }

        if ((event.type == gl::Renderer::Event::WindowClose) ||
            (event.type == gl::Renderer::Event::Key &&
             event.key.code == GLFW_KEY_ESCAPE)) {
            gl::Renderer::Close();
        }

        /* Handle the input on the rop */
        Rop::Handle(gRop, event);
    }
}

/** ---------------------------------------------------------------------------
 * @brief Update state.
 */
static void Update(void)
{
    /* Update the rop. */
    Rop::Update(gRop);
}

/** ---------------------------------------------------------------------------
 * @brief Draw and swap buffers.
 */
static void Render(void)
{
    gl::Renderer::ClearBuffers(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
    Rop::Render(gRop);
    gl::Renderer::SwapBuffers();
}

/** ---------------------------------------------------------------------------
 * main test client
 */
int main(int argc, char const *argv[])
{
    /* Setup OpenGL context and initialize the GLFW library. */
    gl::Renderer::Init(kWidth, kHeight, kTitle);
    gl::Renderer::EnableEvent(
        gl::Renderer::Event::FramebufferSize |
        gl::Renderer::Event::WindowClose     |
        gl::Renderer::Event::Key);

    /* Create the rop object. */
    gRop = Rop::Create();

    /*
     * Render loop:
     *  handle events
     *  update the state
     *  draw and swap buffers
     */
    while (gl::Renderer::IsOpen()) {
        Handle();
        Update();
        Render();
    }

    /* Create the rop object. */
    Rop::Destroy(gRop);

    exit(EXIT_SUCCESS);
}
