/*
 * vertexarray.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_VERTEXARRAY_H_
#define ITO_OPENGL_VERTEXARRAY_H_

#include "base.hpp"

namespace ito {
namespace gl {

/**
 * @brief Generate a new vertex array object and bind it to the context.
 */
GLuint CreateVertexArray(void);

/**
 * @brief Delete a vertex array object.
 */
void DestroyVertexArray(const GLuint &array);

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_VERTEXARRAY_H_ */
