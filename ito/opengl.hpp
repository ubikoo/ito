/*
 * opengl.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_H_
#define ITO_OPENGL_H_

/**
 * @brief OpenGL/GLSL interface.
 * Interface to OpenGL API and OpenGL Objects. These are structures containing
 * state and responsible for transmitting data to and from the GPU:
 *  - Regular Objects contain data:
 *      Buffer Objects
 *      Renderbuffer Objects
 *      Texture Objects
 *      Query Objects
 *      Sampler Objects
 *  - Container Objects contain no data, they are containers with references to
 *  other objects:
 *      Framebuffer Objects
 *      Vertex Array Objects
 *      Transform Feedback Objects
 *      Program Pipeline Objects
 *  - Non-standard Objects do not follow standard conventions for OpenGL objets:
 *      Shader and Program Objects
 *      Sync Objects
 *
 * @brief Graphics interface.
 * Interface to the GLFW library. Handle the initialisation and termination of
 * the GLFW library, set/unset renderer event callbacks, etc.
 *
 * @note The interface is implemented in a data-oriented style for ease of use
 * and to avoid problems associated with the usage of an object-oriented style
 * to maintain OpenGL Objects - RAII, move semantics, etc.
 *
 * @see https://www.khronos.org/opengl/wiki/OpenGL_Object
 *      https://www.khronos.org/opengl/wiki/GLSL_Object
 *      https://www.khronos.org/opengl/wiki/Common_Mistakes
 */
#include "opengl/base.hpp"
#include "opengl/glfw.hpp"
#include "opengl/error.hpp"
#include "opengl/image.hpp"
#include "opengl/imageformat.hpp"
#include "opengl/mesh.hpp"
#include "opengl/timer.hpp"

#include "opengl/buffer.hpp"
#include "opengl/framebuffer.hpp"
#include "opengl/renderbuffer.hpp"
#include "opengl/texture.hpp"
#include "opengl/vertexarray.hpp"

#include "opengl/glsl/attribute.hpp"
#include "opengl/glsl/program.hpp"
#include "opengl/glsl/shader.hpp"
#include "opengl/glsl/uniform.hpp"
#include "opengl/glsl/variable.hpp"

#endif /* ITO_OPENGL_H_ */