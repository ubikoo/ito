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
#include "drawable.hpp"

using namespace ito;

/** ---------------------------------------------------------------------------
 * @brief Constants and globals.
 */
static const int kWidth = 800;
static const int kHeight = 800;
static const char kTitle[] = "Test quadsph";
static const double kTimeout = 0.001;

Drawable gDrawable;

/** ---------------------------------------------------------------------------
 * @brief Handle events.
 */
static void Handle(void)
{
    /* Poll events and handle. */
    glfw::PollEvent(kTimeout);
    while (glfw::HasEvent()) {
        glfw::Event event = glfw::PopEvent();

        if (event.type == glfw::Event::FramebufferSize) {
            int w = event.framebuffersize.width;
            int h = event.framebuffersize.height;
            glfw::SetViewport({0, 0, w, h});
        }

        if ((event.type == glfw::Event::WindowClose) ||
            (event.type == glfw::Event::Key &&
             event.key.code == GLFW_KEY_ESCAPE)) {
            glfw::Close();
        }

        gDrawable.Handle(event);
    }
}

/** ---------------------------------------------------------------------------
 * @brief Update state.
 */
static void Update(void)
{
    gDrawable.Update();
}

/** ---------------------------------------------------------------------------
 * @brief Draw and swap buffers.
 */
static void Render(void)
{
    glfw::ClearBuffers(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
    gDrawable.Render();
    glfw::SwapBuffers();
}

/** ---------------------------------------------------------------------------
 * main test client
 */
int main(int argc, char const *argv[])
{
    /* Initalize GLFW library and create OpenGL context. */
    glfw::Init(kWidth, kHeight, kTitle);
    glfw::EnableEvent(
        glfw::Event::FramebufferSize |
        glfw::Event::WindowClose     |
        glfw::Event::Key);

    /* Create the drawable object. */
    gDrawable = Drawable::Create();

    /* Render loop: handle events, update state, and render. */
    while (glfw::IsOpen()) {
        Handle();
        Update();
        Render();
    }

    /* Destroy the drawable object. */
    Drawable::Destroy(gDrawable);

    /* Terminate GLFW library and destroy OpenGL context. */
    glfw::Terminate();

    exit(EXIT_SUCCESS);
}
