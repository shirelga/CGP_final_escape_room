//
// Created by Shirel Gazit on 21/08/2016.
//

#include "Mesh.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh():_model(1.f), _translation(1.f), _rotation(1.f), _scale(1.f), tex(nullptr){ }

Mesh::~Mesh() {

    if(tex != nullptr)
    {
        delete tex;
    }
    if(_vbo != NULL)
    {
        glDeleteBuffers(2, _vbo);
    }

    if(_vao != 0)
    {
        glDeleteVertexArrays(1, &_vao);
    }
}

bool Mesh::load(std::string &filename) {

    if(!OpenMesh::IO::read_mesh(_meshObj, filename))
    {
        std::cerr << "Fail to load Mesh" << std::endl;
        return false;
    }

    return true;
}

void Mesh::computeBoundingBox() {

    mesh::VertexIter vertexIter;
    mesh::Point p, center = mesh::Point(0.f);
    const float fm = std::numeric_limits<float>::max();
    mesh::Point lowerLeft(fm, fm, fm);
    mesh::Point upperRight(-fm,-fm,-fm);
    vertexIter = _meshObj.vertices_begin();
    lowerLeft = upperRight = _meshObj.point(vertexIter);
    // This is how to go over all the vertices_rgb in the mesh:
    int vNum = _meshObj.n_vertices();
    for (vertexIter = _meshObj.vertices_begin();vertexIter != _meshObj.vertices_end();++vertexIter){
        // This is how to get the extrema associated with the set of vertices_rgb:
        p = _meshObj.point(vertexIter);
        center += p;
        for (int i = 0;i < 3;i++) {
            lowerLeft[i] = std::min(lowerLeft[i], p[i]);
            upperRight[i] = std::max(upperRight[i],p[i]);
        }
    }
    center /= vNum;
    _boundingBox[0] = glm::vec3(lowerLeft[0], lowerLeft[1], lowerLeft[2]);
    _boundingBox[1] = glm::vec3(upperRight[0], upperRight[1], upperRight[2]);
    _center = glm::vec3(center[0], center[1], center[2]);
    normalizeVector(_boundingBox[0], false);
    normalizeVector(_boundingBox[1], false);
    normalizeVector(_center, false);
}

void Mesh::computeVerticesAndNormals() {

    glm::vec3 diff = _boundingBox[1] - _boundingBox[0];

    if(_triangle_adjacency)
    {
        findAdjacencies();
    }
    else
    {
        /* face iterator */
        mesh::FaceIter fIter;
        mesh::FaceHandle fHandle;
        mesh::VertexHandle vHandle;
        /* face vertex iterator: this iterator goes through all the vertices_rgb which belong to the
       face */
        /* This special type of iterator is called circulator in OpenMesh terms */
        /* There are many types of circulators such as VertexVertex or FaceEdge VertexFace etc.
       */
        mesh::FaceVertexIter fvIter;
        /* go over all the faces */
        for (fIter = _meshObj.faces_begin(); fIter != _meshObj.faces_end(); ++fIter) {
            fHandle = *fIter;
            /* for each face - go over all the vertices_rgb that belong to it and compute their
           average position (center) of face */
            /* notice the termination condition of this iterator */
            for (fvIter = _meshObj.fv_iter(fHandle); fvIter; ++fvIter) {

                _indeces.push_back((*fvIter).idx());
            }
        }
    }
}

void Mesh::normalizeVector(glm::vec3 &vec, bool range) {
    if(range)
    {
        vec = ((vec - _boundingBox[0]) / (_boundingBox[1] - _boundingBox[0]));
        return;
    }
    vec = ((vec - _boundingBox[0]) / (_boundingBox[1] - _boundingBox[0])) * 2.f - 1.f;
}

