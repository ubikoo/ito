/*
 * imageformat.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "imageformat.hpp"

namespace ito {
namespace gl {

/**
 * @brief ImageFormat maintains a map of pairs keyed by the internal format.
 * Each pair contains internal format pixel base format and pixel data type:
 *
 *  {internalformat, {baseformat, datatype}}
 *
 *  @par {internalformat} is determined by:
 *  - Internal formats are divided into three groups:
 *      normalized (mapped into the range [0,1]),
 *      floating point,
 *      integer(signed and unsigned)
 *  - Normalized base internal formats:
 *      GL_RED, GL_RG, GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT and GL_DEPTH_STENCIL.
 *  - Normalized sized internal formats:
 *      GL_R8, GL_RG8, GL_RGB8, GL_RGBA8,
 *      GL_R16, GL_RG16, GL_RGB16, GL_RGBA16
 *  - Floating point sized internal formats:
 *      GL_R16F, GL_RG16F, GL_RGB16F, GL_RGBA16F,
 *      GL_R32F, GL_RG32F, GL_RGB32F, GL_RGBA32F
 *  - Integer (signed and unsigned) sized internal formats:
 *      GL_R8I, GL_RG8I, GL_RGB8I, GL_RGBA8I,
 *      GL_R16I, GL_RG16I, GL_RGB16I, GL_RGBA16I,
 *      GL_R8UI, GL_RG8UI, GL_RGB8UI, GL_RGBA8UI,
 *      GL_R16UI, GL_RG16UI, GL_RGB16UI, GL_RGBA16UI,
 *  - Depth sized internal formats:
 *      GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32.
 *
 *  @par {baseformat} is determined by:
 *      GL_RED, GL_RG,
 *      GL_RGB, GL_BGR,
 *      GL_RGBA, GL_BGRA,
 *      GL_RED_INTEGER, GL_RG_INTEGER,
 *      GL_RGB_INTEGER, GL_BGR_INTEGER,
 *      GL_RGBA_INTEGER, GL_BGRA_INTEGER,
 *      GL_DEPTH_COMPONENT,
 *      GL_DEPTH_STENCIL.
 *  - OpenGL converts each element to floating point and clamps to [0,1].
 *  Depth component in GL_DEPTH_STENCIL is interpreted as in GL_DEPTH_COMPONENT.
 *  Stencil component in GL_DEPTH_STENCIL depends on depth + stencil format.
 *
 *  @par {datatype} is determined by:
 *      GL_BYTE, GL_UNSIGNED_BYTE,
 *      GL_SHORT, GL_UNSIGNED_SHORT,
 *      GL_INT, GL_UNSIGNED_INT,
 *      GL_HALF_FLOAT, GL_FLOAT.
 *
 *  @par These formats are required for textures and renderbuffers. Any of the
 *  combinations presented in each row is a required format.
 *      Base format         Data type               Bitdepth per component
 *      RGBA, RG, RED       unsigned normalized     8, 16
 *      RGBA, RG, RED       float                   16, 32
 *      RGBA, RG, RED       signed integral         8, 16, 32
 *      RGBA, RG, RED       unsigned integral       8, 16, 32
 *
 *  @par These formats are required for textures and renderbuffers:
 *      GL_DEPTH_COMPONENT16
 *      GL_DEPTH_COMPONENT24
 *      GL_DEPTH_COMPONENT32F
 *      GL_DEPTH24_STENCIL8
 *      GL_DEPTH32F_STENCIL8
 *
 * @par It is more accurate to use a sized internal format. If the internal
 * format is a base internal format, the GL stores the texture with internal
 * component resolutions of its own choosing.
 *
 * @see https://www.khronos.org/opengl/wiki/Image_Format
 */
