/*
 * triangle.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ITO_OPENGL_TRIANGLE_H_
#define TEST_ITO_OPENGL_TRIANGLE_H_

#include "ito/opengl.hpp"

struct Triangle {
    GLuint program;       /* shader program object */
    GLuint vao;           /* vertex array object */
    GLuint vbo;           /* vertex buffer object */

    void Handle(ito::glfw::Event &event);
    void Update(void);
    void Render(void);

    static Triangle Create(void);
    static void Destroy(Triangle &triangle);
};

#endif /* TEST_ITO_OPENGL_TRIANGLE_H_ */
