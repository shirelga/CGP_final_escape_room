//
// Created by Shirel Gazit on 21/08/2016.
//

#include "Mesh.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh():_model(1.f){ }

Mesh::~Mesh() {
    if(_vbo != NULL)
    {
        glDeleteBuffers(3, _vbo);
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
    mesh::Point p, center;
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
    _boundingBox[0] = glm::vec3(lowerLeft[0], lowerLeft[1], lowerLeft[2]);
    _boundingBox[1] = glm::vec3(upperRight[0], upperRight[1], upperRight[2]);
    _center = glm::vec3(center[0], center[1], center[2]);
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
//                vHandle = *fvIter;
//                mesh::Point vertex = _meshObj.point(fvIter);
//                glm::vec3 v = glm::vec3(vertex[0], vertex[1], vertex[2]);
//                normalizeVector(v);
//                //sort out vertices_rgb
//                for (int i = 0; i < 3; i++) {
//                    _vertices.push_back(v[i]);
//                }
//                _vertices.push_back(1.f);

//                OpenMesh::VectorT<float, 3> meshNorm;
//                meshNorm = _meshObj.calc_vertex_normal(vHandle);
//                glm::vec3 n = glm::vec3(meshNorm[0], meshNorm[1], meshNorm[2]);
//                normalizeVector(n);
//                for (int i = 0; i < 3; i++) {
//                    _normals.push_back(n[i]);
//                }
//                _normals.push_back(0.f);
            }
        }
    }
}

void Mesh::normalizeVector(glm::vec3 &vec) {
    vec = ((vec - _boundingBox[0]) / (_boundingBox[1] - _boundingBox[0])) * 2.f - 1.f;
}

void Mesh::bind_vao_vbo() {
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Create and load vertex data into a Vertex Buffer Object:
    glGenBuffers(3, _vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), &_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(float), &_normals[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indeces[0]) * _indeces.size(), &_indeces[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::findAdjacencies() {
    int count = 0;
    for(mesh::FaceIter fIt = _meshObj.faces_begin(); fIt != _meshObj.faces_end(); fIt++) {
        std::cout << ++count << ":" << std::endl;
        for (mesh::FaceHalfedgeIter feIt = _meshObj.fh_iter(*fIt); feIt; feIt++) {
            mesh::HalfedgeHandle heHandle = *feIt;
            //The vertex that the halfedge point to
            mesh::VertexHandle vHandle = _meshObj.to_vertex_handle(heHandle);

            //calculate the neighbour vertex
            mesh::VertexHandle nvHandle = _meshObj.opposite_he_opposite_vh(heHandle);
//            {
//                //the opposite halfedge
//                mesh::HalfedgeHandle ovHeHandle = _meshObj.opposite_halfedge_handle(heHandle);
//                //the prev halfedge to the opposite
//                mesh::HalfedgeHandle povHeHandle = _meshObj.prev_halfedge_handle(ovHeHandle);
//                //the opposite halfedge to the prev
//                mesh::HalfedgeHandle pvHeHandle = _meshObj.opposite_halfedge_handle(povHeHandle);
//                //the neighbour vertex in the next face refrenced from above
//                nvHandle = _meshObj.to_vertex_handle(pvHeHandle);
//            }

            _indeces.push_back((vHandle).idx());
            _indeces.push_back((nvHandle).idx());
//            mesh::Point v = _meshObj.point(vHandle);
//            mesh::Point nv = _meshObj.point(nvHandle);
//            {
//                _vertices_adjacency.push_back(v[0]);
//                _vertices_adjacency.push_back(v[1]);
//                _vertices_adjacency.push_back(v[2]);
//                _vertices_adjacency.push_back(1.f);
//                _vertices_adjacency.push_back(nv[0]);
//                _vertices_adjacency.push_back(nv[1]);
//                _vertices_adjacency.push_back(nv[2]);
//                _vertices_adjacency.push_back(1.f);
//            }
//
//            {
//                OpenMesh::VectorT<float, 3> meshNorm;
//                meshNorm = _meshObj.calc_vertex_normal(vHandle);
//                glm::vec3 n = glm::vec3(meshNorm[0], meshNorm[1], meshNorm[2]);
//                normalizeVector(n);
//                for (int i = 0; i < 3; i++) {
//                    _normals.push_back(n[i]);
//                }
//                _normals.push_back(0.f);
//            }

            std::cout << vHandle.idx() << std::endl <<  nvHandle.idx() << std::endl;

        }
        std::cout<<std::endl<<std::endl;
    }
}



bool Mesh::init(std::string filename, const glm::vec3& center,bool triangle_adjacency) {

    if(!load(filename))
    {
        return false;
    }

    _triangle_adjacency = triangle_adjacency;
//    _model *= glm::vec4(center, 1.f);

    computeBoundingBox();

    //add normal attribute to the mesh and calculate it.
    _meshObj.request_face_normals();
    _meshObj.request_vertex_normals();
    _meshObj.update_normals();

    for(mesh::VertexIter vi = _meshObj.vertices_begin(); vi != _meshObj.vertices_end(); vi++)
    {
        glm::vec3 vertex;
        glm::vec3 normal;

        {
            mesh::Point v = _meshObj.point(*vi);
            mesh::Point n = _meshObj.calc_vertex_normal(*vi);

            vertex = glm::vec3(v[0], v[1], v[2]);
            normal = glm::vec3(n[0], n[1], n[2]);
            normalizeVector(vertex);
            normalizeVector(normal);
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
        }
    }

    _meshObj.release_face_normals();
    _meshObj.release_vertex_normals();

    computeVerticesAndNormals();
    bind_vao_vbo();

    glm::vec3 diff = center - center;
    _model = glm::translate(_model, diff);
    std::cout<<glm::to_string(center + diff)<<std::endl;
    return true;
}

void Mesh::draw(GLuint program) {

    GLenum  topology = _triangle_adjacency ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;

    int stride = _triangle_adjacency ? 0 : 0;
    GLuint vbo = _triangle_adjacency ? _vbo[2] : _vbo[0];
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    GLint _posAttrib = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(_posAttrib);
    glVertexAttribPointer(_posAttrib, // attribute handle
                          4,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE,
                          0,
                          0);
//    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
    GLint _fillColorUV = glGetAttribLocation(program, "normal");
    glEnableVertexAttribArray(_fillColorUV);
    glVertexAttribPointer(_fillColorUV, // attribute handle
                          4,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE,
                          0,
                          0);
    glDrawElements(topology, _indeces.size(), GL_UNSIGNED_INT, (GLvoid*)(0));
//    glDrawArrays(GL_TRIANGLES_ADJACENCY, 0, static_cast<GLsizei>(_meshObj.n_faces() * 3));
    glBindVertexArray(0);

}
