/*
 * sphere.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ITO_OPENGL_SPHERE_H_
#define TEST_ITO_OPENGL_SPHERE_H_

#include "ito/opengl.hpp"

struct Sphere {
    GLuint program;                 /* shader program object */
    ito::gl::Mesh mesh;             /* sphere mesh image and texture */
    ito::gl::Image image;
    GLuint texture;
    ito::math::mat4f mvp;           /* modelviewprojection */

    void Handle(ito::glfw::Event &event);
    void Update(void);
    void Render(void);

    static Sphere Create(void);
    static void Destroy(Sphere &sphere);
};

#endif /* TEST_ITO_OPENGL_SPHERE_H_ */
