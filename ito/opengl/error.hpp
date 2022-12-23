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
 */
GLenum GetError(void);

/**
 * @brief Does the OpenGL error queue have an error?
 */
bool HasError(void);

/**
 * @brief Clear the OpenGL error queue of any errors.
 */
void ClearError(void);

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_ERROR_H_ */
