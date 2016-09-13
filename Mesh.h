//
// Created by Shirel Gazit on 21/08/2016.
//

#ifndef FINAL_PROJECT_MESH_H
#define FINAL_PROJECT_MESH_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"

typedef OpenMesh::TriMesh_ArrayKernelT<> mesh;


class Mesh {
public:
    Mesh();
    ~Mesh();
    bool init(std::string filename, const glm::vec3& center, bool triangle_adjacency = false);
    void draw(GLuint program);
    void findAdjacencies();
    inline glm::mat4 get_model()
    {
        return _model;
    }
    inline void set_adjacency(const bool state)
    {
        _triangle_adjacency = state;
    };

private:
    mesh _meshObj;
    glm::vec3 _boundingBox[2];
    std::vector<float> _vertices;
    std::vector<float> _vertices_adjacency;
    std::vector<float> _normals;
    GLuint _vao, _vbo[3];
    bool _triangle_adjacency;
    glm::mat4 _model;
    glm::vec3 _center;

private:
    bool load(std::string& filename);
    void computeBoundingBox();
    void computeVerticesAndNormals();
    void bind_vao_vbo();
    void normalizeVector(glm::vec3& vec);
};


#endif //FINAL_PROJECT_MESH_H
