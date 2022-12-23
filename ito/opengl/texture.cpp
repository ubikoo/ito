/*
 * texture.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "texture.hpp"
#include "imageformat.hpp"

namespace ito {
namespace gl {

/**
 * @brief Create a 1d texture with specified size and internal format.
 *
 * @par GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, and GL_TEXTURE_WRAP_R control
 * how texture coordinates are handled outside the range [0.0, 1.0].
 *
 * @par A texture is not mipmap complete if it expects the existence of mipmaps
 * and none is supplied. There are six minifying functions:
 *  GL_NEAREST, GL_LINEAR:
 *  GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST
 *  GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
 *
 * @par Only these types of textures are used for simplicity:
 *  GL_TEXTURE_1D: 1-dimensional textures with width, but no height or depth.
 *  GL_TEXTURE_2D: 2-dimensional textures with width and height but no depth.
 *  GL_TEXTURE_3D: 3-dimensional textures with width, height and depth.
 *  GL_TEXTURE_BUFFER: 1-dimensional texture with an attached buffer object.
 *
 * @see https://www.khronos.org/opengl/wiki/Texture
 */
GLuint CreateTexture1d(
    const GLint internalformat,
    const GLsizei width,
    const GLenum pixelformat,
    const GLenum pixeltype,
    const GLvoid *pixels)
{
    /* Check texture internal format and size dimensions. */
    ito_assert(IsValidTextureInternalformat(internalformat),
        "invalid texture internal format");
    ito_assert(width > 0, "invalid texture width");

    /* Generate a new texture object name and bind it to the target point. */
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_1D, texture);
    ito_assert(glIsTexture(texture), "failed to generate texture object");

    /*
     * Specifiy the texture data store with the pixel data. If pixels is null,
     * allocate memory only.
     */
    glTexImage1D(
        GL_TEXTURE_1D,
        0,                  /* level of detail - 0 is base image */
        internalformat,     /* internal format */
        width,              /* texture width */
        0,                  /* border parameter - must be 0 (legacy) */
        pixelformat,        /* format of the pixel data */
        pixeltype,          /* type of the pixel data */
        pixels);            /* pointer to the pixel data */

    /* Unbind the texture from the target point and return the handle. */
    glBindTexture(GL_TEXTURE_1D, 0);
    return texture;
}

/**
 * @brief Create a 2d texture with specified size and internal format.
 */
GLuint CreateTexture2d(
    const GLint internalformat,
    const GLsizei width,
    const GLsizei height,
    const GLenum pixelformat,
    const GLenum pixeltype,
    const GLvoid *pixels)
{
    /* Check texture internal format and size dimensions. */
    ito_assert(IsValidTextureInternalformat(internalformat),
        "invalid texture internal format");
    ito_assert(width > 0, "invalid texture width");
    ito_assert(height > 0, "invalid texture height");

    /* Generate a new texture object name and bind it to the target point. */
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    ito_assert(glIsTexture(texture), "failed to generate texture object");

    /*
     * Specifiy the texture data store with the pixel data. If pixels is null,
     * allocate memory only.
     */
    glTexImage2D(
        GL_TEXTURE_2D,
        0,                  /* level of detail - 0 is base image */
        internalformat,     /* internal format */
        width,              /* texture width */
        height,             /* texture height */
        0,                  /* border parameter - must be 0 (legacy) */
        pixelformat,        /* format of the pixel data */
        pixeltype,          /* type of the pixel data(GLubyte) */
        pixels);            /* pointer to the pixel data */

    /* Unbind the texture from the target point and return the handle. */
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

/**
 * @brief Create a 3d texture with specified size and internal format.
 */
GLuint CreateTexture3d(
    const GLint internalformat,
    const GLsizei width,
    const GLsizei height,
    const GLsizei depth,
    const GLenum pixelformat,
    const GLenum pixeltype,
    const GLvoid *pixels)
{
    /* Check texture internal format and size dimensions. */
    ito_assert(IsValidTextureInternalformat(internalformat),
        "invalid texture internal format");
    ito_assert(width > 0, "invalid texture width");
    ito_assert(height > 0, "invalid texture height");
    ito_assert(depth > 0, "invalid texture depth");

    /* Generate a new texture object name and bind it to the target point. */
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);
    ito_assert(glIsTexture(texture), "failed to generate texture object");

    /*
     * Specifiy the texture data store with the pixel data. If pixels is null,
     * allocate memory only.
     */
    glTexImage3D(
        GL_TEXTURE_3D,
        0,                  /* level of detail - 0 is base image */
        internalformat,     /* internal format */
        width,              /* texture width */
        height,             /* texture height */
        depth,              /* texture depth */
        0,                  /* border parameter - must be 0 (legacy) */
        pixelformat,        /* format of the pixel data */
        pixeltype,          /* type of the pixel data(GLubyte) */
        pixels);            /* pointer to the pixel data */

    /* Unbind the texture from the target point and return the handle. */
    glBindTexture(GL_TEXTURE_3D, 0);
    return texture;
}

