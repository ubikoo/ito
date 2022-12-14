/*
 * mesh.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */


#include <string>
#include <vector>
#include <cmath>       /* sin, cos */
#include "buffer.hpp"
#include "vertexarray.hpp"
#include "glsl/program.hpp"
#include "glsl/attribute.hpp"
#include "mesh.hpp"

namespace ito {
namespace gl {

/**
 * @brief Create a grid defined by an indexed face topology on a lattice with
 * (n1 * n2) vertices along the first and second dimensions:
 *      (n1 - 1) * (n2 - 1) quads in a lattice with (n1 * n2) vertices,
 *      2 triangles per quad, or 2 * (n1 - 1)*(n2 - 1) lattice triangles,
 *      3 indices per triangle, or 6 * (n1 - 1)*(n2 - 1) triangle indices.
 *
 * The grid represents an orientable manifold with topology:
 *
 *      (i, j+1)--------(i+1, j+1)
 *            | \      |
 *            |  \     |
 *            |   \    |
 *            |    \   |
 *            |     \  |
 *      (i, j)--------(i+1, j)
 *
 *  where (0 <= i <= n1-1) and (0 <= j <= n2-1).
 *  The first triangle is represented by the vertices:
 *      {    (i, j), (i+1, j), (i, j+1)}
 *  The second triangle is represented by the vertices:
 *      {(i+1, j+1), (i, j+1), (i+1, j)}
 */
std::vector<Mesh::Face> Mesh::Grid(const size_t n1, const size_t n2)
{
    ito_assert(n1 > 1 && n2 > 1, "invalid mesh grid dimensions");

    std::vector<Mesh::Face> faces(2 * (n1 - 1) * (n2 - 1));

    for (size_t j = 0; j < n2-1; ++j) {
        for (size_t i = 0; i < n1-1; ++i) {
            size_t k, v0, v1, v2;

            /* first triangle(upward hypotenuse) */
            k = i + j * (n1 - 1);

            v0 = i     +     j * n1;
            v1 = (i+1) +     j * n1;
            v2 = i     + (j+1) * n1;

            faces[k].index[0] = v0;
            faces[k].index[1] = v1;
            faces[k].index[2] = v2;

            /* second triangle(downward hypotenuse) */
            k = i + j * (n1 - 1) + (n1 - 1) * (n2 - 1);

            v0 = (i+1) + (j+1) * n1;
            v1 =     i + (j+1) * n1;
            v2 = (i+1) +     j * n1;

            faces[k].index[0] = v0;
            faces[k].index[1] = v1;
            faces[k].index[2] = v2;
        }
    }

    return faces;
}

/**
 * @brief Create a mesh with a given name bound to a shader program object
 * from a list of vertices and faces.
 *
 * The mesh maintains two vertex buffer objects - vbo and ebo - holding the
 * vertex attributes and the vertex indices respectively. These are accessed
 * from the specified shader program object. The shader program object needs
 * to have three active vertex attributes
 *      position
 *      normal
 *      color
 *      texcoord
 *
 * regardless of whether these will be used in the shader. It is assumed that
 * every mesh has a name that prefixes the vertex attribute names, e.g. "ball".
 * The vertex attribute variables in the shader are then named accordingly as:
 *      ball_position
 *      ball_normal
 *      ball_color
 *      ball_texcoord
 */
Mesh Mesh::Create(
    const GLuint &program,
    const std::string &name,
    const std::vector<Vertex> &vertices,
    const std::vector<Face> &faces)
{
    /*
     * Create a new mesh and given name from a list of vertices and faces.
     */
    Mesh mesh;
    mesh.name = name;
    mesh.vertices = vertices;
    mesh.faces = faces;

    /*
     * Create vertex array object.
     */
    mesh.vao = CreateVertexArray();
    glBindVertexArray(mesh.vao);

    /*
     * Create buffer storage for the vertex data with layout:
     *  {(xyz)_0,
     *   (nor)_0,
     *   (rgb)_0,
     *    (uv)_0,
     *      ...
     *   (xyz)_n
     *   (nor)_n
     *   (rgb)_n
     *    (uv)_n}
     */
    GLsizeiptr vertex_data_size = mesh.vertices.size() * sizeof(Mesh::Vertex);
    mesh.vbo = CreateBuffer(
        GL_ARRAY_BUFFER,
        vertex_data_size,
        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,                /* target binding point */
        0,                              /* offset in data store */
        vertex_data_size,               /* data store size in bytes */
        mesh.vertices.data());          /* pointer to data source */

    /*
     * Create a buffer storage for the face indices with layout:
     *  {(v0,v1,v2)_0,
     *      ...
     *   v0,v1,v2)_n}
     */
    GLsizeiptr index_data_size = mesh.faces.size() * sizeof(Mesh::Face);
    mesh.ebo = CreateBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        index_data_size,
        GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,        /* target binding point */
        0,                              /* offset in data store */
        index_data_size,                /* data store size in bytes */
        mesh.faces.data());             /* pointer to data source */

