/*
 * glfw.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include <queue>
#include "glfw.hpp"

/** ---------------------------------------------------------------------------
 * @brief Interface to the GLFW library and associated GL context. It maintains
 * a GLFWwindow object and its queue of events. The GLFW library is initialized
 * when GLFWwindow is created and terminated when GLFWwindow is destroyed.
 *
 * @see https://stackoverflow.com/questions/29617370
 *      https://stackoverflow.com/questions/35683334
 *      https://stackoverflow.com/questions/7676971
 *      https://discourse.glfw.org/t/what-is-a-possible-use-of-glfwgetwindowuserpointer/1294
 */

namespace ito {
namespace glfw {

static GLFWwindow *gWindow = nullptr;
static std::queue<Event> gEventQueue;
static int gWidth = 0;
static int gHeight = 0;
static std::string gInfoString;

/** ---------------------------------------------------------------------------
 * @brief Error callback function:
 *  glfwSetErrorCallback(GLFWerrorfun cbfun)
 *  typedef void(* GLFWerrorfun)(int, const char *);
 */
static void ErrorCallback(int code, const char *desc)
{
    std::cerr << ito::str::format("GLFW error code %d: %s\n", code, desc);
}

/**
 * @brief Renderer framebuffer resize callback function:
 *  glfwSetFramebufferSizeCallback(GLFWwindow *window,
 *      GLFWframebuffersizefun cbfun)
 *  typedef void(* GLFWframebuffersizefun)(GLFWwindow *, int, int)
 */
static void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    Event event(Event::FramebufferSize);
    event.framebuffersize = {width, height};
    gEventQueue.push(event);
}

/**
 * @brief Renderer position callback function:
 *  glfwSetWindowPosCallback(GLFWwindow *window, GLFWwindowposfun cbfun)
 *  typedef void(* GLFWwindowposfun)(GLFWwindow *, int, int)
 */
static void WindowPosCallback(GLFWwindow *window, int xpos, int ypos)
{
    Event event(Event::WindowPos);
    event.windowpos = {xpos, ypos};
    gEventQueue.push(event);
}

/**
 * @brief Renderer resize callback function:
 *  glfwSetWindowSizeCallback(GLFWwindow *window, GLFWwindowsizefun cbfun)
 *  typedef void(* GLFWwindowsizefun)(GLFWwindow *, int, int)
 */
static void WindowSizeCallback(GLFWwindow *window, int width, int height)
{
    Event event(Event::WindowSize);
    event.windowsize = {width, height};
    gEventQueue.push(event);
}

/**
 * @brief Renderer close callback function:
 *  glfwSetWindowCloseCallback(GLFWwindow *window, GLFWwindowclosefun cbfun)
 *  typedef void(* GLFWwindowclosefun)(GLFWwindow *)
 */
static void WindowCloseCallback(GLFWwindow *window)
{
    Event event(Event::WindowClose);
    gEventQueue.push(event);
}

/**
 * @brief Renderer maximize callback function:
 *  glfwSetWindowMaximizeCallback(GLFWwindow *window,
 *      GLFWwindowmaximizefun cbfun)
 *  typedef void(* GLFWwindowmaximizefun)(GLFWwindow *, int)
 */
static void WindowMaximizeCallback(GLFWwindow *window, int iconified)
{
    Event event(Event::WindowMaximize);
    event.windowmaximize = {iconified};
    gEventQueue.push(event);
}

/**
 * @brief Key callback function:
 *  glfwSetKeyCallback(GLFWwindow *window, GLFWkeyfun cbfun)
 *  typedef void(* GLFWkeyfun)(GLFWwindow *, int, int, int, int)
 */
static void KeyCallback(GLFWwindow *window,
    int code, int scancode, int action, int mods)
{
    Event event(Event::Key);
    event.key = {code, scancode, action, mods};
    gEventQueue.push(event);
}

/**
 * @brief Cursor enter callback function:
 *  glfwSetCursorEnterCallback(GLFWwindow *window, GLFWcursorenterfun cbfun)
 *  typedef void(* GLFWcursorenterfun)(GLFWwindow *, int)
 */
static void CursorEnterCallback(GLFWwindow *window, int entered)
{
    Event event(Event::CursorEnter);
    event.cursorenter = {entered};
    gEventQueue.push(event);
}

/**
 * @brief Cursor position callback function:
 *  glfwSetCursorPosCallback(GLFWwindow *window, GLFWcursorposfun cbfun)
 *  typedef void(* GLFWcursorposfun)(GLFWwindow *, double, double)
 */
static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    Event event(Event::CursorPos);
    event.cursorpos = {xpos, ypos};
    gEventQueue.push(event);
}

