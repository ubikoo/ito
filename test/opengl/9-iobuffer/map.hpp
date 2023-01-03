/*
 * map.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ITO_OPENGL_MAP_H_
#define TEST_ITO_OPENGL_MAP_H_

#include <array>
#include "ito/opengl.hpp"
#include "iobuffer.hpp"

struct Map {
    /* Map begin shader. */
    struct {
        GLuint program;
        GLuint texture;
        ito::gl::Mesh quad;
    } begin;

    /* Map run shader. */
    struct {
        GLuint program;
        ito::gl::Mesh quad;
        size_t read_ix;
        size_t write_ix;
        size_t iterations;
        std::array<IOBuffer,2> buffer;
    } run;

    /* Map end shader. */
    struct {
        GLuint program;
        ito::gl::Mesh quad;
    } end;

    void Handle(ito::gl::Renderer::Event &event);
    void Update(void);
    void Render(void);

    static Map Create(void);
    static void Destroy(Map &map);
};

#endif /* TEST_ITO_OPENGL_MAP_H_ */