    /*
     * Specify how OpenGL interprets the mesh vertex attributes.
     */
    EnableAttribute(program, mesh.name + std::string("_position"));
    AttributePointer(
        program,
        mesh.name + std::string("_position"),
        GL_FLOAT_VEC3,
        11 * sizeof(GLfloat),   /* byte offset between consecutive attributes */
        0,                      /* byte offset of first element in the buffer */
        false);                 /* normalized flag */

    /*
     * Specify how OpenGL interprets the mesh vertex attributes.
     */
    EnableAttribute(program, mesh.name + std::string("_normal"));
    AttributePointer(
        program,
        mesh.name + std::string("_normal"),
        GL_FLOAT_VEC3,
        11 * sizeof(GLfloat),   /* byte offset between consecutive attributes */
        3 * sizeof(GLfloat),    /* byte offset of first element in the buffer */
        false);                 /* normalized flag */

    EnableAttribute(program, mesh.name + std::string("_color"));
    AttributePointer(
        program,
        mesh.name + std::string("_color"),
        GL_FLOAT_VEC3,
        11 * sizeof(GLfloat),   /* byte offset between consecutive attributes */
        6 * sizeof(GLfloat),    /* byte offset of first element in the buffer */
        false);                 /* normalized flag */

    EnableAttribute(program, mesh.name + std::string("_texcoord"));
    AttributePointer(
        program,
        mesh.name + std::string("_texcoord"),
        GL_FLOAT_VEC2,
        11 * sizeof(GLfloat),   /* byte offset between consecutive attributes */
        9 * sizeof(GLfloat),    /* byte offset of first element in the buffer */
        false);                 /* normalized flag */

    /*
     * Unbind vertex array object.
     */
    glBindVertexArray(0);

