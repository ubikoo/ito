/*
 * test-matrix3.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_MATH_MATRIX3_H_
#define TEST_MATH_MATRIX3_H_

#include "ito/core.hpp"
#include "ito/math.hpp"

/**
 * @brief mat3 test client.
 */
template<typename T>
void test_matrix3(void)
{
    using namespace ito;

    /* Constructor and assignment. */
    {
        math::mat3<T> a{};
        math::mat3<T> b = {
            static_cast<T>(1),
            static_cast<T>(1),
            static_cast<T>(1),

            static_cast<T>(1),
            static_cast<T>(1),
            static_cast<T>(1),

            static_cast<T>(1),
            static_cast<T>(1),
            static_cast<T>(1)};
        math::mat3<T> c = math::mat3<T>::zeros;
        math::mat3<T> d = math::mat3<T>::ones;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == c[i]);
            REQUIRE(b[i] == d[i]);
        }
    }

    /* Compound assignment operators (matrix). */
    {
        math::mat3<T> a = {};
        a += math::mat3<T>::ones;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }

        a -= math::mat3<T>::ones;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(0));
        }

        math::mat3<T> b = {
            static_cast<T>(2),
            static_cast<T>(2),
            static_cast<T>(2),

            static_cast<T>(2),
            static_cast<T>(2),
            static_cast<T>(2),

            static_cast<T>(2),
            static_cast<T>(2),
            static_cast<T>(2)};
        a = math::mat3<T>::ones;
        a *= b;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(2));
        }

        a /= b;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }
    }

    /* Compound assignment operators (scalar). */
    {
        math::mat3<T> a = {};
        a += static_cast<T>(1);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }

        a -= static_cast<T>(1);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(0));
        }

        a = math::mat3<T>::ones;
        a *= static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(2));
        }

        a /= static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }
    }


    /* Arithmetic operators (matrix). */
    {
        math::mat3<T> a = math::mat3<T>::ones * static_cast<T>(2);
        math::mat3<T> b = math::mat3<T>::ones * static_cast<T>(4);
        math::mat3<T> c = a + b;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(6));
        }

        c = b - a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }

        c = b * a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(8));
        }

        c = b / a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }
    }

    /* Arithmetic operators (scalar). */
    {
        math::mat3<T> a = math::mat3<T>::ones * static_cast<T>(4);
        math::mat3<T> c = a + static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(6));
        }

        c = a - static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }

        c = a * static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(8));
        }

        c = a / static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }
    }

    /* Unary and increment operators */
    {
        math::mat3<T> a = math::mat3<T>::ones * static_cast<T>(2);

        math::mat3<T> c = +a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }

        c = -a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(-2));
        }

        c = a++;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }

        c = a--;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(3));
        }

        c = ++a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(3));
        }

        c = --a;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(c[i] == static_cast<T>(2));
        }
    }
}

#endif /* TEST_MATH_MATRIX3_H_ */