void Mesh::bind_vao_vbo() {
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Create and load vertex data into a Vertex Buffer Object:
    glGenBuffers(4, _vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), &_vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, // attribute handle
                          4,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE,
                          0,
                          0);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(float), &_normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, // attribute handle
                          4,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE,
                          0,
                          0);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, _tex_cords.size() * sizeof(float), &_tex_cords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, // attribute handle
                          3,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE,
                          0,
                          0);

//    glGenBuffers(1,  &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indeces[0]) * _indeces.size(), &_indeces[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}


void Mesh::findAdjacencies() {
    for(mesh::FaceIter fIt = _meshObj.faces_begin(); fIt != _meshObj.faces_end(); ++fIt) {
        mesh::FaceHalfedgeIter feIt = _meshObj.fh_iter(*fIt);
        for (; feIt; ++feIt) {
            mesh::HalfedgeHandle heHandle = *feIt;
            //The vertex that the halfedge point to
            mesh::VertexHandle vHandle = _meshObj.to_vertex_handle(heHandle);

            //calculate the neighbour vertex
            mesh::VertexHandle nvHandle = _meshObj.opposite_he_opposite_vh(heHandle);

            int vIdx = vHandle.idx();
            int nvIdx = nvHandle.idx();

            _indeces.push_back(vIdx);
            _indeces.push_back(nvIdx);
        }
    }
}



bool Mesh::init(std::string filename, std::string textures[], int num_of_tex,
                const glm::vec3 &center, bool triangle_adjacency) {

    if(!load(filename))
    {
        return false;
    }

    tex = new Texture(textures, num_of_tex);
    tex->load();
    _triangle_adjacency = triangle_adjacency;

    computeBoundingBox();

    //add normal attribute to the mesh and calculate it.
    _meshObj.request_face_normals();
    _meshObj.request_vertex_normals();
    _meshObj.update_normals();


    for(mesh::VertexIter vi = _meshObj.vertices_begin(); vi != _meshObj.vertices_end(); vi++)
    {
        glm::vec3 vertex;
        glm::vec3 normal;
        glm::vec3 tex;

        {
            mesh::Point v = _meshObj.point(*vi);
            mesh::Point n = _meshObj.calc_vertex_normal(*vi);
            vertex = glm::vec3(v[0], v[1], v[2]);
            normal = glm::vec3(n[0], n[1], n[2]);
            normalizeVector(vertex, false);
            normalizeVector(normal, false);
        }


        {
            _vertices.push_back(vertex.x);
            _vertices.push_back(vertex.y);
            _vertices.push_back(vertex.z);
            _vertices.push_back(1.f);
            _normals.push_back(normal.x);
            _normals.push_back(normal.y);
            _normals.push_back(normal.z);
            _normals.push_back(1.f);
            _tex_cords.push_back(_center.x - vertex.x);
            _tex_cords.push_back(_center.y - vertex.y);
            _tex_cords.push_back(_center.z - vertex.z);
        }
    }
    _meshObj.update_normals();
    _meshObj.release_face_normals();
    _meshObj.release_vertex_normals();
    computeVerticesAndNormals();
    bind_vao_vbo();

    translate(center);
    _center += center;
    return true;
}

void Mesh::draw() {

    GLenum  topology = _triangle_adjacency ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(_vao);

    if(tex != nullptr)
    {
        tex->bind(GL_TEXTURE2);
    }
    glDrawElements(topology, _indeces.size(), GL_UNSIGNED_INT, (GLvoid*)(0));

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE0, 0);

}

void Mesh::translate(const glm::vec3 &v) {
    _translation = glm::translate(_translation, v);
    _model =  _translation * _rotation * _scale;
}

void Mesh::scale(const glm::vec3 &v) {
    _scale = glm::scale(_scale, v);
    _model =  _translation * _rotation * _scale;
}

void Mesh::rotate(const float angle) {
    _rotation = glm::rotate(_rotation, glm::radians(angle), glm::vec3(0, 1, 0));
    _model =  _translation * _rotation * _scale;
}
