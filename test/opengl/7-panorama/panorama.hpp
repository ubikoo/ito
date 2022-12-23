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
    GLuint program;                 /* shader program object */
    ito::gl::Mesh mesh;             /* panorama mesh image and texture */
    ito::gl::Image image;
    GLuint texture;
    Camera camera;                  /* panorama camera and projection matrix */
    ito::math::mat4f mvp;

    static Panorama Create(void);
    static void Destroy(Panorama &panorama);
    static void Handle(Panorama &panorama, ito::gl::Renderer::Event &event);
    static void Update(Panorama &panorama);
    static void Render(const Panorama &panorama);
};

#endif /* TEST_ITO_OPENGL_PANORAMA_H_ */