namespace ImageFormat {

static const std::map<GLint, std::pair<GLenum, GLenum>> FormatMap = {
    /* Texel normalized base internal formats */
    {GL_RED,                {GL_RED,                GL_UNSIGNED_BYTE}},
    {GL_RG,                 {GL_RG,                 GL_UNSIGNED_BYTE}},
    {GL_RGB,                {GL_RGB,                GL_UNSIGNED_BYTE}},
    {GL_RGBA,               {GL_RGBA,               GL_UNSIGNED_BYTE}},
    {GL_DEPTH_COMPONENT,    {GL_DEPTH_COMPONENT,    GL_UNSIGNED_BYTE}},
    {GL_DEPTH_STENCIL,      {GL_DEPTH_STENCIL,      GL_UNSIGNED_BYTE}},
    /* Texel 8- and 16-bit normalized sized internal formats */
    {GL_R8,                 {GL_RED,                GL_UNSIGNED_BYTE}},
    {GL_RG8,                {GL_RG,                 GL_UNSIGNED_BYTE}},
    {GL_RGB8,               {GL_RGB,                GL_UNSIGNED_BYTE}},
    {GL_RGBA8,              {GL_RGBA,               GL_UNSIGNED_BYTE}},
    {GL_R16,                {GL_RED,                GL_UNSIGNED_SHORT}},
    {GL_RG16,               {GL_RG,                 GL_UNSIGNED_SHORT}},
    {GL_RGB16,              {GL_RGB,                GL_UNSIGNED_SHORT}},
    {GL_RGBA16,             {GL_RGBA,               GL_UNSIGNED_SHORT}},
    /* Texel 16- and 32-bit floating-point sized internal formats */
    {GL_R16F,               {GL_RED,                GL_HALF_FLOAT}},
    {GL_RG16F,              {GL_RG,                 GL_HALF_FLOAT}},
    {GL_RGB16F,             {GL_RGB,                GL_HALF_FLOAT}},
    {GL_RGBA16F,            {GL_RGBA,               GL_HALF_FLOAT}},
    {GL_R32F,               {GL_RED,                GL_FLOAT}},
    {GL_RG32F,              {GL_RG,                 GL_FLOAT}},
    {GL_RGB32F,             {GL_RGB,                GL_FLOAT}},
    {GL_RGBA32F,            {GL_RGBA,               GL_FLOAT}},
    /* Texel 8-, 16-, and 32-bit integer sized internal formats */
    {GL_R8I,                {GL_RED,                GL_BYTE}},
    {GL_RG8I,               {GL_RG,                 GL_BYTE}},
    {GL_RGB8I,              {GL_RGB,                GL_BYTE}},
    {GL_RGBA8I,             {GL_RGBA,               GL_BYTE}},
    {GL_R16I,               {GL_RED,                GL_SHORT}},
    {GL_RG16I,              {GL_RG,                 GL_SHORT}},
    {GL_RGB16I,             {GL_RGB,                GL_SHORT}},
    {GL_RGBA16I,            {GL_RGBA,               GL_SHORT}},
    {GL_R32I,               {GL_RED,                GL_INT}},
    {GL_RG32I,              {GL_RG,                 GL_INT}},
    {GL_RGB32I,             {GL_RGB,                GL_INT}},
    {GL_RGBA32I,            {GL_RGBA,               GL_INT}},
    {GL_R8UI,               {GL_RED,                GL_UNSIGNED_BYTE}},
    {GL_RG8UI,              {GL_RG,                 GL_UNSIGNED_BYTE}},
    {GL_RGB8UI,             {GL_RGB,                GL_UNSIGNED_BYTE}},
    {GL_RGBA8UI,            {GL_RGBA,               GL_UNSIGNED_BYTE}},
    {GL_R16UI,              {GL_RED,                GL_UNSIGNED_SHORT}},
    {GL_RG16UI,             {GL_RG,                 GL_UNSIGNED_SHORT}},
    {GL_RGB16UI,            {GL_RGB,                GL_UNSIGNED_SHORT}},
    {GL_RGBA16UI,           {GL_RGBA,               GL_UNSIGNED_SHORT}},
    {GL_R32UI,              {GL_RED,                GL_UNSIGNED_INT}},
    {GL_RG32UI,             {GL_RG,                 GL_UNSIGNED_INT}},
    {GL_RGB32UI,            {GL_RGB,                GL_UNSIGNED_INT}},
    {GL_RGBA32UI,           {GL_RGBA,               GL_UNSIGNED_INT}},
    /* Texel depth sized internal formats */
    {GL_DEPTH_COMPONENT16,  {GL_DEPTH_COMPONENT,    GL_UNSIGNED_SHORT}},
    {GL_DEPTH_COMPONENT24,  {GL_DEPTH_COMPONENT,    GL_UNSIGNED_INT}},
    {GL_DEPTH_COMPONENT32,  {GL_DEPTH_COMPONENT,    GL_UNSIGNED_INT}},
    {GL_DEPTH_COMPONENT32F, {GL_DEPTH_COMPONENT,    GL_FLOAT}},
    {GL_DEPTH24_STENCIL8,   {GL_DEPTH_COMPONENT,    GL_UNSIGNED_INT_24_8}},
};

/**
 * @brief Does the map contain the OpenGL image format?
 */
bool Contains(const GLint internalformat)
{
    return (FormatMap.find(internalformat) != FormatMap.end());
}

/**
 * @brief Return the pixel base format of the OpenGL image format.
 */
GLenum BaseFormat(const GLint internalformat)
{
    if (!Contains(internalformat)) {
        return GL_NONE;
    }
    return FormatMap.at(internalformat).first;
}

/**
 * @brief Return the pixel data type of the OpenGL image format.
 */
GLenum DataType(const GLint internalformat)
{
    if (!Contains(internalformat)) {
        return GL_NONE;
    }
    return FormatMap.at(internalformat).second;
}

/**
 * @brief Return the size in bytes of the OpenGL image format.
 */
GLsizei DataSize(const GLint internalformat)
{
    size_t type = DataType(internalformat);
    return (
        type == GL_BYTE              ? sizeof(GLbyte)   :
        type == GL_SHORT             ? sizeof(GLshort)  :
        type == GL_INT               ? sizeof(GLint)    :
        type == GL_UNSIGNED_BYTE     ? sizeof(GLubyte)  :
        type == GL_UNSIGNED_SHORT    ? sizeof(GLushort) :
        type == GL_UNSIGNED_INT      ? sizeof(GLuint)   :
        type == GL_HALF_FLOAT        ? sizeof(GLhalf)   :
        type == GL_FLOAT             ? sizeof(GLfloat)  :
        type == GL_UNSIGNED_INT_24_8 ? sizeof(GLuint)   : 0);
}

/**
 * @brief Return the number of components of the OpenGL image format.
 */
GLsizei Components(const GLint internalformat)
{
    size_t format = BaseFormat(internalformat);
    return (
        format == GL_RED             ? 1 :
        format == GL_RG              ? 2 :
        format == GL_RGB             ? 3 :
        format == GL_RGBA            ? 4 :
        format == GL_DEPTH_COMPONENT ? 1 :
        format == GL_DEPTH_STENCIL   ? 2 : 0);
}

} /* ImageFormat */

} /* gl */
} /* ito */
