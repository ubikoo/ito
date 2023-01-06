/*
 * drawable.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ITO_OPENGL_DRAWABLE_H_
#define TEST_ITO_OPENGL_DRAWABLE_H_

#include "ito/opengl.hpp"

struct Drawable {
    /* Sphere drawable */
    struct {
        GLuint program;             /* shader program object */
        GLuint texture;             /* sphere texture */
        ito::gl::Mesh mesh;         /* image and texture */
        ito::math::mat4f mvp;       /* sphere mvp matrix */
    } sphere;

    /* Quad drawable */
    struct {
        GLuint program;             /* shader program object */
        ito::gl::Mesh mesh;         /* image and texture */
        ito::math::mat4f mvp;       /* quad mvp matrix */
    } quad;

    /* Fbo data */
    struct {
        GLsizei width;              /* fbo width */
        GLsizei height;             /* fbo height */
        GLuint color_texture;       /* fbo color attachment */
        GLuint depth_texture;       /* fbo depth attachment */
        GLuint id;                  /* framebuffer object id */
    } fbo;

    void Handle(ito::glfw::Event &event);
    void Update(void);
    void Render(void);

    static Drawable Create(void);
    static void Destroy(Drawable &drawable);
};

#endif /* TEST_ITO_OPENGL_DRAWABLE_H_ */
