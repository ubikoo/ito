/*
 * mesh.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_OPENGL_MESH_H_
#define ITO_OPENGL_MESH_H_

/** Assimp interface, scene data structure and post process flags. */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include "base.hpp"

namespace ito {
namespace gl {

/**
 * @brief Mesh maintains a shared-vertex representation of a mesh. It consists
 * of a collection of vertices and of a list of faces. Each face is specified
 * by a list of indices of the vertices forming its boundary.
 * Each vertex has a position, normal, color and texture coordinates.
 * Edges are implicit in the sequence of vertices composing the face.
 *
 * @par Geometry
 * A mesh with (n1 x n2) vertices along the first and second dimensions is
 * defined as an orientable manifold with topology:
 *
 *      (i, j+1)--------(i+1, j+1)
 *            | \      |
 *            |  \     |
 *            |   \    |
 *            |    \   |
 *            |     \  |
 *      (i, j)--------(i+1, j)
 *
 * where (0 <= i <= n1-1) and (0 <= j <= n2-1), resulting:
 *      (n1-1) * (n2-1) quads in a lattice with (n1 * n2) vertices,
 *      2 triangles per quad, or 2*(n1 - 1)*(n2 - 1) lattice triangles,
 *      3 indices per triangle, or 6*(n1 - 1)*(n2 - 1) triangle indices.
 *
 * @par Data structrure
 * Meshes are represented as a collection of vertices, edges and faces.
 * The mesh data structure maintains the mesh data:
 *  - geometry: vertex positions and normals.
 *  - attributes: vertex color and texture coordinates.
 *  - connectivity: adjacency relationships between mesh elements.
 *
 * @note: The data structure may optionally support a set of functions on the
 * data - rendering, geometry queries, and manipulation (remove or add a vertex,
 * vertex split, edge collapse, etc).
 *
 * @par Shared-vertex representation
 * Shared-vertex representations are compact and allow constant time complexity
 * O(1) lookups of the vertices of a given face.
 *
 * Indexed rendering requires a list of vertices of each face to be transmitted
 * to the GPU. The vertices are stored in a vertex buffer object holding the
 * position/color/normal structures.
 *
 * Changes in shape, but not geometry, can be dynamically updated by simply
 * resending the vertex data without updating the face connectivity.
 *
 * Shared-vertex representations are static and encode little information
 * regarding the structure of the mesh - traversals and mesh manipulation
 * are inefficient.
 *
 * @par Half-edge representation
 * Half-edge is a boundary representation that models explicitly the vertices,
 * edges, and faces of the mesh with additional adjacency information.
 * This permits a more efficient manipulation of the mesh - edge collapsing,
 * mesh traversal, adjacency queries, eg:
 *  - which faces use this vertex?
 *  - which edges use this vertex?
 *  - which faces border this edge?
 *  - which edges border this face?
 *  - which faces are adjacent to this face?
 *
 * @see OpenGL mesh and polygon file format(ply):
 *      https://learnopengl.com/Model-Loading/Mesh
 *      http://paulbourke.net/dataformats/ply
 *
 * @see Mesh data structures:
 *      https://www.redblobgames.com/x/1722-b-rep-triangle-meshes
 *      https://en.wikipedia.org/wiki/Polygon_mesh
 *      https://www.flipcode.com/archives/The_Half-Edge_Data_Structure.shtml
 *      https://towardsdatascience.com/mesh-data-structure-d8b1a61d749e
 *      https://martindevans.me/game-development/2016/03/30/
 *          Procedural-Generation-For-Dummies-Half-Edge-Geometry
 */
struct Mesh {
    /** -----------------------------------------------------------------------
     * @brief Vertex holds the geometry and attributes of a point in the mesh.
     */
    struct Vertex {
        GLfloat position[3];
        GLfloat normal[3];
        GLfloat color[3];
        GLfloat texcoord[2];
    };

    /**
     * @brief Face is represented by the indices of the vertices in the mesh.
     */
    struct Face {
        GLuint index[3];
    };

    /** -----------------------------------------------------------------------
     * Mesh member variables.
     */
    std::string name;                   /* mesh name */
    std::vector<Vertex> vertices;       /* vertex list */
    std::vector<Face> faces;            /* indexed face list */
    GLuint vao;                         /* vertex array object */
    GLuint vbo;                         /* vertex buffer object */
    GLuint ebo;                         /* element buffer object */

    /** -----------------------------------------------------------------------
     * @brief Create a grid with (n1 * n2) vertices.
     */
    static std::vector<Face> Grid(const size_t n1, const size_t n2);

    /** @brief Create a mesh. */
    static Mesh Create(
        const GLuint &program,
        const std::string &name,
        const std::vector<Vertex> &vertices,
        const std::vector<Face> &faces);

    /** @brief Destroy mesh objects. */
    static void Destroy(Mesh &mesh);

    /** @brief Update mesh vertex data on the gpu. */
    static void Update(const Mesh &mesh);

    /** @brief Render the mesh. */
    static void Render(const Mesh &mesh);

    /** @brief Create a plane represented by (n1 * n2) vertices. */
    static Mesh Plane(
        const GLuint &program,
        const std::string &name,
        size_t n1,
        size_t n2,
        GLfloat xlo,
        GLfloat xhi,
        GLfloat ylo,
        GLfloat yhi);

    /** @brief Create a mesh with (n1 * n2) vertices. */
    static Mesh Sphere(
        const GLuint &program,
        const std::string &name,
        size_t n1,
        size_t n2,
        GLfloat radius,
        GLfloat theta_lo,
        GLfloat theta_hi,
        GLfloat phi_lo,
        GLfloat phi_hi);

    /** @brief Load the model meshes from a specified filename. */
    static std::vector<Mesh> Load(
        const GLuint &program,
        const std::string &name,
        const std::string &filename);

    /** @brief Process an Assimp mesh and retrieve vertex and face data. */
    static bool Process(
        const aiMesh *mesh,
        std::vector<Mesh::Vertex> &vertices,
        std::vector<Mesh::Face> &faces);
};

} /* gl */
} /* ito */

#endif /* ITO_OPENGL_MESH_H_ */
