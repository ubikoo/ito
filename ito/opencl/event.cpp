/*
 * event.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "event.hpp"

namespace ito {
namespace cl {

/**
 * @brief Create a NULL initialized event.
 */
cl_event CreateEvent(void)
{
    cl_event event = NULL;
    return event;
}

/**
 * @brief Create a user event object.
 */
cl_event CreateUserEvent(const cl_context &context)
{
    cl_int err;
    cl_event event = clCreateUserEvent(context, &err);
    ito_assert(err == CL_SUCCESS, "clCreateUserEvent");
    return event;
}

/**
 * @brief Decrement the event reference count.
 */
cl_int ReleaseEvent(const cl_event &event)
{
    cl_int err = CL_INVALID_VALUE;
    if (event != NULL) {
        err = clReleaseEvent(event);
        ito_assert(err == CL_SUCCESS, "clReleaseEvent");
    }
    return err;
}

/**
 * @brief Wait for commands identified by all event objects to complete.
 */
cl_int WaitForEvents(const std::vector<cl_event> &events)
{
    cl_int err = clWaitForEvents(
        (cl_uint) events.size(),
        (!events.empty()) ? events.data() : NULL);
    ito_assert(err == CL_SUCCESS, "clWaitForEvents");
    return err;
}

/**
 * @brief Wait for commands identified by the event object to complete.
 */
cl_int WaitForEvent(const cl_event &event)
{
    cl_int err = clWaitForEvents(1, &event);
    ito_assert(err == CL_SUCCESS, "clWaitForEvents");
    return err;
}

/**
 * @brief Register a callback function for a specific command execution status.
 */
cl_int SetEventCallback(
    const cl_event &event,
    cl_int command_exec_callback_type,
  	void (CL_CALLBACK *pfn_event_notify) (
        cl_event event,
        cl_int event_command_exec_status,
        void *user_data),
  	void *user_data)
{
    cl_int err = clSetEventCallback(
        event,
        command_exec_callback_type,
        pfn_event_notify,
        user_data);
    ito_assert(err == CL_SUCCESS, "clSetEventCallback");
    return err;
}

/**
 * @brief Return a 64-bit value representing the current time counter in
 * nanoseconds when the command identified by the event starts execution.
 */
cl_ulong GetCommandStart(const cl_event &event)
{
    cl_ulong time_start;
    cl_int err = clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_START,
        sizeof(time_start),
        &time_start,
        NULL);
    ito_assert(err == CL_SUCCESS, "clGetEventProfilingInfo");
    return time_start;
}

/**
 * @brief Return a 64-bit value representing the current time counter in
 * nanoseconds when the command identified by the event finishes execution.
 */
cl_ulong GetCommandEnd(const cl_event &event)
{
    cl_ulong time_end;
    cl_int err = clGetEventProfilingInfo(
        event,
        CL_PROFILING_COMMAND_END,
        sizeof(time_end),
        &time_end,
        NULL);
    ito_assert(err == CL_SUCCESS, "clGetEventProfilingInfo");
    return time_end;
}

} /* cl */
} /* ito */
