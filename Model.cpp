//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "glm/gtc/matrix_transform.hpp"

#ifdef __APPLE__
    #define SHADERS_DIR "../shaders/"
#else
    #define SHADERS_DIR "shaders/"
#endif

#define ROWS 100
#define COLS 100
#define MATRIX_ENTRY_1D(row, col) (((row) * COLS) + col)

Model::Model() : _translation(1.f), _rotatation(1.f), _scale(1.f), _model(1.f), _vao(0), _ibo(0), tex(
        nullptr)
{

}

Model::~Model()
{
    if(tex != nullptr)
    {
        delete tex;
    }
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
    if (_vbo[0] != 0) glDeleteBuffers(2, _vbo);
    if (_ibo != 0) glDeleteBuffers(1, &_ibo);
} 

void Model::init(const std::string &texture_fn)
{
    if(texture_fn.compare("")) {
        tex = new Texture(texture_fn);
        tex->load();
    }
    // Initialize vertices buffer and transfer it to OpenGL
    {

        std::vector<float>  _vertices;
        std::vector<float>  _tex_cords;
        std::vector<float> _normals(COLS * ROWS * 4);
        std::map<int, std::vector<glm::vec3>> indexNormal;
        std::vector<glm::vec4>  _vertices_mid;
        std::vector<GLuint> _indices;

        //make vertices
        for(float row = 0; row < ROWS; row++)
        {
            for(float col = 0; col < COLS; col++)
            {
                float z = ((row/ROWS) * 2.f - 1.f) * 20;
                float x = ((col/COLS) * 2.f - 1.f) * 20;
                _vertices.push_back(x);
                _vertices.push_back(0.f);
                _vertices.push_back(z);
                _vertices.push_back(1.f);
                _tex_cords.push_back(row/ROWS);
                _tex_cords.push_back(col/COLS);
                _tex_cords.push_back(1.f);
            }

        }

        //make indices
        for(GLuint row = 0; row < ROWS - 1; ++row)
        {
            for (GLuint col = 0; col < COLS - 1; ++col) {
                unsigned int v[3], nv[3];
                _indices.push_back(v[0] = MATRIX_ENTRY_1D(row, col));
                _indices.push_back(v[1] = MATRIX_ENTRY_1D((row + 1), col));
                _indices.push_back(v[2] = MATRIX_ENTRY_1D(row + 1, col + 1));

                _indices.push_back(nv[0] = MATRIX_ENTRY_1D(row, col));
                _indices.push_back(nv[1] = MATRIX_ENTRY_1D(row, col + 1));
                _indices.push_back(nv[2] = MATRIX_ENTRY_1D(row + 1, col + 1));

                //calculate normals
                {
                    float e1[3], e2[3], ne1[3], ne2[3];
                    for (int i = 0; i < 3; i++) {
                        e1[i] = _vertices[v[1] * 4 + i] - _vertices[v[0] * 4 + i];
                        e2[i] = _vertices[v[2] * 4 + i] - _vertices[v[0] * 4 + i];

                        ne1[i] = _vertices[nv[0] * 4 + i] - _vertices[nv[2] * 4 + i];
                        ne2[i] = _vertices[nv[1] * 4 + i] - _vertices[nv[2] * 4 + i];
                    }

                    glm::vec3 normal = glm::cross(glm::make_vec3(e1),
                                                  glm::make_vec3(e2));
                    glm::vec3 nnormal = glm::cross(glm::make_vec3(ne2),
                                                   glm::make_vec3(ne1));


                    indexNormal[v[0]].push_back(normal);
                    indexNormal[v[1]].push_back(normal);
                    indexNormal[v[2]].push_back(normal);

                    indexNormal[nv[0]].push_back(nnormal);
                    indexNormal[nv[1]].push_back(nnormal);
                    indexNormal[nv[2]].push_back(nnormal);
                }
            }
        }

        for(std::map<int, std::vector<glm::vec3>>::iterator it = indexNormal.begin();
            it != indexNormal.end(); ++it)
        {
            glm::vec3 avg(0.f);
            for(auto vec : it->second)
            {
                avg += vec;
            }
            avg /= (it->second).size();
            for(int i = 0; i < 3; i++)
            {
                _normals.at(it->first * 4 + i) = avg[i];
            }
            _normals.at(it->first * 4 + 3) = 0.f;
        }


        _nVertices = _indices.size();

        // Create and bind the object's Vertex Array Object:
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
		
        // Create and load vertex data into a Vertex Buffer Object:
        glGenBuffers(4, _vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, // attribute handle
                              4,          // number of scalars per vertex
                              GL_FLOAT,   // scalar type
                              GL_FALSE,
                              0,
                              0);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _normals.size(), &_normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, // attribute handle
                              4,          // number of scalars per vertex
                              GL_FLOAT,   // scalar type
                              GL_FALSE,
                              0,
                              0);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _tex_cords.size(), &_tex_cords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, // attribute handle
                              3,          // number of scalars per vertex
                              GL_FLOAT,   // scalar type
                              GL_FALSE,
                              0,
                              0);

//        glGenBuffers(1, &_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[3]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
		
        // Unbind vertex array:
        glBindVertexArray(0);
    }
}

void Model::draw()
{
    // Set the program to be used in subsequent lines:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//    // Draw using the state stored in the Vertex Array object:
    glBindVertexArray(_vao);
	if(tex != nullptr)
    {
        tex->bind(GL_TEXTURE0);
    }

    glDrawElements(GL_TRIANGLES, _nVertices, GL_UNSIGNED_INT, (GLvoid*)0);

    // Unbind the Vertex Array object
    glBindVertexArray(0);

}

void Model::scale(const glm::vec3 &v)
{
    _scale = glm::scale(_scale, v);
    update_model();
}

void Model::rotate(float angel, const glm::vec3 &axis) {
    _rotatation = glm::rotate(_rotatation, glm::radians(angel), axis);
    update_model();
}

void Model::translte(const glm::vec3 &v) {
    _translation = glm::translate(_translation, v);
    update_model();
}

void Model::update_model() {
    _model =  _translation * _rotatation * _scale;
}
