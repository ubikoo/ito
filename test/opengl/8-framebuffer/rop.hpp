/*
 * rop.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ITO_OPENGL_ROP_H_
#define TEST_ITO_OPENGL_ROP_H_

#include "ito/opengl.hpp"

struct Rop {
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
        float mix;                  /* mixing parameter */
        GLsizei width;              /* fbo width */
        GLsizei height;             /* fbo height */
        GLuint color_texture;       /* fbo color attachment */
        GLuint depth_texture;       /* fbo depth attachment */
        GLuint id;                  /* framebuffer object id */
    } fbo;

    static Rop Create(void);
    static void Destroy(Rop &rop);
    static void Handle(Rop &rop, ito::gl::Renderer::Event &event);
    static void Update(Rop &rop);
    static void Render(const Rop &rop);
};

#endif /* TEST_ITO_OPENGL_ROP_H_ */