/**
 * @brief Mouse button callback function:
 *  glfwSetMouseButtonCallback(GLFWwindow *window, GLFWmousebuttonfun cbfun)
 *  typedef void(* GLFWmousebuttonfun)(GLFWwindow *, int, int, int)
 */
static void MouseButtonCallback(GLFWwindow *window,
    int button, int action, int mods)
{
    Event event(Event::MouseButton);
    event.mousebutton = {button, action, mods};
    gEventQueue.push(event);
}

/**
 * @brief Mouse scroll callback function:
 *  glfwSetScrollCallback(GLFWwindow *window, GLFWscrollfun cbfun)
 *  typedef void(* GLFWscrollfun)(GLFWwindow *, double, double)
 */
static void MouseScrollCallback(GLFWwindow *window,
    double xoffset, double yoffset)
{
    Event event(Event::MouseScroll);
    event.mousescroll = {xoffset, yoffset};
    gEventQueue.push(event);
}

/** ---------------------------------------------------------------------------
 * @brief Initialize the GLFW library and create a GLFW window.
 */
void Init(
    const int width,
    const int height,
    const char *title,
    const int major,
    const int minor,
    const bool offscreen)
{
    ito_assert(!IsInit(), "GLFW library already initialized");
    ito_assert(width > 0 && height > 0, "invalid window dimensions");
    ito_assert(title != nullptr, "invalid window title");
    ito_assert(major >= 3, "client API major version number < 3");
    ito_assert(minor >= 3, "client API minor version number < 3");

    /*
     * Initialize the GLFW library. If offscreen is enabled, create a context
     * with hidden windows using the GLFW_VISIBLE window creation hint.
     * macOS: The first time a window is created the menu bar is created.
     * Menu bar creation can be disabled with the GLFW_COCOA_MENUBAR init hint.
     */
    glfwSetErrorCallback(ErrorCallback);
#ifdef __APPLE__
    if (offscreen) {
        glfwInitHint(GLFW_COCOA_MENUBAR, GLFW_FALSE);
    }
#endif
    if (glfwInit() != GLFW_TRUE) {
        ito_throw("failed to initialise GLFW library");
    }

    /*
     * Set OpenGL context and renderer related hints.
     * macOS: Only forward compatible core profile contexts for GL versions 3.2
     * and later are supported. Set context hints GLFW_OPENGL_FORWARD_COMPAT and
     * GLFW_OPENGL_PROFILE before creation. A forward-compatible context is one
     * where all functionality deprecated in the requested version is removed.
     */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    if (offscreen) {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }

    /*
     * Create a new GLFWwindow object.
     */
    GLFWmonitor *monitor = nullptr;     /* run in windowed mode */
    GLFWwindow *share = nullptr;        /* do not share the OpenGL context */
    gWindow = glfwCreateWindow(width, height, title, monitor, share);
    if (gWindow == nullptr) {
        glfwTerminate();
        ito_throw("failed to create GLFWwindow");
    }

    /*
     * Make current the OpenGL context of the newly created window and load the
     * OpenGL function pointers using the context provided by the GLFW loader
     * function. The watertight method is to call gladLoadGLLoader after each
     * and every context change.
     */
    glfwMakeContextCurrent(gWindow);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        glfwTerminate();
        ito_throw("failed to initialise glad");
    }

    /*
     * Set the buffer swap interval to a single monitor refresh between each
     * buffer swap to synchronize buffer swap with the monitor refresh rate.
     */
    glfwSwapInterval(1);

    /*
     * Set OpenGL viewport.
     */
    glfwGetFramebufferSize(gWindow, &gWidth, &gHeight);
    glViewport(0, 0, gWidth, gHeight);

    /*
     * Store a info string with the following format:
     *  the version of GLFW,
     *  the name of the renderer system API,
     *  the name of the context creation API,
     *  any additional options or APIs.
     */
    gInfoString = ito::str::format(
        "GLFW version: %s\nOpenGL Renderer: %s\nOpenGL Version: %s\n",
        glfwGetVersionString(),
        glGetString(GL_RENDERER),
        glGetString(GL_VERSION));
}

/**
 * @brief Destroy the GLFWwindow object and terminate the GLFW library.
 */
void Terminate(void)
{
    ito_assert(IsInit(), "GLFW library is not initialized");

    glfwDestroyWindow(gWindow);
    glfwTerminate();

    gWindow = nullptr;
    gWidth = 0;
    gHeight = 0;
    gInfoString = {};
}

/**
 * @brief Is GLFW library initialized?
 */
bool IsInit(void)
{
    return gWindow != nullptr;
}

