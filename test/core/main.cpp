/*
 * test.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "ito/core.hpp"

#include "test-memory.hpp"
#include "test-string.hpp"
#include "test-file.hpp"

/** ---- Memory Tests ---------------------------------------------------------
 * main test client
 */
int main(int argc, char const *argv[])
{
    try {
        test_core_memory();
        test_core_string();
        test_core_file();
    } catch (std::exception& e) {
        ito_throw(ito::str::format("%s\nFAIL", e.what()));
    }

    exit(EXIT_SUCCESS);
}
