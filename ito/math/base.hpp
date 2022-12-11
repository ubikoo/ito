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

#ifndef ITO_MATH_BASE_H
#define ITO_MATH_BASE_H

#include <cmath>
#include <limits>
#include <type_traits>

#include "ito/core.hpp"

/**
 * @brief Vector data representation
 *  1x2 vector is interpreted as one 128-bit (2x64) memory references.
 *  1x3 vector is interpreted as one 256-bit (4x64) memory references
 *  with the last 64-bit double padded to zero.
 *  1x4 vector is interpreted as one 256-bit (4x64) memory references.
 *
 * @brief Matrix data representation
 *  2x2 matrix is interpreted as two 128-bit (2x64) memory references.
 *  3x3 matrix is interpreted as three 256-bit (4x64) memory references
 *  with the last 64-bit double padded to zero.
 *  4x4 matrix is interpreted as four 256-bit (4x64) memory references.
 *
 * @brief Aligned data types.
 * Define aligned data types to get the attribute to apply to the pointed-to
 * type, rather than the pointer itself.
 * With GNU C __attribute__ syntax, it may be necessary use a typedef to get
 * the attribute to apply to the pointed-to type, rather than to the pointer
 * itself. Set a 32-byte alignment as required by AVX instruction set.
 *
 * @see https://stackoverflow.com/questions/4421706
 *      https://stackoverflow.com/questions/36955576
 *      https://gamedev.stackexchange.com/questions/33142
 *      https://stackoverflow.com/questions/36211864
 *      https://gcc.gnu.org/onlinedocs/gcc-6.5.0/gcc/Common-Type-Attributes.html
 */
namespace ito {
namespace math {

typedef std::int8_t     align_int8_t    ito_aligned(32);
typedef std::uint8_t    align_uint8_t   ito_aligned(32);
typedef std::int16_t    align_int16_t   ito_aligned(32);
typedef std::uint16_t   align_uint16_t  ito_aligned(32);
typedef std::int32_t    align_int32_t   ito_aligned(32);
typedef std::uint32_t   align_uint32_t  ito_aligned(32);
typedef std::int64_t    align_int64_t   ito_aligned(32);
typedef std::uint64_t   align_uint64_t  ito_aligned(32);
typedef std::float_t    align_float_t   ito_aligned(32);
typedef std::double_t   align_double_t  ito_aligned(32);

} /* math */
} /* ito */

#endif /* ITO_MATH_BASE_H */
