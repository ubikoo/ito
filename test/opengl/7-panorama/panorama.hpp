/*
 * panorama.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ITO_OPENGL_PANORAMA_H_
#define TEST_ITO_OPENGL_PANORAMA_H_

#include "ito/opengl.hpp"
#include "camera.hpp"

struct Panorama {
    GLuint program;             /* shader program object */
    ito::gl::Image image;       /* panoranam image, mesh and texture */
    ito::gl::Mesh mesh;
    GLuint texture;
    Camera camera;              /* panorama camera and projection matrix */
    ito::math::mat4f mvp;

    void Handle(ito::glfw::Event &event);
    void Update(void);
    void Render(void);

    static Panorama Create(void);
    static void Destroy(Panorama &panorama);
};

#endif /* TEST_ITO_OPENGL_PANORAMA_H_ */
