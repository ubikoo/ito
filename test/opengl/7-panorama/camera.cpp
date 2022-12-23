/**
 * camera.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "ito/opengl.hpp"
using namespace ito;
#include "camera.hpp"

/**
 * @brief Rotate camera
 */
void Camera::rotate_left(void)
{
    phi += phi_step;
}

void Camera::rotate_right(void)
{
    phi -= phi_step;
}

void Camera::rotate_up(void)
{
    theta += theta_step;
}

void Camera::rotate_down(void)
{
    theta -= theta_step;
}

/**
 * @brief Return the camera view matrix.
 */
math::mat4f Camera::lookat(void)
{
    /* The camera is located at the origin by default. */
    math::vec3f eye = {0.0f, 0.0f, 0.0f};

    /* And is looking in the positive x-direction. */
    math::vec3f ctr = {1.0f, 0.0f, 0.0f};

    /* Rotate the view in the azimuth direction */
    {
        math::mat4f rot = math::rotate({0.0f, 0.0f, 1.0f}, phi);
        math::vec4f vec = {ctr.x, ctr.y, ctr.z, 0.0f};
        vec = math::dot(rot, vec);
        ctr = {vec.x, vec.y, vec.z};
    }

    /*
     * Compute the local basis set. The Gram-Schmidt up-vector is z-direction
     * unless it is parallel to the w-direction, in which case, it will be in
     * the y-direction.
     */
    math::vec3f w = math::normalize(eye - ctr);
    math::vec3f up = {0.0f, 0.0f, 1.0f};
    math::vec3f u = math::normalize(math::cross(up, w));
    /* math::vec3f v = math::normalize(math::cross(w, u)); */

    /* Rotate the view in the polar direction */
    {
        math::mat4f rot = math::rotate(u, theta);
        math::vec4f vec = {ctr.x, ctr.y, ctr.z, 0.0f};
        vec = math::dot(rot, vec);
        ctr = {vec.x, vec.y, vec.z};
    }

    /* Return the modelviewprojection matrix */
    return math::lookat(
        {eye.x, eye.y, eye.z},
        {ctr.x, ctr.y, ctr.z},
        up);
}
