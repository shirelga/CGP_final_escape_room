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
#include "Texture.h"

typedef OpenMesh::TriMesh_ArrayKernelT<> mesh;


class Mesh{
public:
    Mesh();
    ~Mesh();
    bool init(std::string filename, std::string textures[], int num_of_tex,
                  const glm::vec3 &center, bool triangle_adjacency);
    void draw();
    inline const glm::mat4 get_model()
    {
        return _model;
    }
    inline void set_adjacency(const bool state)
    {
        _triangle_adjacency = state;
    };

    void scale(const glm::vec3& v);
    void translate(const glm::vec3& v);
    void rotate(const float angle);

private:
    mesh _meshObj;
    glm::vec3 _boundingBox[2];
    std::vector<float> _vertices;
    std::vector<float> _normals;
    std::vector<float> _tex_cords;
    std::vector<int> _indeces;
    GLuint _vao, _vbo[4];
    bool _triangle_adjacency;
    glm::mat4 _model;
    glm::mat4 _translation;
    glm::mat4 _rotation;
    glm::mat4 _scale;
    glm::vec3 _center;
    Texture* tex;

private:
    bool load(std::string& filename);
    void computeBoundingBox();
    void computeVerticesAndNormals();
    void findAdjacencies();
    void bind_vao_vbo();
    void normalizeVector(glm::vec3 &vec, bool range);
};


#endif //FINAL_PROJECT_MESH_H