/**
 * @brief Create a 1-dimensional texture and attach the storage for the buffer
 * object to the newly active buffer texture.
 *
 * @param buffer specifies the buffer object attached to the texture. If zero,
 * any buffer object attached to the buffer texture is detached. Otherwise it
 * must be the name of an existing buffer object.
 */
GLuint CreateTextureBuffer(GLint internalformat, GLuint buffer)
{
    ito_assert(IsValidTextureBufferInternalformat(internalformat),
        "invalid texture buffer internal format");
    ito_assert(glIsBuffer(buffer), "invalid texture buffer object");

    /* Generate a new texture object name and bind it to GL_TEXTURE_BUFFER. */
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_BUFFER, texture);
    ito_assert(glIsTexture(texture), "failed to generate texture object");

    /*
     * Attach the storage for the buffer object to the active buffer texture.
     * The parameter internalformat specifies the format of the texel array
     * in the buffer object.
     */
    glTexBuffer(GL_TEXTURE_BUFFER, internalformat, buffer);

    /* Unbind the texture from the target point and return the handle. */
    glBindTexture(GL_TEXTURE_BUFFER, 0);
    return texture;
}

/**
 * @brief Delete a texture object.
 */
void DestroyTexture(const GLuint &texture)
{
    glDeleteTextures(1, &texture);
}

/**
 * @brief Generate texture mipmaps and set the indices of the lowest
 * GL_TEXTURE_BASE_LEVEL and highest GL_TEXTURE_MAX_LEVEL mipmap levels.
 * Initially, GL_TEXTURE_BASE_LEVEL is 0 and GL_TEXTURE_MAX_LEVEL is 1000.
 */
void SetTextureMipmap(
    const GLenum target,
    const GLint base_level,
    const GLint max_level,
    const GLboolean generate)
{
    if (generate) {
        glGenerateMipmap(target);
    }
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, base_level);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, max_level);
}

/**
 * @brief Set texture minifying and magnifying functions.
 * The minifying function GL_TEXTURE_MIN_FILTER can be either:
 *      GL_NEAREST, GL_LINEAR,
 *      GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST,
 *      GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR.
 * The magnifying function GL_TEXTURE_MAG_FILTER can be either:
 *      GL_NEAREST, or GL_LINEAR,
 * Initially, GL_TEXTURE_MIN_FILTER and GL_TEXTURE_MAG_FILTER are set to
 * GL_NEAREST.
 */
void SetTextureFilter(
    const GLenum target,
    const GLint filter_min,
    const GLint filter_mag)
{
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter_mag);
}

/**
 * @brief Set wrap parameter for the texture coordinates GL_TEXTURE_WRAP_S,
 * GL_TEXTURE_WRAP_T and GL_TEXTURE_WRAP_R to either:
 *  GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT, or
 *  GL_MIRROR_CLAMP_TO_EDGE.
 * Initially, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T and GL_TEXTURE_WRAP_R are
 * set to GL_REPEAT.
 */
void SetTextureWrap(
    const GLenum target,
    const GLint wrap_s,
    const GLint wrap_t,
    const GLint wrap_r)
{
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, wrap_r);
}

/**
 * @brief Return the width for 0th level of detail of a texture currently bound
 * the specified target.
 */
GLsizei GetTextureWidth(const GLenum target)
{
    GLsizei width;
    glGetTexLevelParameteriv(
        target,
        0,                      /* 0th level-of-detail */
        GL_TEXTURE_WIDTH,
        (GLint *) &width);
    return width;
}

/**
 * @brief Return the height for 0th level of detail of a texture currently bound
 * the specified target.
 */
GLsizei GetTextureHeight(const GLenum target)
{
    GLsizei height;
    glGetTexLevelParameteriv(
        target,
        0,                      /* 0th level-of-detail */
        GL_TEXTURE_HEIGHT,
        (GLint *) &height);
    return height;
}

/**
 * @brief Return the depth for 0th level of detail of a texture currently bound
 * the specified target.
 */
GLsizei GetTextureDepth(const GLenum target)
{
    GLsizei depth;
    glGetTexLevelParameteriv(
        target,
        0,                      /* 0th level-of-detail */
        GL_TEXTURE_DEPTH,
        (GLint *) &depth);
    return depth;
}

/**
 * @brief Return the internal format for 0th level of detail of a texture
 * currently bound the specified target.
 */
GLint GetTextureInternalformat(const GLenum target)
{
    GLint internalformat;
    glGetTexLevelParameteriv(
        target,
        0,                      /* 0th level-of-detail */
        GL_TEXTURE_INTERNAL_FORMAT,
        &internalformat);
    return internalformat;
}

/**
 * @brief Is the texture internal format a valid GL texture format?
 */
