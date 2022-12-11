/*
 * test-ortho.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_MATH_ORTHO_H_
#define TEST_MATH_ORTHO_H_

#include "ito/core.hpp"
#include "ito/math.hpp"

/**
 * @brief ortho test client.
 */
template<typename T>
ito_inline bool test_ortho_is_valid(const ito::math::onb<T> &o)
{
    using namespace ito;

    /*
     * Check norm
     */
    T norm_u = std::fabs(math::norm(o.u));
    if (!math::iseq(norm_u, (T) 1)) {
        std::cerr << ito::str::format("norm_u %lf\n", norm_u);
        return false;
    }

    T norm_v = std::fabs(math::norm(o.v));
    if (!math::iseq(norm_v, (T) 1)) {
        std::cerr << ito::str::format("norm_v %lf\n", norm_v);
        return false;
    }

    T norm_w = std::fabs(math::norm(o.w));
    if (!math::iseq(norm_w, (T) 1)) {
        std::cerr << ito::str::format("norm_w %lf\n", norm_w);
        return false;
    }

    /*
     * Check orthogonality
     */
    T dot_uv = std::fabs(math::dot(o.u, o.v));
    if (!math::iseq(dot_uv, (T) 0)) {
        std::cerr << ito::str::format("dot_uv %lf\n", dot_uv);
        return false;
    }

    T dot_uw = std::fabs(math::dot(o.u, o.w));
    if (!math::iseq(dot_uw, (T) 0)) {
        std::cerr << ito::str::format("dot_uw %lf\n", dot_uw);
        return false;
    }

    T dot_vw = std::fabs(math::dot(o.v, o.w));
    if (!math::iseq(dot_vw, (T) 0)) {
        std::cerr << ito::str::format("dot_vw %lf\n", dot_vw);
        return false;
    }

    /*
     * Check cross products
     */
    T dot_wuv = math::dot(o.w, cross(o.u, o.v));
    if (!math::iseq(dot_wuv, (T) 1)) {
        std::cerr << ito::str::format("dot_wuv %lf\n", dot_wuv);
        return false;
    }

    T dot_uvw = math::dot(o.u, cross(o.v, o.w));
    if (!math::iseq(dot_uvw, (T) 1)) {
        std::cerr << ito::str::format("dot_uvw %lf\n", dot_uvw);
        return false;
    }

    T dot_vwu = math::dot(o.v, cross(o.w, o.u));
    if (!math::iseq(dot_vwu, (T) 1)) {
        std::cerr << ito::str::format("dot_vwu %lf\n", dot_vwu);
        return false;
    }

    return true;
}

template<typename T>
ito_inline bool test_ortho_is_nan(const ito::math::vec3<T> &v)
{
    using namespace ito;

    if (std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z)) {
        std::cerr << "isnan " << math::to_string(v) << "\n";
        return true;
    }
    return false;
}

