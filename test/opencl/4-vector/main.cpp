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

#include <vector>
#include <chrono>
#include <numeric>
#include "../params.hpp"

using namespace ito;

/** ---------------------------------------------------------------------------
 * Program vecsum kernel source.
 */
const std::string vecsum_source = ito_strify(
__kernel void vecsum(
    const unsigned long array_size,
    __global const float *a,
    __global const float *b,
    __global float *result)
{
    const size_t ix = get_global_id(0);
    if (ix < array_size) {
       result[ix] = a[ix] + b[ix];
    }
});

/** ---------------------------------------------------------------------------
 * Create OpenCL program.
 */
void Create(
    cl_program &program,
    cl_kernel &kernel,
    std::vector<cl_mem> &buffers,
    std::vector<cl_mem> &images)
{
    /* Create a OpenCL program for the kernel source. */
    program = cl::CreateProgramWithSource(clfw::Context(), vecsum_source);
    cl::BuildProgram(program, clfw::Device());

    /* Create the OpenCL kernel. */
    kernel = cl::CreateKernel(program, "vecsum");
}

/** ---------------------------------------------------------------------------
 * Destroy OpenCL program.
 */
void Destroy(
    cl_program &program,
    cl_kernel &kernel,
    std::vector<cl_mem> &buffers,
    std::vector<cl_mem> &images)
{
    for (auto &it : images) {
        cl::ReleaseMemObject(it);
    }
    for (auto &it : buffers) {
        cl::ReleaseMemObject(it);
    }
    cl::ReleaseKernel(kernel);
    cl::ReleaseProgram(program);
}

/** ---------------------------------------------------------------------------
 * Execute OpenCL program.
 */
void Execute(
    cl_program &program,
    cl_kernel &kernel,
    std::vector<cl_mem> &buffers,
    std::vector<cl_mem> &images)
{
    /*
     * Get references to the OpenCL context, device and command queue.
     */
    cl_context context = clfw::Context();
    // cl_device_id device = clfw::Device();
    cl_command_queue queue = clfw::Queue();

    /*
     * Create memory objects for the kernel arguments. First create host memory
     * arrays for the kernel args, then create the memory objects on the device.
     */
    size_t array_size = 1000000;
    std::vector<float> result(array_size, 0.0f);
    std::vector<float> a(array_size, 0.0f);
    std::vector<float> b(array_size, 0.0f);

    std::iota(a.begin(), a.end(), 0);
    std::iota(b.begin(), b.end(), 0);

    buffers.emplace_back(cl::CreateBuffer(
        context,
        CL_MEM_READ_ONLY,
        array_size * sizeof(float),
        (void *) NULL));

    buffers.emplace_back(cl::CreateBuffer(
        context,
        CL_MEM_READ_ONLY,
        array_size * sizeof(float),
        (void *) NULL));

    buffers.emplace_back(cl::CreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        array_size * sizeof(float),
        (void *) NULL));

    /*
     * Set the kernel arguments (result, a, b).
     */
    cl::SetKernelArg(kernel, 0, sizeof(cl_ulong), &array_size);
    cl::SetKernelArg(kernel, 1, sizeof(cl_mem), &buffers[0]);
    cl::SetKernelArg(kernel, 2, sizeof(cl_mem), &buffers[1]);
    cl::SetKernelArg(kernel, 3, sizeof(cl_mem), &buffers[2]);

    /*
     * Update the gpu buffer store.
     */
    cl::EnqueueWriteBuffer(
        queue,
        buffers[0],
        CL_TRUE,
        0,
        array_size * sizeof(float),
        (void *) a.data());

    cl::EnqueueWriteBuffer(
        queue,
        buffers[1],
        CL_TRUE,
        0,
        array_size * sizeof(float),
        (void *) b.data());

    /*
     * Queue the kernel up for execution
     */
    {
        /* Start time */
        auto tic = std::chrono::high_resolution_clock::now();

        /* Set the size of the NDRange workgroups */
        cl::NDRange local_ws  = cl::NDRange::Make(Params::kWorkGroupSize1d);
        cl::NDRange global_ws = cl::NDRange::Make(
            cl::NDRange::Roundup(array_size, Params::kWorkGroupSize1d));

        /* Run the kernel */
        cl::EnqueueNDRangeKernel(
            queue,
            kernel,
            cl::NDRange::Null,
            global_ws,
            local_ws);

        /* End time */
        auto toc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
        std::printf("elapsed time %lf\n", msec.count());
    }

    /*
     * Read the output buffer back to the host.
     */
    cl::EnqueueReadBuffer(
        queue,
        buffers[2],
        CL_TRUE,
        0,
        array_size * sizeof(float),
        (void *) result.data());
}

/** ---------------------------------------------------------------------------
 * main
 */
int main(int argc, char const *argv[])
{
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    std::vector<cl_mem> buffers;
    std::vector<cl_mem> images;

    /* Initialize OpenCL context on the specified device. */
    clfw::Init(CL_DEVICE_TYPE_GPU, Params::kDeviceIndex);
    std::cout << clfw::InfoString() << "\n";

    /* Run OpenCL program. */
    Create(program, kernel, buffers, images);
    Execute(program, kernel, buffers, images);
    Destroy(program, kernel, buffers, images);

    /* Terminate OpenCL context. */
    clfw::Terminate();

    exit(EXIT_SUCCESS);
}
