/*
 * event.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENCL_EVENT_H_
#define ITO_OPENCL_EVENT_H_

#include <vector>
#include "base.hpp"

namespace ito {
namespace cl {

/**
 * @brief Create a NULL initialized event.
 */
cl_event CreateEvent(void);

/**
 * @brief Create a user event object.
 */
cl_event CreateUserEvent(const cl_context &context);

/**
 * @brief Release the event and decrement its reference count.
 */
void ReleaseEvent(const cl_event &event);

/**
 * @brief Wait for commands identified by all event objects to complete.
 */
cl_int WaitForEvents(const std::vector<cl_event> &events);

/**
 * @brief Wait for commands identified by the event object to complete.
 */
cl_int WaitForEvent(const cl_event &event);

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
  	void *user_data);

/**
 * @brief Return a 64-bit value representing the current time counter in
 * nanoseconds when the command identified by the event starts execution.
 */
cl_ulong GetCommandStart(const cl_event &event);

/**
 * @brief Return a 64-bit value representing the current time counter in
 * nanoseconds when the command identified by the event finishes execution.
 */
cl_ulong GetCommandEnd(const cl_event &event);

} /* cl */
} /* ito */

#endif /* ITO_OPENCL_EVENT_H_ */