    /*
     * Return mesh object
     */
    return mesh;
}

/**
 * @brief Destroy mesh objects.
 */
void Mesh::Destroy(Mesh &mesh)
{
    /* Destroy OpenGL data */
    DestroyBuffer(mesh.ebo);
    DestroyBuffer(mesh.vbo);
    DestroyVertexArray(mesh.vao);

    /* Destroy vertex data */
    mesh.vertices.clear();
    mesh.faces.clear();
}

/**
 * @brief Update mesh vertex data on the gpu.
 */
void Mesh::Update(const Mesh &mesh)
{
    GLsizeiptr vertex_data_size = mesh.vertices.size() * sizeof(Mesh::Vertex);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,            /* target binding point */
        0,                          /* offset in data store */
        vertex_data_size,           /* data store size in bytes */
        mesh.vertices.data());      /* pointer to data source */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * @brief Render the mesh. The number of elements to be rendered is the number
 * of vertex indices per primitive times the total number of primitives.
 */
void Mesh::Render(const Mesh &mesh)
{
    GLsizei n_elements = 3 * mesh.faces.size();
    glBindVertexArray(mesh.vao);
    glDrawElements(
        GL_TRIANGLES,           /* what kind of primitives to render */
        n_elements,             /* number of elements to be rendered */
        GL_UNSIGNED_INT,        /* type of the values in indices */
        (GLvoid *) 0);          /* offset of first index in the data array */
    glBindVertexArray(0);
}

/**
 * @brief Create a plane represented by (n1 * n2) vertices on a rectangle region
 * in the xy-plane, bounded by lower (xlo, ylo) and upper (xhi, yhi) positions.
 *
 * The vertex coordinates (x, y) are linearly interpolated between (xlo, ylo)
 * and (xhi, yhi) positions along the n1 and n2 lattice directions.
 * The normal of each vertex points in the (0,0,1) direction.
 * The vertex colors (r, g, b) map texture coordinates to a rg-colormap.
 * The vertex texture coordinates (u,v) are linearly interpolated between (0,0)
 * and (1,1).
 */
Mesh Mesh::Plane(
    const GLuint &program,
    const std::string &name,
    size_t n1,
    size_t n2,
    GLfloat xlo,
    GLfloat xhi,
    GLfloat ylo,
    GLfloat yhi)
{
    ito_assert(n1 > 1 && n2 > 1, "invalid mesh dimensions");
    ito_assert(xlo < xhi && ylo < yhi, "invalid coordinates");

    /* Create mesh vertices. */
    std::vector<Mesh::Vertex> vertices(n1 * n2);

    GLfloat dx = (xhi - xlo) / (GLfloat) (n1 - 1);
    GLfloat dy = (yhi - ylo) / (GLfloat) (n2 - 1);

    GLfloat du = 1.0f / (GLfloat) (n1 - 1);
    GLfloat dv = 1.0f / (GLfloat) (n2 - 1);

    for (size_t j = 0; j < n2; ++j) {
        for (size_t i = 0; i < n1; ++i) {
            size_t k = i + j * n1;

            /* Vertex positions are in the xy-plane by default. */
            vertices[k].position[0] = xlo + (GLfloat) i * dx;
            vertices[k].position[1] = ylo + (GLfloat) j * dy;
            vertices[k].position[2] = 0.0;

            /* Vertex normals point in the z-direction by default. */
            vertices[k].normal[0] = 0.0f;
            vertices[k].normal[1] = 0.0f;
            vertices[k].normal[2] = 1.0f;

            /* Encode the vertex colors with their uv-coordinates. */
            vertices[k].color[0] = (GLfloat) i * du;
            vertices[k].color[1] = (GLfloat) j * dv;
            vertices[k].color[2] = 0.0f;

            /* Vertex uv-coordinates lie in the unit square. */
            vertices[k].texcoord[0] = (GLfloat) i * du;
            vertices[k].texcoord[1] = (GLfloat) j * dv;
        }
    }

    /* Create mesh indexed face set. */
    std::vector<Mesh::Face> faces = Mesh::Grid(n1, n2);

    /* Create mesh. */
    return Mesh::Create(program, name, vertices, faces);
}

/**
 * @brief Create a mesh with (n1 * n2) vertices, mapping vertex positions onto a
 * region in a sphere with a specified radius and bounded by lower (theta_lo,
 * phi_lo) and upper (theta_hi, phi_hi) spherical coordinates.
 *
 * The vertex spherical coordinates are linearly interpolated between (theta_lo,
 * phi_lo) and (theta_hi, phi_hi) points along n1 and n2 lattice directions.
 * The normal of each vertex points in outward radial direction as defined by
 * its spherical coordinates.
 * The vertex colors (r, g, b) map the texture coordinates to a rg-colormap.
 * The vertex texture coordinates (u,v) are linearly interpolated between (0,0)
 * and (1,1).
 */
Mesh Mesh::Sphere(
    const GLuint &program,
    const std::string &name,
    size_t n1,
    size_t n2,
    GLfloat radius,
    GLfloat theta_lo,
    GLfloat theta_hi,
    GLfloat phi_lo,
    GLfloat phi_hi)
{
    ito_assert(n1 > 1 && n2 > 1, "invalid mesh dimensions");
    ito_assert(radius > 0.0, "invalid radius");
    ito_assert(theta_lo < theta_hi, "invalid polar angle");
    ito_assert(phi_lo < phi_hi, "invalid azimuth angle");

    /* Create mesh vertices. */
    std::vector<Mesh::Vertex> vertices(n1 * n2);

    GLfloat dtheta = (theta_hi - theta_lo) / (GLfloat) (n2 - 1);
    GLfloat dphi = (phi_hi - phi_lo) / (GLfloat) (n1 - 1);

    GLfloat du = 1.0f / (GLfloat) (n1 - 1);
    GLfloat dv = 1.0f / (GLfloat) (n2 - 1);

    for (size_t j = 0; j < n2; ++j) {
        for (size_t i = 0; i < n1; ++i) {
            size_t k = i + j * n1;

            /* Compute the spherical coordinates */
            GLfloat theta = theta_hi - (GLfloat) j * dtheta;
            GLfloat phi = phi_lo + (GLfloat) i * dphi;

            /* Vertex positions are just the normal scaled by the radius. */
            vertices[k].position[0] = radius * std::sin(theta) * std::cos(phi);
            vertices[k].position[1] = radius * std::sin(theta) * std::sin(phi);
            vertices[k].position[2] = radius * std::cos(theta);

            /* Vertex normals point in the z-direction by default. */
            vertices[k].normal[0] = std::sin(theta) * std::cos(phi);
            vertices[k].normal[1] = std::sin(theta) * std::sin(phi);
            vertices[k].normal[2] = std::cos(theta);

            /* Encode the vertex colors with their uv-coordinates. */
            vertices[k].color[0] = (GLfloat) i * du;
            vertices[k].color[1] = (GLfloat) j * dv;
            vertices[k].color[2] = 0.0f;

            /* Vertex uv-coordinates lie in the unit square. */
            vertices[k].texcoord[0] = (GLfloat) i * du;
            vertices[k].texcoord[1] = (GLfloat) j * dv;
        }
    }

    /* Create mesh indexed face set. */
    std::vector<Mesh::Face> faces = Mesh::Grid(n1, n2);

    /* Create mesh. */
    return Mesh::Create(program, name, vertices, faces);
}

/**
 * @brief Load the model meshes from a specified filename. The filename has a
 * format by Assimp and loads the model with the supported extensions.
 * If successful, processes each individual mesh in the Assimp scene and
 * retrieves the vertices and faces.
 *
 * @see OpenGL mesh and polygon file format(ply):
 *      https://learnopengl.com/Model-Loading/MModel
 *      http://paulbourke.net/dataformats/ply
 */
std::vector<Mesh> Mesh::Load(
    const GLuint &program,
    const std::string &name,
    const std::string &filename)
{
    /*
     * Load Assimp scene from the specified filename.
     * aiProcess_Triangulate ensures triangles are the model's only primitive.
     * aiProcess_GenSmoothNormals computes normal vectors for each vertex.
     */
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filename,
        aiProcess_Triangulate       |
        aiProcess_GenSmoothNormals  |
        aiProcess_CalcTangentSpace);
    ito_assert(scene != NULL, importer.GetErrorString());

