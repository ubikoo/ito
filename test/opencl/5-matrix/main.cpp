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
#include <random>
#include <algorithm>

#include "../base.hpp"
using namespace ito;

/** ---------------------------------------------------------------------------
 * Program matmul kernel source.
 */
const std::string matmul_source = ito_strify(
__kernel void matmul(
    __global const float *a,
    __global const float *b,
    __global float *c,
    const unsigned long dim_M,
    const unsigned long dim_N,
    const unsigned long dim_P)
{
    const size_t i = get_global_id(0);    // global pos in x-direction
    const size_t j = get_global_id(1);    // global pos in y-direction

    float sum = 0.0f;
    for (size_t k = 0; k < dim_P; ++k) {
        sum += a[i*dim_P + k] * b[k*dim_N + j];
    }
    c[i*dim_M + j] = sum;
});

/** ---------------------------------------------------------------------------
 * Setup OpenCL context.
 */
void Setup(
    cl_context &context,
    cl_device_id &device,
    cl_command_queue &queue,
    cl_program &program,
    cl_kernel &kernel,
    std::vector<cl_mem> &buffers,
    std::vector<cl_mem> &images)
{
    /* Setup OpenCL context on the first available platform. */
    context = cl::CreateContext(CL_DEVICE_TYPE_GPU);
    device = cl::GetContextDevice(context, Params::device_index);
    std::cout << cl::GetDeviceInfoStr(device) << "\n";

    /* Create a command queue on the specified device. */
    queue = cl::CreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE);

    /* Create a OpenCL program for the kernel source. */
    program = cl::CreateProgramWithSource(context, matmul_source);
    cl::BuildProgram(program, device, "");

    /* Create the OpenCL kernel. */
    kernel = cl::CreateKernel(program, "matmul");
}

/** ---------------------------------------------------------------------------
 * Teardown OpenCL data.
 */
void Teardown(
    cl_context &context,
    cl_device_id &device,
    cl_command_queue &queue,
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
    cl::ReleaseCommandQueue(queue);
    cl::ReleaseDevice(device);
    cl::ReleaseContext(context);
}

/** ---------------------------------------------------------------------------
 * main
 */
