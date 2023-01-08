/*
 * main.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "../params.hpp"

using namespace ito;

/** ---------------------------------------------------------------------------
 * main
 */
int main(int argc, char const *argv[])
{
    /* Create a OpenCL context with all devices in the first platform. */
    cl_context context = cl::CreateContext(CL_DEVICE_TYPE_ALL);
    for (auto &it : cl::GetContextDevices(context)) {
        std::cout << cl::GetDeviceInfoString(it) << "\n";
    }

    /* Release the context before exit. */
    cl::ReleaseContext(context);

    exit(EXIT_SUCCESS);
}