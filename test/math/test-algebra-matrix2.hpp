/*
 * test-algebra-matrix2.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_MATH_ALGEBRA_MATRIX2_H_
#define TEST_MATH_ALGEBRA_MATRIX2_H_

#include "ito/core.hpp"
#include "ito/math.hpp"

/**
 * @brief mat2 algebra test client.
 */
template<typename T>
void test_algebra_matrix2_run(const size_t n_iters)
{
    using namespace ito;

    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_real_distribution<T> dist(0.0, 1.0);

    for (size_t iter = 0; iter < n_iters; ++iter) {
        /* Test arithmetic functions */
        {
            math::mat2<T> arr_a = {
                dist(rng), dist(rng),
                dist(rng), dist(rng)};
            math::mat2<T> arr_b = -arr_a;
            math::mat2<T> arr_c = math::mat2<T>::zeros;

            (arr_c = arr_a) += arr_b;
            for (size_t j = 0; j < arr_c.length; ++j) {
                REQUIRE(math::iseq(arr_c[j], static_cast<T>(0)));
            }

            (arr_c = arr_a) -= arr_b;
            for (size_t j = 0; j < arr_c.length; ++j) {
                REQUIRE(math::iseq(arr_c[j], static_cast<T>(2) * arr_a[j]));
            }

            arr_c = arr_a + arr_b;
            for (size_t j = 0; j < arr_c.length; ++j) {
                REQUIRE(math::iseq(arr_c[j], static_cast<T>(0)));
            }

            arr_c = arr_a - arr_b;
            for (size_t j = 0; j < arr_c.length; ++j) {
                REQUIRE(math::iseq(arr_c[j], static_cast<T>(2) * arr_a[j]));
            }
        }

        /* Matrix transpose */
        {
            math::mat2<T> arr_a = {
                dist(rng), dist(rng),
                dist(rng), dist(rng)};
            arr_a += math::mat2<T>::eye * static_cast<T>(2);
            math::mat2<T> arr_b = math::transpose(arr_a);   /* b = a^t */
            math::mat2<T> arr_c = math::dot(arr_a, arr_b);  /* c = a * a^t */

            T det_a = math::determinant(arr_a);
            T det_b = math::determinant(arr_b);
            T det_c = math::determinant(arr_c);

            REQUIRE(math::iseq(det_a, det_b));
            REQUIRE(math::iseq(det_a * det_a, det_c));
        }

        /* Matrix inverse */
        {
            math::mat2<T> arr_a = math::mat2<T>{
                dist(rng), dist(rng),
                dist(rng), dist(rng)};
            arr_a += math::mat2<T>::eye * static_cast<T>(2);
            math::mat2<T> inv_a = math::inverse(arr_a);
            math::mat2<T> eye_a = math::dot(arr_a, inv_a);

            math::mat2<T> identity = math::mat2<T>::eye;
            for (size_t j = 0; j < eye_a.length; ++j) {
                REQUIRE(math::iseq(eye_a[j], identity[j]));
            }
        }

        /* Matrix solve, x = a^-1 * arr_b, err = a * x - b */
        {
            math::mat2<T> arr_a = math::mat2<T>{
                dist(rng), dist(rng),
                dist(rng), dist(rng)};
            arr_a += math::mat2<T>::eye * static_cast<T>(2);
            math::mat2<T> inv_a = math::inverse(arr_a);
            math::vec2<T> vec_b = {dist(rng), dist(rng)};
            math::vec2<T> vec_x = math::dot(inv_a, vec_b);
            math::vec2<T> err = vec_b - math::dot(arr_a, vec_x);
            REQUIRE(math::iseq(math::norm(err), static_cast<T>(0)));
        }
    }
}

template<typename T>
void test_algebra_matrix2(const size_t n_iters)
{
    /* Test random matrices */
    ito_pragma(omp parallel default(none) shared(n_iters))
    {
        size_t n_threads = omp_get_num_threads();
        ito_pragma(omp parallel for schedule(static))
        for (size_t ix = 0; ix < n_threads; ++ix) {
            test_algebra_matrix2_run<T>(n_iters);
        }
    }
}

#endif /* TEST_MATH_ALGEBRA_MATRIX2_H_ */
