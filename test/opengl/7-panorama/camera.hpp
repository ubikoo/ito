/**
 * camera.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_ITO_OPENGL_CAMERA_H_
#define TEST_ITO_OPENGL_CAMERA_H_

#include "ito/opengl.hpp"

struct Camera {
    float theta;                /* camera polar angle */
    float phi;                  /* camera azimuth angle */
    float theta_step;           /* polar angle increment value */
    float phi_step;             /* azimuth angle increment value */

    void rotate_left(void);
    void rotate_right(void);

    void rotate_up(void);
    void rotate_down(void);

    ito::math::mat4f lookat(void);
};

#endif /* TEST_ITO_OPENGL_CAMERA_H_ */
