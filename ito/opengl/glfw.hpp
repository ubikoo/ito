/*
 * glfw.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_GLFW_H_
#define ITO_OPENGL_GLFW_H_

#include <array>
#include "base.hpp"

namespace ito {
namespace glfw {

/** ---------------------------------------------------------------------------
 * @brief Initialize the GLFW library and create a GLFW window.
 */
void Init(
    const int width,
    const int height,
    const char *title,
    const int major = 3,
    const int minor = 3,
    const bool offscreen = false);

/** @brief Destroy the GLFWwindow object and terminate the GLFW library. */
void Terminate(void);

/** @brief Is GLFW library initialized? */
bool IsInit(void);

/** @brief Return a const pointer to the GLFWwindow. */
GLFWwindow *Window(void);

/** @brief Return a string with OpenGL information. */
const std::string &InfoString();

/** @brief Return true GLFWwindow should stay open, false otherwise. */
bool IsOpen(void);

/** @brief Set the closed status flag of the GLFWwindow. */
void Close(void);

/** @brief Swap the front and back buffers of the GLFWwindow. */
void SwapBuffers(void);

/** @brief Clear OpenGL color and depth buffers. */
void ClearBuffers(
    GLfloat red,
    GLfloat green,
    GLfloat blue,
    GLfloat alpha,
    GLdouble depth);

/** @brief Get the window viewport position, width and height. */
void GetViewport(std::array<GLint, 4> &viewport);

/** @brief Set the window viewport position, width and height. */
void SetViewport(const std::array<GLint, 4> &viewport);

/** @brief Get the renderer framebuffer size. */
void GetFramebufferSize(std::array<GLint,2> &size);
void GetFramebufferSize(std::array<GLfloat,2> &size);

/** -------------------------------------------------------------------------
 * @brief Event is a union of structs representing different renderer events.
 * Given an Event object, the valid struct is the one that corresponds to the
 * type member variable. Trying to read another struct results iin undefined
 * behaviour.
 */
struct Event {
    /** Event enumerated type. */
    enum : GLenum {
        /* Window events */
        FramebufferSize         = 1 << 0,
        WindowPos               = 1 << 1,
        WindowSize              = 1 << 2,
        WindowClose             = 1 << 3,
        WindowMaximize          = 1 << 4,
        /* Input events */
        Key                     = 1 << 5,
        CursorEnter             = 1 << 6,
        CursorPos               = 1 << 7,
        MouseButton             = 1 << 8,
        MouseScroll             = 1 << 9,
        /* All events */
        All = FramebufferSize
            | WindowPos
            | WindowSize
            | WindowClose
            | WindowMaximize
            | Key
            | CursorEnter
            | CursorPos
            | MouseButton
            | MouseScroll
    };
    GLenum type;

    /** Type assignment operator. */
    GLenum operator=(const GLenum value) { return (type = value); }

    /** Bitwise type assignment AND, OR and XOR operators. */
    GLenum operator&= (const GLenum &value) { return (type &= value); }
    GLenum operator|= (const GLenum &value) { return (type |= value); }
    GLenum operator^= (const GLenum &value) { return (type ^= value); }

    /** Event data structs and corresponding union. */
    struct FramebufferSizeType {
        int width;
        int height;
    };

    struct WindowPosType {
        int xpos;
        int ypos;
    };

    struct WindowSizeType {
        int width;
        int height;
    };

    struct WindowMaximizeType {
        int iconified;
    };

    struct KeyType {
        int code;
        int scancode;
        int action;
        int mods;
    };

    struct CursorEnterType {
        int entered;
    };

    struct CursorPosType {
        double xpos;
        double ypos;
    };

    struct MouseButtonType {
        int button;
        int action;
        int mods;
    };

    struct MouseScrollType {
        double xoffset;
        double yoffset;
    };

    union {
        FramebufferSizeType framebuffersize;
        WindowPosType windowpos;
        WindowSizeType windowsize;
        WindowMaximizeType windowmaximize;
        KeyType key;
        CursorEnterType cursorenter;
        CursorPosType cursorpos;
        MouseButtonType mousebutton;
        MouseScrollType mousescroll;
    };

    /** Constructor/destructor.  */
    explicit Event(const GLenum value) : type(value) {}
    ~Event() = default;

    /** Copy constructor/assignment. */
    Event(const Event &other);
    Event &operator=(const Event &other);
};

/** @brief Event copy constructor. */
inline Event::Event(const Event &other)
{
    /* Copy event type and event data. */
    type = other.type;

    switch (type) {
    case FramebufferSize:
        framebuffersize = other.framebuffersize;
        break;
    case WindowPos:
        windowpos = other.windowpos;
        break;
    case WindowSize:
        windowsize = other.windowsize;
        break;
    case WindowMaximize:
        windowmaximize = other.windowmaximize;
        break;
    case Key:
        key = other.key;
        break;
    case CursorEnter:
        cursorenter = other.cursorenter;
        break;
    case CursorPos:
        cursorpos = other.cursorpos;
        break;
    case MouseButton:
        mousebutton = other.mousebutton;
        break;
    case MouseScroll:
        mousescroll = other.mousescroll;
        break;
    default:
        break;
    }
}

/** @brief Event copy assignment. */
inline Event &Event::operator=(const Event &other)
{
    if (this == &other) {
        return *this;
    }

    /* Copy event type and event data. */
    type = other.type;

    switch (type) {
    case FramebufferSize:
        framebuffersize = other.framebuffersize;
        break;
    case WindowPos:
        windowpos = other.windowpos;
        break;
    case WindowSize:
        windowsize = other.windowsize;
        break;
    case WindowMaximize:
        windowmaximize = other.windowmaximize;
        break;
    case Key:
        key = other.key;
        break;
    case CursorEnter:
        cursorenter = other.cursorenter;
        break;
    case CursorPos:
        cursorpos = other.cursorpos;
        break;
    case MouseButton:
        mousebutton = other.mousebutton;
        break;
    case MouseScroll:
        mousescroll = other.mousescroll;
        break;
    default:
        break;
    }

    return *this;
}

/** @brief Event bitwise unary NOT and binary AND, OR and XOR operators. */
inline GLenum operator~(Event lhs) { return ~(lhs.type); }

inline GLenum operator&(Event lhs, const GLenum &rhs) { return (lhs &= rhs); }
inline GLenum operator|(Event lhs, const GLenum &rhs) { return (lhs |= rhs); }
inline GLenum operator^(Event lhs, const GLenum &rhs) { return (lhs ^= rhs); }

inline GLenum operator&(Event lhs, const Event &rhs) { return (lhs &= rhs.type); }
inline GLenum operator|(Event lhs, const Event &rhs) { return (lhs |= rhs.type); }
inline GLenum operator^(Event lhs, const Event &rhs) { return (lhs ^= rhs.type); }

/** @brief Does the queue have any events to be processed? */
bool HasEvent(void);

/** @brief Poll events until the specified timeout is reached. */
void PollEvent(double timeout);

/** @brief Add an event to the renderer event queue. */
void PushEvent(const Event &event);

/** @brief Get the top event from the renderer event queue. */
Event PopEvent(void);

/** @brief Enable a collection of events in the renderer. */
void EnableEvent(const GLenum mask);

/** @brief Disable a collection of events in the renderer. */
void DisableEvent(const GLenum mask);

} /* glfw */
} /* ito */

#endif /* ITO_OPENGL_GLFW_H_ */
