/*
 * imageformat.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_IMAGEFORMAT_H_
#define ITO_OPENGL_IMAGEFORMAT_H_

#include "base.hpp"

namespace ito {
namespace gl {

/**
 * @brief ImageFormat maintains a map of pairs, each comprising a pixel base
 * format and pixel data type, and keyed by the corresponding internal format.
 */
namespace ImageFormat {
    /**
     * @brief Does the map contain the OpenGL image format?
     */
    bool Contains(const GLint internalformat);

    /**
     * @brief Return the pixel base format of the OpenGL image format.
     */
    GLenum BaseFormat(const GLint internalformat);

    /**
     * @brief Return the pixel data type of the OpenGL image format.
     */
    GLenum DataType(const GLint internalformat);

    /**
     * @brief Return the size in bytes of the OpenGL image format.
     */
    GLsizei DataSize(const GLint internalformat);

    /**
     * @brief Return the number of components of the OpenGL image format.
     */
    GLsizei Components(const GLint internalformat);
} /* ImageFormat */

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_IMAGEFORMAT_H_ */
