/*
 * transform.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_MATH_TRANSFORM_H_
#define ITO_MATH_TRANSFORM_H_

namespace ito {
namespace math {

/** ---------------------------------------------------------------------------
 * @brief Translate the matrix by d.
 */
template<typename T>
ito_inline mat4<T> translate(const vec3<T> d)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {1.0, 0.0, 0.0, d.x,
            0.0, 1.0, 0.0, d.y,
            0.0, 0.0, 1.0, d.z,
            0.0, 0.0, 0.0, 1.0};
}

template<typename T>
ito_inline mat4<T> translate(const mat4<T> m, const vec3<T> d)
{
    return dot(translate(d), m);
}

/** ---------------------------------------------------------------------------
 * @brief Scale the matrix by s.
 */
template<typename T>
ito_inline mat4<T> scale(const vec3<T> s)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    return {s.x, 0.0, 0.0, 0.0,
            0.0, s.y, 0.0, 0.0,
            0.0, 0.0, s.z, 0.0,
            0.0, 0.0, 0.0, 1.0};
}

template<typename T>
ito_inline mat4<T> scale(const mat4<T> m, const vec3<T> s)
{
    return dot(scale(s), m);
}

/** ---------------------------------------------------------------------------
 * @brief Rotate the matrix around n-axis by using Rodrigues formula.
 */
template<typename T>
ito_inline mat4<T> rotate(vec3<T> n, const T theta)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");

    /* Compute cross product matrix */
    n = normalize(n);
    mat4<T> R = { 0.0, -n.z,  n.y, 0.0,
                  n.z,  0.0, -n.x, 0.0,
                 -n.y,  n.x,  0.0, 0.0,
                  0.0,  0.0,  0.0, 0.0};

    /* Compute rotation matrix */
    mat4<T> result = mat4<T>::eye;
    result += R * std::sin(theta);
    result += dot(R, R) * (1.0 - std::cos(theta));
    return result;
}

template<typename T>
ito_inline mat4<T> rotate(const mat4<T> m, vec3<T> n, const T theta)
{
    return dot(rotate(n, theta), m);
}

/** ---------------------------------------------------------------------------
 * @brief Return the matrix that maps direction vector-a to vector-b.
 */
template<typename T>
ito_inline mat4<T> align(vec3<T> a, vec3<T> b)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");

    /* Compute the cross product */
    a = normalize(a);
    b = normalize(b);
    T cos_theta = dot(a,b);
    vec3<T> n = cross(a, b);

    /* Return identity if the direction vectors are already aligned. */
    if (iseq(cos_theta, -1.0)) {
        return mat4<T>::eye * (-1.0);
    }

    if (iseq(cos_theta, 1.0)) {
        return mat4<T>::eye;
    }

    /* Return the rotation matrix using Rodrigues formula. */
    return rotate(n, std::acos(cos_theta));
}

template<typename T>
ito_inline mat4<T> align(const mat4<T> m, vec3<T> a, vec3<T> b)
{
    return dot(align(a, b), m);
}

/** ---------------------------------------------------------------------------
 * @brief Compute a lookat local frame of reference given the eye position,
 * the ctr reference viewpoint and the updward directon vector up.
 *
 * The convention used here is a right-handed basis set, where the z-axis is
 * represented by forward f-vector. The x-axis is represented by the sideway
 * s-vector and the y-axis is represented by the upward u-vector in the local
 * reference frame.
 */
template<typename T>
ito_inline mat4<T> lookat(
    const vec3<T> &eye,
    const vec3<T> &ctr,
    const vec3<T> &up)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");

    /* Compute the lookat basis set s-u-f. */
    vec3<T> f = normalize(eye - ctr);       /* f = eye - ctr */
    vec3<T> s = normalize(cross(up, f));    /* s = up x f */
    vec3<T> u = normalize(cross(f, s));     /* u = s x f */

    /* Compute the lookat projection matrix with eye translation. */
    mat4<T> M{ s.x,  s.y,  s.z, -eye.x,
               u.x,  u.y,  u.z, -eye.y,
               f.x,  f.y,  f.z, -eye.z,
               0.0,  0.0,  0.0,    1.0};
    return M;
}

template<typename T>
ito_inline mat4<T> lookat(
    const mat4<T> m,
    const vec3<T> &eye,
    const vec3<T> &ctr,
    const vec3<T> &up)
{
    return dot(lookat(eye, ctr, up), m);
}

/** ---------------------------------------------------------------------------
 * perspective
 * @brief Compute the perspective projection matrix.
 */
template<typename T>
ito_inline mat4<T> perspective(
    const T fovy,
    const T aspect,
    const T znear,
    const T zfar)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");

    const T tan_half_fovy = std::tan(fovy / 2.0);

    mat4<T> result{};
    result.xx =  1.0 / (tan_half_fovy * aspect);
    result.xy =  0.0;
    result.xz =  0.0;
    result.xw =  0.0;

    result.yx =  0.0;
    result.yy =  1.0 / tan_half_fovy;
    result.yz =  0.0;
    result.yw =  0.0;

    result.zx =  0.0;
    result.zy =  0.0;
    result.zz = -(zfar + znear) / (zfar - znear);
    result.zw = -(2.0 * zfar * znear) / (zfar - znear);

    result.wx =  0.0;
    result.wy =  0.0;
    result.wz = -1.0;
    result.ww =  0.0;
    return result;
}

template<typename T>
ito_inline mat4<T> perspective(
    const mat4<T> m,
    const T fovy,
    const T aspect,
    const T znear,
    const T zfar)
{
    return dot(perspective(fovy, aspect, znear, zfar), m);
}

/** ---------------------------------------------------------------------------
 * @brief Compute the orthographic projection matrix.
 */
template<typename T>
ito_inline mat4<T> ortho(
    const T left,
    const T right,
    const T bottom,
    const T top,
    const T znear,
    const T zfar)
{
    static_assert(std::is_floating_point<T>::value, "non floating point");
    mat4<T> result{};
    result.xx =  2.0 / (right - left);
    result.xy =  0.0;
    result.xz =  0.0;
    result.xw =  -(right + left) / (right - left);

    result.yx =  0.0;
    result.yy =  2.0 / (top - bottom);
    result.yz =  0.0;
    result.yw =  -(top + bottom) / (top - bottom);

    result.zx =  0.0;
    result.zy =  0.0;
    result.zz = -2.0 / (zfar - znear);
    result.zw = -(zfar + znear) / (zfar - znear);

    result.wx =  0.0;
    result.wy =  0.0;
    result.wz =  0.0;
    result.ww =  1.0;
    return result;
}

template<typename T>
ito_inline mat4<T> ortho(
    const mat4<T> m,
    const T left,
    const T right,
    const T bottom,
    const T top,
    const T znear,
    const T zfar)
{
    return dot(ortho(left, right, bottom, top, znear, zfar), m);
}

} /* math */
} /* ito */

/** ---- simd implementations ------------------------------------------------
 */
#ifdef __AVX__
#include "simd/transform.hpp"
#endif

#endif /* ITO_MATH_TRANSFORM_H_ */
