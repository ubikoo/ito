/*
 * test-random.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "Catch2/catch.hpp"
#include "ito/core.hpp"
#include "ito/math.hpp"

/**
 * @brief Random test client.
 */
TEST_CASE("Random")
{
    static const size_t n_short_runs    = 4096;
    static const size_t n_short_samples = 262144;
    static const size_t n_long_samples  = n_short_runs * n_short_samples;

    /* 32-bit random number generator */
    SECTION("random32")
    {
        /* Create a collection of random engines, one for each thread. */
        std::vector<ito::math::random_engine> engine;
        ito::math::random_device device;
        std::cout << ito::str::format("%0x\n", device.fp.get());
        ito_pragma(omp parallel default(none) shared(std::cout, device, engine))
        {
            size_t n_threads = omp_get_num_threads();
            ito_pragma(omp master)
            for (size_t i = 0; i < n_threads; ++i) {
                engine.push_back(ito::math::make_random(device));
                std::cout << "engine " << engine.size()
                    << " x "  << engine.back().x << " "
                    << " y "  << engine.back().y << " "
                    << " z1 " << engine.back().z1 << " "
                    << " c1 " << engine.back().c1 << " "
                    << " z2 " << engine.back().z2 << " "
                    << " c2 " << engine.back().c2 << "\n";
            }
        }

        /* 32-bit short runs */
        for (size_t ir = 0; ir < n_short_runs; ++ir) {
            /* Generate random32 data */
            std::vector<uint32_t> samples(n_short_samples, 0);
            ito_pragma(omp parallel default(none) shared(std::cout, samples, engine))
            {
                size_t tid = omp_get_thread_num();
                ito_pragma(omp for schedule(static))
                for (size_t i = 0; i < samples.size(); ++i) {
                    samples[i] = ito::math::random32(engine[tid]);
                }
            }

            /* Write the data to output file */
            std::string filename("/tmp/out.random32." + std::to_string(ir));
            ito::file_ptr fp = ito::make_file(filename, "wb");
            REQUIRE(fp);

            int64_t ret = ito::file::write(fp,
                samples.data(), samples.size() * sizeof(uint32_t));
            REQUIRE(ret == 1);
        }

        /* 32-bit long run */
        {
            /* Generate random32 data */
            std::vector<uint32_t> samples(n_long_samples, 0);
            ito_pragma(omp parallel default(none) shared(samples, engine))
            {
                size_t tid = omp_get_thread_num();
                ito_pragma(omp for schedule(static))
                for (size_t i = 0; i < samples.size(); ++i) {
                    samples[i] = ito::math::random32(engine[tid]);
                }
            }

            /* Write the data to output file */
            std::string filename("/tmp/out.random32.long");
            ito::file_ptr fp = ito::make_file(filename, "wb");
            REQUIRE(fp);

            int64_t ret = ito::file::write(fp,
                samples.data(), samples.size() * sizeof(uint32_t));
            REQUIRE(ret == 1);
        }
    }

    /* 64-bit random number generator */
    SECTION("random64")
    {
        /* Create a collection of random engines, one for each thread. */
        std::vector<ito::math::random_engine> engine;
        ito::math::random_device device;
        std::cout << ito::str::format("%0x\n", device.fp.get());
        ito_pragma(omp parallel default(none) shared(std::cout, device, engine))
        {
            size_t n_threads = omp_get_num_threads();
            ito_pragma(omp master)
            for (size_t i = 0; i < n_threads; ++i) {
                engine.push_back(ito::math::make_random(device));
                std::cout << "engine " << engine.size()
                    << " x "  << engine.back().x << " "
                    << " y "  << engine.back().y << " "
                    << " z1 " << engine.back().z1 << " "
                    << " c1 " << engine.back().c1 << " "
                    << " z2 " << engine.back().z2 << " "
                    << " c2 " << engine.back().c2 << "\n";
            }
        }

        /* 64-bit short runs */
        for (size_t ir = 0; ir < n_short_runs; ++ir) {
            /* Generate random64 data */
            std::vector<uint64_t> samples(n_short_samples, 0);
            ito_pragma(omp parallel default(none) shared(samples, engine))
            {
                size_t tid = omp_get_thread_num();
                ito_pragma(omp for schedule(static))
                for (size_t i = 0; i < samples.size(); ++i) {
                    samples[i] = ito::math::random64(engine[tid]);
                }
            }

            /* Write the data to output file */
            std::string filename("/tmp/out.random64." + std::to_string(ir));
            ito::file_ptr fp = ito::make_file(filename, "wb");
            REQUIRE(fp);

            int64_t ret = ito::file::write(fp,
                samples.data(), samples.size() * sizeof(uint64_t));
            REQUIRE(ret == 1);
        }

        /* 64-bit long run */
        {
            /* Generate random64 data */
            std::vector<uint64_t> samples(n_long_samples, 0);
            ito_pragma(omp parallel default(none) shared(samples, engine))
            {
                size_t tid = omp_get_thread_num();
                ito_pragma(omp for schedule(static))
                for (size_t i = 0; i < samples.size(); ++i) {
                    samples[i] = ito::math::random64(engine[tid]);
                }
            }

            /* Write the data to output file */
            std::string filename("/tmp/out.random64.long");
            ito::file_ptr fp = ito::make_file(filename, "wb");
            REQUIRE(fp);

            int64_t ret = ito::file::write(fp,
                samples.data(), samples.size() * sizeof(uint64_t));
            REQUIRE(ret == 1);
        }
    }
}

