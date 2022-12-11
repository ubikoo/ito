/*
 * test-vector2.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef TEST_MATH_VECTOR2_H_
#define TEST_MATH_VECTOR2_H_

#include "ito/core.hpp"
#include "ito/math.hpp"

/**
 * @brief vec2 test client.
 */
template<typename T>
void test_vector2(void)
{
    using namespace ito;

    /* Constructor and assignment. */
    {
        math::vec2<T> a{};
        math::vec2<T> b = {static_cast<T>(1), static_cast<T>(1)};
        math::vec2<T> c = math::vec2<T>::zeros;
        math::vec2<T> d = math::vec2<T>::ones;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == c[i]);
            REQUIRE(b[i] == d[i]);
        }
    }

    /* Compound assignment operators (vector). */
    {
        math::vec2<T> a = {};
        a += math::vec2<T>::ones;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }

        a -= math::vec2<T>::ones;
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(0));
        }

        math::vec2<T> b = {static_cast<T>(2), static_cast<T>(2)};
        a = math::vec2<T>::ones;
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
        math::vec2<T> a = {};
        a += static_cast<T>(1);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }

        a -= static_cast<T>(1);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(0));
        }

        a = math::vec2<T>::ones;
        a *= static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(2));
        }

        a /= static_cast<T>(2);
        for (size_t i = 0; i < a.length; ++i) {
            REQUIRE(a[i] == static_cast<T>(1));
        }
    }


    /* Arithmetic operators (vector). */
    {
        math::vec2<T> a = math::vec2<T>::ones * static_cast<T>(2);
        math::vec2<T> b = math::vec2<T>::ones * static_cast<T>(4);
        math::vec2<T> c = a + b;
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
        math::vec2<T> a = math::vec2<T>::ones * static_cast<T>(4);
        math::vec2<T> c = a + static_cast<T>(2);
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
        math::vec2<T> a = math::vec2<T>::ones * static_cast<T>(2);

        math::vec2<T> c = +a;
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

#endif /* TEST_MATH_VECTOR2_H_ */