template<typename T>
ito_inline void test_ortho_run(const size_t n_iters)
{
    using namespace ito;

    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_real_distribution<T> dist(-1.0, 1.0);

    static constexpr T zero = static_cast<T>(0);
    static constexpr T two = static_cast<T>(2);

    for (size_t iter = 0; iter < n_iters; ++iter) {
        math::vec3<T> arr_u = {two + dist(rng), dist(rng), dist(rng)};
        math::vec3<T> arr_v = {dist(rng), two + dist(rng), dist(rng)};
        math::vec3<T> arr_w = {dist(rng), dist(rng), two + dist(rng)};

        /* Check vector samples */
        REQUIRE(!test_ortho_is_nan(arr_u));
        REQUIRE(!test_ortho_is_nan(arr_v));
        REQUIRE(!test_ortho_is_nan(arr_w));

        /* Error message */
        auto errmsg1 = [] (
            const std::string msg,
            const math::vec3<T> &a,
            const math::onb<T> &onb) -> void {
            std::cerr << msg << "\n";
            std::cerr << "a " << math::to_string(a) << "\n";
            std::cerr << "onb " <<  math::to_string(onb) << "\n";
        };

        auto errmsg2 = [] (
            const std::string msg,
            const math::vec3<T> &a,
            const math::vec3<T> &b,
            const math::onb<T> &onb) -> void {
            std::cerr << msg << "\n";
            std::cerr << "a " << math::to_string(a) << "\n";
            std::cerr << "b " <<  math::to_string(b) << "\n";
            std::cerr << "onb " <<  math::to_string(onb) << "\n";
        };

        /* Check create_from_u */
        {
            math::onb<T> onb = math::onb<T>::create_from_u(arr_u);
            bool is_valid = test_ortho_is_valid(onb);
            if (!is_valid) {
                errmsg1("create_from_u", arr_u, onb);
            }
            REQUIRE(is_valid);
        }

        /* Check create_from_v */
        {
            math::onb<T> onb = math::onb<T>::create_from_v(arr_v);
            bool is_valid = test_ortho_is_valid(onb);
            if (!is_valid) {
                errmsg1("create_from_v", arr_v, onb);
            }
            REQUIRE(is_valid);
        }

        /* Check create_from_w */
        {
            math::onb<T> onb = math::onb<T>::create_from_w(arr_w);
            bool is_valid = test_ortho_is_valid(onb);
            if (!is_valid) {
                errmsg1("create_from_w", arr_w, onb);
            }
            REQUIRE(is_valid);
        }

        /* Check create_from_uv */
        {
            math::onb<T> onb = math::onb<T>::create_from_uv(arr_u, arr_v);
            bool is_valid = test_ortho_is_valid(onb);
            if (!is_valid) {
                errmsg2("create_from_uv", arr_u, arr_v, onb);
            }
            REQUIRE(is_valid);
        }

        /* Check create_from_vu */
        {
            math::onb<T> onb = math::onb<T>::create_from_vu(arr_v, arr_u);
            bool is_valid = test_ortho_is_valid(onb);
            if (!is_valid) {
                errmsg2("create_from_vu", arr_v, arr_u, onb);
            }
            REQUIRE(is_valid);
        }

        /* Check create_from_vw */
        {
            math::onb<T> onb = math::onb<T>::create_from_vw(arr_v, arr_w);
            bool is_valid = test_ortho_is_valid(onb);
            if (!is_valid) {
                errmsg2("create_from_vw", arr_v, arr_w, onb);
            }
            REQUIRE(is_valid);
        }

        /* Check create_from_wv */
        {
            math::onb<T> onb = math::onb<T>::create_from_wv(arr_w, arr_v);
            bool is_valid = test_ortho_is_valid(onb);
            if (!is_valid) {
                errmsg2("create_from_wv", arr_w, arr_v, onb);
            }
            REQUIRE(is_valid);
        }

        /* Check create_from_wu */
        {
            math::onb<T> onb = math::onb<T>::create_from_wu(arr_w, arr_u);
            bool is_valid = test_ortho_is_valid(onb);
            if (!is_valid) {
                errmsg2("create_from_wu", arr_w, arr_u, onb);
            }
            REQUIRE(is_valid);
        }

        /* Check create_from_uw */
        {
            math::onb<T> onb = math::onb<T>::create_from_uw(arr_u, arr_w);
            bool is_valid = test_ortho_is_valid(onb);
            if (!is_valid) {
                errmsg2("create_from_uw", arr_u, arr_w, onb);
            }
            REQUIRE(is_valid);
        }

        /* Check world_to_local and local_to_world */
        {
            math::onb<T> onb = math::onb<T>::create_from_u(arr_u);
            math::vec3<T> a = {dist(rng), dist(rng), dist(rng)};
            math::vec3<T> b = onb.world_to_local(a);
            math::vec3<T> e = a - onb.local_to_world(b);
            bool is_valid = math::iseq(norm(e), zero);
            if (!is_valid) {
                std::cerr << "world_to_local -> local_to_world\n";
                std::cerr << "o: " << math::to_string(onb) << "\n";
                std::cerr << "a: " << math::to_string(a) << "\n";
                std::cerr << "b: " << math::to_string(b) << "\n";
                std::cerr << "e: " << math::to_string(e) << "\n";
                std::cerr << "norm e: " << math::to_string(norm(e)) << "\n";
            }
            REQUIRE(is_valid);
        }
    }
}

template<typename T>
ito_inline void test_ortho(const size_t n_iters)
{
    /* Test random orthonormal basis */
    ito_pragma(omp parallel default(none) shared(n_iters))
    {
        size_t n_threads = omp_get_num_threads();
        ito_pragma(omp parallel for schedule(static))
        for (size_t ix = 0; ix < n_threads; ++ix) {
            test_ortho_run<T>(n_iters);
        }
    }
}

#endif /* TEST_MATH_ORTHO_H_ */
