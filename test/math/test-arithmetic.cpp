/*
 * test-arithmetic.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "Catch2/catch.hpp"
#include "test-arithmetic2.hpp"
#include "test-arithmetic3.hpp"
#include "test-arithmetic4.hpp"

/**
 * @brief Vector arithmetic test client.
 */
TEST_CASE("Arithmetic") {
    SECTION("vec2") {
        test_arithmetic2<float>();
        test_arithmetic2<double>();
    }
    SECTION("vec3") {
        test_arithmetic3<float>();
        test_arithmetic3<double>();
    }
    SECTION("vec4") {
        test_arithmetic4<float>();
        test_arithmetic4<double>();
    }
}
