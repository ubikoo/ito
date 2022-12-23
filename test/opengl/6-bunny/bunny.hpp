/*
 * bunny.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ITO_OPENGL_BUNNY_H_
#define TEST_ITO_OPENGL_BUNNY_H_

#include <vector>
#include "ito/opengl.hpp"

struct Bunny {
    GLuint program;                         /* shader program object */
    std::vector<ito::gl::Mesh> model;       /* bunny model */
    ito::math::mat4f mvp;                   /* modelviewprojection */

    static Bunny Create(void);
    static void Destroy(Bunny &bunny);
    static void Handle(Bunny &bunny, ito::gl::Renderer::Event &event);
    static void Update(Bunny &bunny);
    static void Render(const Bunny &bunny);
};

#endif /* TEST_ITO_OPENGL_BUNNY_H_ */
