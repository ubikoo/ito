/*
 * error.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_ERROR_H_
#define ITO_OPENGL_ERROR_H_

#include "base.hpp"

namespace ito {
namespace gl {

/**
 * @brief Check the OpenGL error queue.
 * @see https://www.khronos.org/opengl/wiki/Example_Code
 *      https://www.khronos.org/opengl/wiki/OpenGL_Error
 */
GLenum GetError(void)
{
    GLenum error_code = glGetError();
    if (error_code != GL_NO_ERROR) {
        std::string error = {};
        switch (error_code) {
            case GL_INVALID_ENUM:
                error = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                error = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                error = "GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                error = "GL_STACK_OVERFLOW";
                break;
            default:
                error = "UNKNOWN";
                break;
        }

        std::cerr << ito::str::format("OpenGL error: %s\n", error.c_str());
    }
    return error_code;
}

/**
 * @brief Does the OpenGL error queue have an error?
 */
bool HasError(void)
{
    return (GetError() != GL_NO_ERROR);
}

/**
 * @brief Clear the OpenGL error queue of any errors.
 */
void ClearError(void)
{
    while (glGetError() != GL_NO_ERROR);
}

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_ERROR_H_ */