bool IsValidTextureInternalformat(const GLint internalformat)
{
    if (!ImageFormat::Contains(internalformat)) {
        return false;
    }

    return (
        /* Texel normalized base internal formats */
        internalformat == GL_RED                ||
        internalformat == GL_RG                 ||
        internalformat == GL_RGB                ||
        internalformat == GL_RGBA               ||
        internalformat == GL_DEPTH_COMPONENT    ||
        internalformat == GL_DEPTH_STENCIL      ||
        /* Texel 8- and 16-bit normalized sized internal formats */
        internalformat == GL_R8                 ||
        internalformat == GL_RG8                ||
        internalformat == GL_RGB8               ||
        internalformat == GL_RGBA8              ||
        internalformat == GL_R16                ||
        internalformat == GL_RG16               ||
        internalformat == GL_RGB16              ||
        internalformat == GL_RGBA16             ||
        /* Texel 16- and 32-bit floating-point sized internal formats */
        internalformat == GL_R16F               ||
        internalformat == GL_RG16F              ||
        internalformat == GL_RGB16F             ||
        internalformat == GL_RGBA16F            ||
        internalformat == GL_R32F               ||
        internalformat == GL_RG32F              ||
        internalformat == GL_RGB32F             ||
        internalformat == GL_RGBA32F            ||
        /* Texel 8-, 16-, and 32-bit integer sized internal formats */
        internalformat == GL_R8I                ||
        internalformat == GL_RG8I               ||
        internalformat == GL_RGB8I              ||
        internalformat == GL_RGBA8I             ||
        internalformat == GL_R16I               ||
        internalformat == GL_RG16I              ||
        internalformat == GL_RGB16I             ||
        internalformat == GL_RGBA16I            ||
        internalformat == GL_R8UI               ||
        internalformat == GL_RG8UI              ||
        internalformat == GL_RGB8UI             ||
        internalformat == GL_RGBA8UI            ||
        internalformat == GL_R16UI              ||
        internalformat == GL_RG16UI             ||
        internalformat == GL_RGB16UI            ||
        internalformat == GL_RGBA16UI           ||
        /* Texel depth sized internal formats */
        internalformat == GL_DEPTH_COMPONENT16  ||
        internalformat == GL_DEPTH_COMPONENT24  ||
        internalformat == GL_DEPTH_COMPONENT32  ||
        internalformat == GL_DEPTH_COMPONENT32F ||
        internalformat == GL_DEPTH24_STENCIL8);
}

/**
 * @brief Is the texture internal format a valid GL texture format?
 */
bool IsValidTextureBufferInternalformat(const GLint internalformat)
{
    if (!ImageFormat::Contains(internalformat)) {
        return false;
    }

    return (
        /* 1-channel internal format */
        internalformat == GL_R8       ||
        internalformat == GL_R16      ||
        internalformat == GL_R16F     ||
        internalformat == GL_R32F     ||
        internalformat == GL_R8I      ||
        internalformat == GL_R16I     ||
        internalformat == GL_R32I     ||
        internalformat == GL_R8UI     ||
        internalformat == GL_R16UI    ||
        internalformat == GL_R32UI    ||
        /* 2-channel internal format */
        internalformat == GL_RG8      ||
        internalformat == GL_RG16     ||
        internalformat == GL_RG16F    ||
        internalformat == GL_RG32F    ||
        internalformat == GL_RG8I     ||
        internalformat == GL_RG16I    ||
        internalformat == GL_RG32I    ||
        internalformat == GL_RG8UI    ||
        internalformat == GL_RG16UI   ||
        internalformat == GL_RG32UI   ||
        /* 4-channel internal format */
        internalformat == GL_RGBA8    ||
        internalformat == GL_RGBA16   ||
        internalformat == GL_RGBA16F  ||
        internalformat == GL_RGBA32F  ||
        internalformat == GL_RGBA8I   ||
        internalformat == GL_RGBA16I  ||
        internalformat == GL_RGBA32I  ||
        internalformat == GL_RGBA8UI  ||
        internalformat == GL_RGBA16UI ||
        internalformat == GL_RGBA32UI);
}

/**
 * @brief Bind the texture to the target at the specified texture unit.
 */
void ActiveBindTexture(GLenum target, GLenum texunit, GLuint texture)
{
    ito_assert(
        target == GL_TEXTURE_1D ||
        target == GL_TEXTURE_2D ||
        target == GL_TEXTURE_3D,
        "invalid texture target");
    glActiveTexture(GL_TEXTURE0 + texunit);
    glBindTexture(target, texture);
}

/**
 * @brief Bind the texture to the target of the specified texture unit and
 * attach the buffer storage to the texture.
 */
void ActiveBindTextureBuffer(
    GLenum target,
    GLenum texunit,
    GLuint texture,
    GLenum internalformat,
    GLuint buffer)
{
    ito_assert(target == GL_TEXTURE_BUFFER, "invalid texture buffer target");
    glActiveTexture(GL_TEXTURE0 + texunit);
    glBindTexture(target, texture);
    glTexBuffer(target, internalformat, buffer);
}

} /* gl */
} /* ito */
