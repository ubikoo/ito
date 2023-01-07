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

/** ---------------------------------------------------------------------------
 * @brief Constants and globals.
 */
static const int kWidth = 800;
static const int kHeight = 800;
static const char kTitle[] = "Test GLFW";
static const double kTimeout = 0.001;

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

        if (event.type == glfw::Event::FramebufferSize) {
            std::cout << ito::str::to_string(
                "FramebufferSize ",
                event.framebuffersize.width, " ",
                event.framebuffersize.height, "\n");
            int w = event.framebuffersize.width;
            int h = event.framebuffersize.height;
            glfw::SetViewport({0, 0, w, h});
        }

        if (event.type == glfw::Event::WindowPos) {
            std::cout << ito::str::to_string(
                "WindowPos ",
                event.windowpos.xpos, " ",
                event.windowpos.ypos, "\n");
        }

        if (event.type == glfw::Event::WindowSize) {
            std::cout << ito::str::to_string(
                "WindowSize ",
                event.windowsize.width, " ",
                event.windowsize.height, "\n");
        }

        if (event.type == glfw::Event::WindowClose) {
            std::cout << ito::str::to_string(
                "WindowClose\n");
        }

        if (event.type == glfw::Event::WindowMaximize) {
            std::cout << ito::str::to_string(
                "WindowMaximize ",
                event.windowmaximize.iconified, "\n");
        }

        if (event.type == glfw::Event::Key) {
            std::cout << ito::str::to_string(
                "Key ",
                event.key.code, " ",
                event.key.scancode, " ",
                event.key.action, " ",
                event.key.mods, "\n");
        }

        if (event.type == glfw::Event::CursorEnter) {
            std::cout << ito::str::to_string(
                "CursorEnter ",
                event.cursorenter.entered, "\n");
        }

        if (event.type == glfw::Event::CursorPos) {
            std::cout << ito::str::to_string(
                "CursorPos ",
                event.cursorpos.xpos, " ",
                event.cursorpos.ypos, "\n");
        }

        if (event.type == glfw::Event::MouseButton) {
            std::cout << ito::str::to_string(
                "MouseButton ",
                event.mousebutton.button, " ",
                event.mousebutton.action, " ",
                event.mousebutton.mods, "\n");
        }

        if (event.type == glfw::Event::MouseScroll) {
            std::cout << ito::str::to_string(
                "MouseScroll ",
                event.mousescroll.xoffset, " ",
                event.mousescroll.yoffset, "\n");
        }
    }
}

/** ---------------------------------------------------------------------------
 * @brief Update state.
 */
static void Update(void)
{}

/** ---------------------------------------------------------------------------
 * @brief Draw and swap buffers.
 */
static void Render(void)
{
    glfw::ClearBuffers(0.5f, 0.5f, 0.5f, 1.0f, 1.0f);
    glfw::SwapBuffers();
}

/** ---------------------------------------------------------------------------
 * main test client
 */
int main(int argc, char const *argv[])
{
    /* Initalize GLFW library and create OpenGL context. */
    glfw::Init(kWidth, kHeight, kTitle);
    glfw::EnableEvent(glfw::Event::All);

    /* Render loop: handle events, update state, and render. */
    while (glfw::IsOpen()) {
        Handle();
        Update();
        Render();
    }

    /* Terminate GLFW library and destroy OpenGL context. */
    glfw::Terminate();

    exit(EXIT_SUCCESS);
}
