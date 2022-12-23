/*
 * quad.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ITO_OPENGL_QUAD_H_
#define TEST_ITO_OPENGL_QUAD_H_

#include "ito/opengl.hpp"

struct Quad {
    GLuint program;         /* shader program object */
    GLuint vao;             /* vertex array object */
    GLuint vbo;             /* vertex buffer object */
    GLuint ebo;             /* element buffer object */
    ito::math::mat4f mvp;   /* modelviewprojection */

    static Quad Create(void);
    static void Destroy(Quad &quad);
    static void Handle(Quad &quad, ito::gl::Renderer::Event &event);
    static void Update(Quad &quad);
    static void Render(const Quad &quad);
};

#endif /* TEST_ITO_OPENGL_QUAD_H_ */
