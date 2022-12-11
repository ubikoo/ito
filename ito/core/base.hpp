/*
 * base.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_CORE_BASE_H
#define ITO_CORE_BASE_H

#include <iostream>
#include <sstream>
#include <exception>

/**
 * Stringify macros
 */
#define ito_strify(a)           #a
#define ito_strify_x(a)         ito_strify(a)

#define ito_concat2(a,b)        a ## b
#define ito_concat2_x(a,b)      ito_concat2(a,b)

#define ito_concat3(a,b,c)      a ## b ## c
#define ito_concat3_x(a,b,c)    ito_concat3(a,b,c)

#define ito_concat4(a,b,c,d)    a ## b ## c ## d
#define ito_concat4_x(a,b,c,d)  ito_concat4(a,b,c,d)

/**
 * @brief Include the AVX instruction set according to the macro __AVX__.
 * SSE vector registers are 128 bit wide (16 bytes), and able to pack 4 integers
 * (4 byte integer) or 2 doubles (8 byte double).
 * AVX vector registers are 256 bit wide (32 bytes), and able to pack 8 integers
 * (4 byte integer) or 4 doubles (8 byte double).
 * Data is transferred in chunks to/from vector registers. Data transfer is
 * more efficient if the memory addresses are a multiple of 16 bytes (SSE) or
 * 32 bytes(AVX). A boundary multiple of 16/32 bytes is 16/32 byte aligned.
 *
 * Alternatively, fail with error macro:
 *  #error AVX extended instruction set required.
 */
#ifdef __AVX__
#include <x86intrin.h>
#endif  /* __AVX__ */

/**
 * OpenMP headers and macros
 * @brief These functions are declared in the omp.h header file and have well defined
 * meanings in a non parallel environment. If _OPENMP is not defined, fallback
 * and elide the macro.
 */
#ifdef _OPENMP
#include <omp.h>
#define ito_pragma(a)           _Pragma(ito_strify(a))
#else   /* _OPENMP */
#define ito_pragma(a)
inline int omp_get_level(void) { return 0; }
inline int omp_get_thread_num(void) { return 0; }
inline int omp_get_num_threads(void) { return 1; }
inline int omp_get_max_threads(void) { return 1; }
#endif  /* _OPENMP */

/**
 * @brief Convenience GNU-C extension macros. Define the following extensions
 * if we're using GNUC. Otherwise, fallback and elide the corresponding macro.
 * Alternatively, fail with:
 *  #error GNU C extensions required.
 */
#ifdef __GNUC__
#define ito_inline              inline __attribute__((always_inline))
#define ito_aligned(key)        __attribute__((aligned(key)))
#define ito_unused(decl)        __attribute__((unused)) decl
#define ito_extension(decl)     __extension__ decl
#define ito_restrict(decl)      __restrict__ decl
#else
#define ito_inline              inline
#define ito_aligned(key)
#define ito_unused(decl)        decl
#define ito_extension(decl)     decl
#define ito_restrict(decl)      decl
#endif /* __GNUC__ */

/**
 * Error, throw and assert macros ---------------------------------------
 * @brief Encapsulate debug information - current file, current line, error
 * message - and throw an exception. Alternatively, inherit from class
 * std::runtime_error and create a custom exception handler.
 * Enclose the compound statement inside a do {...} while(0) to swallow the
 * semicolon and the macro in one statement.
 * @see https://en.cppreference.com/w/cpp/error/runtime_error
 *      https://stackoverflow.com/questions/8152720
 *      https://stackoverflow.com/questions/1569726
 */
#ifndef ito_throw
#define ito_throw(msg) \
do { \
    std::ostringstream ss; \
    ss << "\n" << __FILE__ \
        << ":" <<  __LINE__ \
        << ": " << (msg); \
    throw std::runtime_error(ss.str()); \
} while(0)
#endif /* ito_throw */

#ifndef ito_assert
#define ito_assert(expr,msg) \
do { \
    if (!(expr)) { \
        std::ostringstream ss; \
        ss << "\n" << __FILE__ \
           << ":" <<  __LINE__ \
           << ": " << (msg); \
        throw std::runtime_error(ss.str()); \
    } \
} while(0)
#endif /* ito_assert */

#endif /* ITO_CORE_BASE_H */
