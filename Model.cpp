//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Model.h"

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "glm/gtc/matrix_transform.hpp"

#ifdef __APPLE__
    #define SHADERS_DIR "../shaders/"
#else
    #define SHADERS_DIR "shaders/"
#endif

#define ROWS 20
#define COLS 20
#define MATRIX_ENTRY_1D(row, col) (((row) * COLS) + col)

Model::Model() :
    _vao(0), _ibo(0)
{

}

Model::~Model()
{
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
    if (_vbo[0] != 0) glDeleteBuffers(2, _vbo);
    if (_ibo != 0) glDeleteBuffers(1, &_ibo);
} 

void Model::init()
{
    programManager::sharedInstance()
	.createProgram("default",
                       SHADERS_DIR "phong.vert",
                       SHADERS_DIR "phong.frag");

    GLuint program = programManager::sharedInstance().programWithID("default");
		
    // Obtain uniform variable handles:
    _fillColorUV  = glGetUniformLocation(program, "gMaterialColor");
    _gpuW  = glGetUniformLocation(program, "gModel");
    _gpuV  = glGetUniformLocation(program, "gView");
    _gpuP  = glGetUniformLocation(program, "gProjection");

    // Initialize vertices buffer and transfer it to OpenGL
    {

        std::vector<float>  _vertices;
        std::vector<float> _normals(COLS * ROWS * 4);
        std::map<int, std::vector<glm::vec3>> indexNormal;
        std::vector<glm::vec4>  _vertices_mid;
        std::vector<GLuint> _indices;

        //make vertices
        for(float row = 0; row < ROWS; row++)
        {
            for(float col = 0; col < COLS; col++)
            {
                float z = ((row/ROWS) * 2.f - 1.f) * 10;
                float x = ((col/COLS) * 2.f - 1.f) * 10;
//                std::cout<<"x: "<<x<<" z: "<<z<<std::endl;
                _vertices.push_back(x);
                _vertices.push_back(0.f);
                _vertices.push_back(z);
                _vertices.push_back(1.f);
//                _vertices_mid.push_back(glm::vec4(x, 0.f, z, 1.f));
            }

        }

//        _vertices = midMap(_vertices_mid);
//        _normals.reserve(_vertices.size());
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

        std::cout<<indexNormal.size()<<std::endl;
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
        glGenBuffers(2, _vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _normals.size(), &_normals[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
        
        // Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
        // Obtain attribute handles:
        _posAttrib = glGetAttribLocation(program, "position");
        glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
        glEnableVertexAttribArray(_posAttrib);
        glVertexAttribPointer(_posAttrib, // attribute handle
                              4,          // number of scalars per vertex
                              GL_FLOAT,   // scalar type
                              GL_FALSE,
                              0,
                              0);

        _posAttrib = glGetAttribLocation(program, "normal");
        glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
        glEnableVertexAttribArray(_posAttrib);
        glVertexAttribPointer(_posAttrib, // attribute handle
                              4,          // number of scalars per vertex
                              GL_FLOAT,   // scalar type
                              GL_FALSE,
                              0,
                              0);
		
        // Unbind vertex array:
        glBindVertexArray(0);
    }
}

void Model::draw(mat4 w, mat4 v, mat4 p)
{
    // Set the program to be used in subsequent lines:
    glUseProgram(programManager::sharedInstance().programWithID("default"));

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Also try using GL_FILL and GL_POINT
  
    // pass the wvp to vertex shader
    glUniformMatrix4fv (_gpuW, 1, GL_FALSE, value_ptr(w));
    glUniformMatrix4fv (_gpuV, 1, GL_FALSE, value_ptr(v));
    glUniformMatrix4fv (_gpuP, 1, GL_FALSE, value_ptr(p));

    // pass the model color to fragment shader   
    glUniform4f(_fillColorUV, 0.3f, 0.3f, 0.3f, 1.0);

    // Draw using the state stored in the Vertex Array object:
    glBindVertexArray(_vao);
	
   
    glDrawElements(GL_TRIANGLES, _nVertices, GL_UNSIGNED_INT, (GLvoid*)0);

    // Unbind the Vertex Array object
    glBindVertexArray(0);
	
    // Cleanup, not strictly necessary
    glUseProgram(0);
}

void Model::resize(int width, int height)
{
    _width = width;
    _height = height;
    _offsetX = 0;
    _offsetY = 0;
}


//std::vector<float> Model::midMap(std::vector<glm::vec4>& base)
//{
//    srand(std::clock());
//    std::vector<float> vertices;
////    int square_idx = floor(base.size() / 4);
//    base[(ROWS - 1)/2 * COLS + (COLS - 1)].y = (((float) rand() / (RAND_MAX)) * 2.f - 1.f)/5.f;
//
//    base[((ROWS-1)/4*(COLS/2)) + (COLS - 1)/2].y = (((float) rand() / (RAND_MAX)) * 2.f - 1.f)/5.f;
//
//    for(auto vec : base)
//    {
//        vertices.push_back(vec.x);
//        vertices.push_back(vec.y);
//        vertices.push_back(vec.z);
//        vertices.push_back(vec.w);
//    }
//    return vertices;
//}
