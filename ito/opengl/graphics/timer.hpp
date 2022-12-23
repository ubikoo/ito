/*
 * timer.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_GRAPHICS_TIMER_H_
#define ITO_OPENGL_GRAPHICS_TIMER_H_

#include "../base.hpp"

namespace ito {
namespace gl {

struct Timer {
    size_t period;          /* sampling period */
    size_t count;           /* framerate counter */
    double start;           /* starting time */

    /* Return the elapsed time. */
    double elapsed(void) const { return (glfwGetTime() - start); }

    /* Increment the framerate count and return true if completed a period */
    bool nextframe(void) { return (++count % period == 0); }

    /* Return the framerate count. */
    double framerate(void) const {
        return (static_cast<double>(count) / elapsed());
    }

    /* Reset the framerate counter. */
    void reset(void) {
        count = 0;
        start = glfwGetTime();
    }

    /* Print the framerate count. */
    std::string to_string(void) const {
        const double fps = framerate();
        return ito::str::format("%llu frames @ %.2lf ms/frame (%.1lf fps)",
            count, 1000.0 / fps, fps);
    }

    /* Constructor/destructor. */
    explicit Timer(const size_t period) : period(period) { reset(); }
    ~Timer() = default;
}; /* Timer */

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_GRAPHICS_TIMER_H_ */
