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
#include "glm/gtc/matrix_transform.hpp"

#ifdef __APPLE__
    #define SHADERS_DIR "../shaders/"
#else
    #define SHADERS_DIR "shaders/"
#endif

#define ROWS 20
#define COLS 20

Model::Model() :
    _vao(0), _vbo(0), _ibo(0)
{

}

Model::~Model()
{
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
    if (_vbo != 0) glDeleteBuffers(1, &_vbo);
    if (_ibo != 0) glDeleteBuffers(1, &_ibo);
} 

void Model::init()
{
    programManager::sharedInstance()
	.createProgram("default",
                       SHADERS_DIR "SimpleShader.vert",
                       SHADERS_DIR "SimpleShader.frag");

    GLuint program = programManager::sharedInstance().programWithID("default");
		
    // Obtain uniform variable handles:
    _fillColorUV  = glGetUniformLocation(program, "fillColor");
    _gpuWVP  = glGetUniformLocation(program, "wvp");

    // Initialize vertices buffer and transfer it to OpenGL
    {
//        // positioning vertices
//        static const GLfloat vertices[] = {
//            -0.5f, -1.0f, -0.5f, 1.0f,
//            0.5f,   -1.0f, -0.5f, 1.0f,
//            -0.5f,   -1.0f,0.5f, 1.0f,
//            0.5f,   -1.0f,0.5f, 1.0f,
//            -1.5f,   -1.0f,-0.5f, 1.0f,
//            -1.5f,  -1.0f,  0.5f,1.0f,
//        };
        std::vector<float>  _vertices;
        std::vector<glm::vec4>  _vertices_mid;
        std::vector<GLuint> _indices;

        //make vertices
        for(float row = 0; row < ROWS; row++)
        {
            for(float col = 0; col < COLS; col++)
            {
                float z = (row/ROWS) * 2.f - 1.f;
                float x = (col/COLS) * 2.f - 1.f;
//                std::cout<<"x: "<<x<<" z: "<<z<<std::endl;
                _vertices.push_back(x * 5);
                _vertices.push_back(0.f);
                _vertices.push_back(z * 5);
                _vertices.push_back(1.f);
//                _vertices_mid.push_back(glm::vec4(x, 0.f, z, 1.f));
            }

        }

//        _vertices = midMap(_vertices_mid);
        //make indices
        for(GLuint row = 0; row < ROWS - 1; ++row)
        {
            for (GLuint col = 0; col < COLS - 1; ++col)
            {
                if(row % 2) {
                    if (col % 2) {
                        _indices.push_back(row * COLS + col);
                        _indices.push_back((row + 1) * COLS + col);
                        _indices.push_back((row + 1) * COLS + (col + 1));

                        _indices.push_back(row * COLS + col);
                        _indices.push_back(row * COLS + (col + 1));
                        _indices.push_back((row + 1) * COLS + (col + 1));
                    }
                    else {
                        _indices.push_back((row + 1) * COLS + col);
                        _indices.push_back(row * COLS + (col + 1));
                        _indices.push_back((row + 1) * COLS + (col + 1));

                        _indices.push_back((row + 1) * COLS + col);
                        _indices.push_back(row * COLS + (col + 1));
                        _indices.push_back(row * COLS + col);
                    }
                }
                else
                {
                    if (col % 2) {
                        _indices.push_back((row + 1) * COLS + col);
                        _indices.push_back(row * COLS + (col + 1));
                        _indices.push_back((row + 1) * COLS + (col + 1));

                        _indices.push_back((row + 1) * COLS + col);
                        _indices.push_back(row * COLS + (col + 1));
                        _indices.push_back(row * COLS + col);
                    }
                    else {
                        _indices.push_back(row * COLS + col);
                        _indices.push_back((row + 1) * COLS + col);
                        _indices.push_back((row + 1) * COLS + (col + 1));

                        _indices.push_back(row * COLS + col);
                        _indices.push_back(row * COLS + (col + 1));
                        _indices.push_back((row + 1) * COLS + (col + 1));
                    }
                }
            }
        }

        // connecting vertices into triangles
//        static const GLubyte indices[] = {
//            1,0,3,
//            2,0,3,
//            0,2,4,
//            2,4,5
//        };
        _nVertices = _indices.size();

        // Create and bind the object's Vertex Array Object:
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
		
        // Create and load vertex data into a Vertex Buffer Object:
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
        
        // Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:
        // Obtain attribute handles:
        _posAttrib = glGetAttribLocation(program, "position");
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

void Model::draw(mat4 wvp)
{
    // Set the program to be used in subsequent lines:
    glUseProgram(programManager::sharedInstance().programWithID("default"));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Also try using GL_FILL and GL_POINT
  
    // pass the wvp to vertex shader
    glUniformMatrix4fv (_gpuWVP, 1, GL_FALSE, value_ptr(wvp));
    
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