/**
 * @brief Return a pointer to the GLFWwindow object.
 */
GLFWwindow *Window(void)
{
    return gWindow;
}

/**
 * @brief Return a string with OpenGL information.
 */
const std::string &InfoString()
{
    return gInfoString;
}

/**
 * @brief Return true GLFWwindow should stay open, false otherwise.
 */
bool IsOpen(void)
{
    return (glfwWindowShouldClose(gWindow) == GLFW_FALSE);
}

/**
 * @brief Set the closed status flag of the GLFWwindow.
 */
void Close(void)
{
    glfwSetWindowShouldClose(gWindow, GLFW_TRUE);
}

/**
 * @brief Swap the front and back buffers of the GLFWwindow.
 */
void SwapBuffers(void)
{
    glfwSwapBuffers(gWindow);
}

/**
 * @brief Clear OpenGL color and depth buffers.
 */
void ClearBuffers(
    GLfloat red,
    GLfloat green,
    GLfloat blue,
    GLfloat alpha,
    GLdouble depth)
{
    glClearColor(red, green, blue, alpha);
    glClearDepth(depth);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Get the window viewport position, width and height.
 */
void GetViewport(std::array<GLint, 4> &viewport)
{
    glGetIntegerv(GL_VIEWPORT, viewport.data());
}

/**
 * @brief Set the window viewport position, width and height.
 * @param viewport: array specifying the elements (x, y, w, h).
 * (x, y) specify the lower left corner of the viewport rectangle.
 * (w, h) specify the viewport rectangle size.
 */
void SetViewport(const std::array<GLint, 4> &viewport)
{
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

/**
 * @brief Get the renderer framebuffer size.
 */
void GetFramebufferSize(std::array<GLint,2> &size)
{
    glfwGetFramebufferSize(gWindow, &size[0], &size[1]);
}

void GetFramebufferSize(std::array<GLfloat,2> &size)
{
    int width, height;
    glfwGetFramebufferSize(gWindow, &width, &height);
    size[0] = width;
    size[1] = height;
}

/** ---------------------------------------------------------------------------
 * @brief Does the queue have any events to be processed?
 */
bool HasEvent(void)
{
    return !gEventQueue.empty();
}

/**
 * @brief Poll events until the specified timeout is reached.
 */
void PollEvent(double timeout)
{
    glfwWaitEventsTimeout(std::max(0.0, timeout));
}

/**
 * @brief Add an event to the renderer event queue.
 */
void PushEvent(const Event &event)
{
    ito_assert(event.type & Event::All, "invalid event type");
    gEventQueue.push(event);
}

/**
 * @brief Get the top event from the renderer event queue.
 */
Event PopEvent(void)
{
    ito_assert(HasEvent(), "empty event queue");
    Event top = gEventQueue.front();
    gEventQueue.pop();
    return top;
}

/**
 * @brief Define the event callback functions. For each event enabled in the
 * GLFWwindow, there is an associated static callback function. The callback
 * is responsible to store the event data in the renderer event* queue.
 * There are two types of events recognised here:
 *  - window events(position, size, etc)
 *  - input events(key, cursor movement, mouse buttons, etc)
 *
 * @par Event processing is normally done each frame after buffer swapping.
 * This step is performed even in the absence of windows, in order to receive
 * monitor connection events.
 *
 * @par Define GLFW library version macros
 *  GlfwVersionHash     Hash version macro creator
 *  GlfwVersionCheck    Check GLFW version minimum requirement
 *  GlfwVersionError    GLFW version error
 */
#ifndef GlfwVersionHash
#define GlfwVersionHash(major, minor) (((major) << 8) | (minor))
#endif

#ifndef GlfwVersionCheck
#define GlfwVersionCheck(major, minor) \
        GlfwVersionHash(GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR) >= \
        GlfwVersionHash((major), (minor))
#endif

#ifndef GlfwVersionError
#define GlfwVersionError(major, minor) \
do { \
    std::cout << core::str_format( \
        "need GLFW version >= %d.%d: %s\n", \
       (major), (minor), glfwGetVersionString()); \
} while(0)
#endif

/**
 * @brief Enable a collection of events in the renderer.
 * @param mask bitwise OR of event types to be enabled.
 */
void EnableEvent(const GLenum mask)
{
    ito_assert(mask & Event::All, "invalid event type");

    /* Set FramebufferSize callback. */
    if (mask & Event::FramebufferSize) {
#if GlfwVersionCheck(3,0)
        glfwSetFramebufferSizeCallback(gWindow, FramebufferSizeCallback);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Set WindowPos callback. */
    if (mask & Event::WindowPos) {
#if GlfwVersionCheck(3,0)
        glfwSetWindowPosCallback(gWindow, WindowPosCallback);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Set WindowSize callback. */
    if (mask & Event::WindowSize) {
#if GlfwVersionCheck(3,0)
        glfwSetWindowSizeCallback(gWindow, WindowSizeCallback);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Set WindowClose callback. */
    if (mask & Event::WindowClose) {
#if GlfwVersionCheck(3,0)
        glfwSetWindowCloseCallback(gWindow, WindowCloseCallback);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Set WindowMaximize callback. */
    if (mask & Event::WindowMaximize) {
#if GlfwVersionCheck(3,3)
        glfwSetWindowMaximizeCallback(gWindow, WindowMaximizeCallback);
#else
        GlfwVersionError(3,3);
#endif
    }

    /* Set Key callback. */
    if (mask & Event::Key) {
#if GlfwVersionCheck(3,0)
        glfwSetKeyCallback(gWindow, KeyCallback);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Set CursorEnter callback. */
    if (mask & Event::CursorEnter) {
#if GlfwVersionCheck(3,0)
        glfwSetCursorEnterCallback(gWindow, CursorEnterCallback);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Set CursorPos callback. */
    if (mask & Event::CursorPos) {
#if GlfwVersionCheck(3,0)
        glfwSetCursorPosCallback(gWindow, CursorPosCallback);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Set MouseButton callback. */
    if (mask & Event::MouseButton) {
#if GlfwVersionCheck(3,0)
        glfwSetMouseButtonCallback(gWindow, MouseButtonCallback);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Set MouseScroll callback. */
    if (mask & Event::MouseScroll) {
#if GlfwVersionCheck(3,0)
        glfwSetScrollCallback(gWindow, MouseScrollCallback);
#else
        GlfwVersionError(3,0);
#endif
    }
}

/**
 * @brief Disable a collection of events in the renderer.
 * @param mask bitwise OR of event types to be disabled.
 */
void DisableEvent(const GLenum mask)
{
    ito_assert(mask & Event::All, "invalid event type");

    /* Unset FramebufferSize callback. */
    if (mask & Event::FramebufferSize) {
#if GlfwVersionCheck(3,0)
        glfwSetFramebufferSizeCallback(gWindow, nullptr);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Unset WindowPos callback. */
    if (mask & Event::WindowPos) {
#if GlfwVersionCheck(3,0)
        glfwSetWindowPosCallback(gWindow, nullptr);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Unset WindowSize callback. */
    if (mask & Event::WindowSize) {
#if GlfwVersionCheck(3,0)
        glfwSetWindowSizeCallback(gWindow, nullptr);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Unset WindowClose callback. */
    if (mask & Event::WindowClose) {
#if GlfwVersionCheck(3,0)
        glfwSetWindowCloseCallback(gWindow, nullptr);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Unset WindowMaximize callback. */
    if (mask & Event::WindowMaximize) {
#if GlfwVersionCheck(3,3)
        glfwSetWindowMaximizeCallback(gWindow, nullptr);
#else
        GlfwVersionError(3,3);
#endif
    }

    /* Unset Key callback. */
    if (mask & Event::Key) {
#if GlfwVersionCheck(3,0)
        glfwSetKeyCallback(gWindow, nullptr);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Unset CursorEnter callback. */
    if (mask & Event::CursorEnter) {
#if GlfwVersionCheck(3,0)
        glfwSetCursorEnterCallback(gWindow, nullptr);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Unset CursorPos callback. */
    if (mask & Event::CursorPos) {
#if GlfwVersionCheck(3,0)
        glfwSetCursorPosCallback(gWindow, nullptr);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Unset MouseButton callback. */
    if (mask & Event::MouseButton) {
#if GlfwVersionCheck(3,0)
        glfwSetMouseButtonCallback(gWindow, nullptr);
#else
        GlfwVersionError(3,0);
#endif
    }

    /* Unset MouseScroll callback. */
    if (mask & Event::MouseScroll) {
#if GlfwVersionCheck(3,0)
        glfwSetScrollCallback(gWindow, nullptr);
#else
        GlfwVersionError(3,0);
#endif
    }
}

/**
 * @brief Undefine GLFW library version macros
 *  GlfwVersionHash     Hash version macro creator
 *  GlfwVersionCheck    Check GLFW version minimum requirement
 *  GlfwVersionError    GLFW version error
 */
#ifdef GlfwVersionHash
#undef GlfwVersionHash
#endif

#ifdef GlfwVersionCheck
#undef GlfwVersionCheck
#endif

#ifdef GlfwVersionError
#undef GlfwVersionError
#endif

} /* glfw */
} /* ito */