int main(int argc, char const *argv[])
{
    cl_context context = NULL;
    cl_device_id device = NULL;
    cl_command_queue queue = NULL;
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    std::vector<cl_mem> buffers;
    std::vector<cl_mem> images;

    /*
     * Setup OpenCL context.
     */
    Setup(context, device, queue, program, kernel, buffers, images);

    /*
     * Create memory objects for the kernel arguments. First create host memory
     * arrays for the kernel args, then create the memory objects on the device.
     */
    size_t dim_M = 1024;
    size_t dim_N = 1024;
    size_t dim_P = 2048;

    std::vector<float> arr_a(dim_M * dim_P, 0.0f);
    std::vector<float> arr_b(dim_P * dim_N, 0.0f);
    std::vector<float> arr_c(dim_M * dim_N, 0.0f);
    std::vector<float> arr_d(dim_M * dim_N, 0.0f);

    buffers.emplace_back(cl::CreateBuffer(
        context,
        CL_MEM_READ_ONLY,
        dim_M * dim_P * sizeof(float),
        (void *) NULL));

    buffers.emplace_back(cl::CreateBuffer(
        context,
        CL_MEM_READ_ONLY,
        dim_P * dim_N * sizeof(float),
        (void *) NULL));

    buffers.emplace_back(cl::CreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        dim_M * dim_N * sizeof(float),
        (void *) NULL));

    /*
     * Set the kernel arguments.
     */
    cl::SetKernelArg(kernel, 0, sizeof(cl_mem), &buffers[0]);
    cl::SetKernelArg(kernel, 1, sizeof(cl_mem), &buffers[1]);
    cl::SetKernelArg(kernel, 2, sizeof(cl_mem), &buffers[2]);
    cl::SetKernelArg(kernel, 3, sizeof(cl_ulong), &dim_M);
    cl::SetKernelArg(kernel, 4, sizeof(cl_ulong), &dim_N);
    cl::SetKernelArg(kernel, 5, sizeof(cl_ulong), &dim_P);

    /*
     * Queue the kernel up for execution
     */
    const size_t max_iters = 4;
    for (size_t iter = 0; iter < max_iters; ++iter) {
       /*
        * Generate new data
        */
        {
            std::random_device seed;    /* rng device */
            std::mt19937 rng(seed());   /* rng engine */
            std::normal_distribution<float> dist(0.0, 1.0);
            auto f = [&](const float &v) -> float { return dist(rng); };
            std::transform(arr_a.begin(), arr_a.end(), arr_a.begin(), f);
            std::transform(arr_b.begin(), arr_b.end(), arr_b.begin(), f);
        }

       /*
        * Update the gpu buffer store.
        */
        std::vector<cl_event> event_write_buffers(3);
        {
            cl::EnqueueWriteBuffer(
                queue,
                buffers[0],
                CL_TRUE,
                0,
                dim_M * dim_P * sizeof(float),
                (void *) arr_a.data(),
                NULL,
                &event_write_buffers[0]);
            cl::EnqueueWriteBuffer(
                queue,
                buffers[1],
                CL_TRUE,
                0,
                dim_P * dim_N * sizeof(float),
                (void *) arr_b.data(),
                NULL,
                &event_write_buffers[1]);
            cl::EnqueueWriteBuffer(
                queue,
                buffers[2],
                CL_TRUE,
                0,
                dim_M * dim_N * sizeof(float),
                (void *) arr_c.data(),
                NULL,
                &event_write_buffers[2]);
        }

       /*
        * Run the kernel.
        */
        {
            /* Start time */
            auto tic = std::chrono::high_resolution_clock::now();

            /* Set the size of the NDRange workgroups */
            cl::NDRange global_ws = cl::NDRange::Make(dim_M, dim_N);
            cl::NDRange local_ws = cl::NDRange::Make(
                Params::work_group_size_2d,
                Params::work_group_size_2d);

            /* Run the kernel */
            cl_event event_matmul;
            cl::EnqueueNDRangeKernel(
                queue,
                kernel,
                cl::NDRange::Null,
                global_ws,
                local_ws,
                &event_write_buffers,
                &event_matmul);

            /* Wait for kernel to compute */
            cl::Finish(queue);

            /* End time */
            cl_ulong time_start = cl::GetCommandStart(event_matmul);
            cl_ulong time_end = cl::GetCommandEnd(event_matmul);
            cl_ulong exec_time = time_end - time_start;
            double exec_time_msec = (double) exec_time / 1000000;

            auto toc = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;

            std::printf("iter %lu\n", iter);
            std::printf("elapsed time: %lf msec\n", msec.count());
            std::printf("execution time: %lf msec\n", exec_time_msec);
        }

        /*
         * Read the output buffer back to the host.
         */
        {
            cl::EnqueueReadBuffer(
                queue,
                buffers[2],
                CL_TRUE,
                0,
                dim_M * dim_N * sizeof(float),
                (void *) arr_c.data(),
                NULL,
                NULL);
        }

        /*
         * Compute the error
         */
        {
            /* Start time */
            auto tic = std::chrono::high_resolution_clock::now();

            /* Run */
#if 1
            for (size_t i = 0; i < dim_M; ++i) {
                for (size_t j = 0; j < dim_N; ++j) {
                    float sum = 0;
                    for (size_t k = 0; k < dim_P; ++k) {
                        sum += arr_a[i*dim_P + k] * arr_b[k*dim_N + j];
                    }
                    arr_d[i*dim_M + j] = sum;
                }
            }
#else
            auto matmul = [&] (size_t i) -> void {
                for (size_t j = 0; j < dim_N; ++j) {
                    float sum = 0;
                    for (size_t k = 0; k < dim_P; ++k) {
                        sum += arr_a[i*dim_P + k] * arr_b[k*dim_N + j];
                    }
                    arr_d[i*dim_M + j] = sum;
                }
            };
            ito_pragma(omp parallel for default(none) \
                shared(dim_M, matmul) schedule(static))
            for (size_t i = 0; i < dim_M; ++i) {
                matmul(i);
            }
#endif

            /* End time */
            auto toc = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double,std::ratio<1,1000>> msec = toc-tic;
            std::printf("serial time: %lf msec\n", msec.count());

            double sum_c = 0.0;
            double sum_d = 0.0;
            double err = 0.0;
            for (size_t i = 0; i < dim_M; ++i) {
                for (size_t j = 0; j < dim_N; ++j) {
                    size_t k = i * dim_M + j;
                    sum_c += std::fabs(arr_c[k]);
                    sum_d += std::fabs(arr_d[k]);
                    err += std::fabs(arr_c[k] - arr_d[k]);
                }
            }
            std::printf("sum_c = %lf\n", sum_c);
            std::printf("sum_d = %lf\n", sum_d);
            std::printf("|arr_c - arr_d| = %lf\n", err);
        }
    }

    /*
     * Teardown OpenCL data.
     */
    Teardown(context, device, queue, program, kernel, buffers, images);

    exit(EXIT_SUCCESS);
}