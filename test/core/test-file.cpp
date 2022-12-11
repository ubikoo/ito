/*
 * test-file.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "ito/core.hpp"
#include "test-file.hpp"

/** ---- File utilities -------------------------------------------------------
 */
static bool check_file(ito::File &file)
{
    auto boolchar = [] (bool v) -> char { return v ? 'T' : 'F'; };
    std::cout << ito::str::format(
        "open %c, error %c, eof %c\n",
        boolchar(file.get() != nullptr),
        boolchar(std::ferror(file.get()) == 0),
        boolchar(std::feof(file.get()) == 0));
    return (std::ferror(file.get()) == 0) && (std::feof(file.get()) == 0);
}

/**
 * test_core_file
 * @brief test ito::File api.
 */
void test_core_file(void)
{
    /* ---- Test 1 stdin/stdout -----------------------------------------------
     */
    std::vector<std::string> buffer_stdin;

    std::cout << "\n>>> Test 1 readlines\n";
    {
        ito::File fin = ito::make_file(stdin);
        ito_assert(fin, "FAIL");
        ito_assert(check_file(fin), "FAIL");

        ito::file::readlines(fin, buffer_stdin);
    }

    std::cout << "\n>>> Test 1 writeline/writelines, sep='\\n'\n";
    {
        ito::File fout = ito::make_file(stdout);
        ito_assert(fout, "FAIL");
        ito_assert(check_file(fout), "FAIL");

        ito::file::writelines(fout, buffer_stdin, "\n");
        for (auto &line : buffer_stdin) {
            ito::file::writeline(fout, line, "\n");
        }
    }

    std::cout << "\n>>> Test 1 writeline/writelines, sep='(@)'\n";
    {
        ito::File fout = ito::make_file(stdout);
        ito_assert(fout, "FAIL");
        ito_assert(check_file(fout), "FAIL");

        ito::file::writelines(fout, buffer_stdin, "(@)");
        for (auto &line : buffer_stdin) {
            ito::file::writeline(fout, line, "(@)");
        }
    }

    std::cout << "\n>>> Test 1 printf\n";
    {
        ito::File fout = ito::make_file(stdout);
        ito_assert(fout, "FAIL");
        ito_assert(check_file(fout), "FAIL");

        for (auto &line : buffer_stdin) {
            ito::file::printf(fout, "%s\n", line.c_str());
        }
    }

    /* ---- Test 2 file in/out ------------------------------------------------
     */
    std::vector<std::string> buffer_in;

    std::cout << "\n>>> Test 2 open filename\n";
    {
        ito::File fin = ito::make_file("data/in.lorem_ipsum.txt", "r");
        ito_assert(fin, "FAIL");
        ito_assert(check_file(fin), "FAIL");
    }

    std::cout << "\n>>> Test 2 readlines filename\n";
    {
        ito::File fin = ito::make_file("data/in.lorem_ipsum.txt", "r");
        ito_assert(fin, "FAIL");
        ito_assert(check_file(fin), "FAIL");

        ito::file::readlines(fin, buffer_in);

        auto it_stdin = buffer_stdin.begin();
        auto it_in = buffer_in.begin();
        while (it_stdin != buffer_stdin.end() && it_in != buffer_in.end()) {
            ito_assert(*it_stdin == *it_in, "FAIL");
            ++it_stdin;
            ++it_in;
        }
    }

    std::cout << "\n>>> Test 2 readline filename\n";
    {
        ito::File fin = ito::make_file("data/in.lorem_ipsum.txt", "r");
        ito_assert(fin, "FAIL");
        ito_assert(check_file(fin), "FAIL");

        std::string line;
        while (ito::file::readline(fin, line)) {
            std::cout << line << "\n";
        }
    }

    std::cout << "\n>>> Test 2 readline / writeline\n";
    {
        ito::File fin = ito::make_file("data/in.lorem_ipsum.txt", "r");
        ito_assert(fin, "FAIL");
        ito_assert(check_file(fin), "FAIL");

        ito::File fout_new = ito::make_file("data/out.lorem_ipsum_new.txt", "w");
        ito_assert(fout_new, "FAIL");
        ito_assert(check_file(fout_new), "FAIL");

        ito::File fout_csv = ito::make_file("data/out.lorem_ipsum_csv.txt", "w");
        ito_assert(fout_csv, "FAIL");
        ito_assert(check_file(fout_csv), "FAIL");

        std::string line;
        while (ito::file::readline(fin, line)) {
            ito::file::writeline(fout_new, line, "\n");
            ito::file::writeline(fout_csv, line, ",");
        }
    }

    std::cout << "\n>>> Test 2 readline / writeline 1, size=4096\n";
    {
        ito::File fin = ito::make_file("data/in.lorem_ipsum.txt", "r");
        ito_assert(fin, "FAIL");
        ito_assert(check_file(fin), "FAIL");

        ito::File fout_new = ito::make_file("data/out.lorem_ipsum_new_1.txt", "w");
        ito_assert(fout_new, "FAIL");
        ito_assert(check_file(fout_new), "FAIL");

        ito::File fout_csv = ito::make_file("data/out.lorem_ipsum_csv_1.txt", "w");
        ito_assert(fout_csv, "FAIL");
        ito_assert(check_file(fout_csv), "FAIL");

        std::string line;
        while (ito::file::readline(fin, line, 4096, '.')) {
            ito::file::writeline(fout_new, line, "\n");
            ito::file::writeline(fout_csv, line, ",");
        }
    }

    std::cout << "\n>>> Test 2 readlines / writelines 2, size=4096\n";
    {
        ito::File fin = ito::make_file("data/in.lorem_ipsum.txt", "r");
        ito_assert(fin, "FAIL");
        ito_assert(check_file(fin), "FAIL");

        ito::File fout_new = ito::make_file("data/out.lorem_ipsum_new_2.txt", "w");
        ito_assert(fout_new, "FAIL");
        ito_assert(check_file(fout_new), "FAIL");

        ito::File fout_csv = ito::make_file("data/out.lorem_ipsum_csv_2.txt", "w");
        ito_assert(fout_csv, "FAIL");
        ito_assert(check_file(fout_csv), "FAIL");

        std::vector<std::string> buffer;
        ito::file::readlines(fin, buffer, 4096, '.');
        ito::file::writelines(fout_new, buffer, "\n");
        ito::file::writelines(fout_csv, buffer, ",");
    }

    /* ---- Test 3: binary read/write -----------------------------------------
     */
    std::cout << "\n>>> Test 3 read txt / write bin\n";
    {
        /* Read text file into character buffer. */
        ito::File fin = ito::make_file("data/in.lorem_ipsum.txt", "rb");
        ito_assert(fin, "FAIL");
        ito_assert(check_file(fin), "FAIL");

        int64_t length = ito::file::length(fin);
        std::vector<char> buffer(length);

        ito::file::read(fin, buffer.data(), buffer.size());
        ito_assert(check_file(fin), "FAIL");

        std::cout << ito::str::format("file length = %lld\n", length);
        std::cout << ito::str::format("buffer size = %lu\n", buffer.size());
        for (char n : buffer) {
            std::cout << n;
        }
        std::cout << "\n";

        /* Write binary file. */
        ito::File fout = ito::make_file("data/out.lorem_ipsum_1.bin", "wb");
        ito_assert(fout, "FAIL");
        ito_assert(check_file(fout), "FAIL");

        ito::file::write(fout, buffer.data(), buffer.size());
        ito_assert(check_file(fout), "FAIL");
    }

    std::cout << "\n>>> Test 3 read bin/write bin\n";
    {
        /* Read text file into character buffer. */
        ito::File fin = ito::make_file("data/out.lorem_ipsum_1.bin", "rb");
        ito_assert(fin, "FAIL");
        ito_assert(check_file(fin), "FAIL");

        int64_t length = ito::file::length(fin);
        std::vector<char> buffer(length);

        ito::file::read(fin, buffer.data(), buffer.size());
        ito_assert(check_file(fin), "FAIL");

        std::cout << ito::str::format("file length = %lld bytes\n", length);
        std::cout << ito::str::format("buffer size = %lu\n", buffer.size());
        for (char n : buffer) {
            std::cout << n;
        }
        std::cout << "\n";

        /* Write binary file. */
        ito::File fout = ito::make_file("data/out.lorem_ipsum_2.bin", "wb");
        ito_assert(fout, "FAIL");
        ito_assert(check_file(fout), "FAIL");

        ito::file::write(fout, buffer.data(), buffer.size());
        ito_assert(check_file(fout), "FAIL");
    }
}