    /* Initialize the meshes in the scene one by one. */
    std::vector<Mesh> meshes;
    for (size_t i = 0; i < scene->mNumMeshes; ++i) {
        std::vector<Mesh::Vertex> vertices;
        std::vector<Mesh::Face> faces;
        if (Mesh::Process(scene->mMeshes[i], vertices, faces)) {
            meshes.push_back(Mesh::Create(program, name, vertices, faces));
        }

    }
    return meshes;
}

/**
 * @brief Process an Assimp mesh and retrieve vertex and face data.
 */
bool Mesh::Process(
    const aiMesh *mesh,
    std::vector<Mesh::Vertex> &vertices,
    std::vector<Mesh::Face> &faces)
{
    /* Return if mesh is invalid. */
    if (mesh == nullptr) {
        return false;
    }

    /*
     * Process mesh vertices.
     */
    vertices.clear();
    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
        Mesh::Vertex vertex{};

        /* Mesh vertex positions */
        {
            vertex.position[0] = mesh->mVertices[i].x;
            vertex.position[1] = mesh->mVertices[i].y;
            vertex.position[2] = mesh->mVertices[i].z;
        }

        /* Mesh vertex normals */
        if (mesh->HasNormals()) {
            vertex.normal[0] = mesh->mNormals[i].x;
            vertex.normal[1] = mesh->mNormals[i].y;
            vertex.normal[2] = mesh->mNormals[i].z;
        }

        /*
         * Mesh vertex colors.
         * A mesh may contain 0 to AI_MAX_NUMBER_OF_COLOR_SETS vertex colors
         * per vertex. We assume we will not use models where a vertex has
         * multiple colors, so we always take the first set (0).
         */
        if (mesh->HasVertexColors(0)) {
            vertex.color[0] = mesh->mColors[0][i].r;
            vertex.color[1] = mesh->mColors[0][i].g;
            vertex.color[2] = mesh->mColors[0][i].b;
        }

        /*
         * Mesh vertex texture coordinates.
         * A mesh may contain 0 to AI_MAX_NUMBER_OF_TEXTURECOORDS per vertex.
         * We assume we will not use models where a vertex has multiple texture
         * coordinates, so we always take the first set (0).
         */
        if (mesh->HasTextureCoords(0)) {
            vertex.texcoord[0] = mesh->mTextureCoords[0][i].x;
            vertex.texcoord[1] = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    /*
     * Process mesh faces.
     */
    faces.clear();
    for (size_t i = 0; i < mesh->mNumFaces; ++i) {
        /* Return false if face num indices is invalid. */
        if (mesh->mFaces[i].mNumIndices != 3) {
            return false;
        }
        faces.push_back({
            mesh->mFaces[i].mIndices[0],
            mesh->mFaces[i].mIndices[1],
            mesh->mFaces[i].mIndices[2]});
    }

    return true;
}

} /* gl */
} /* ito */
