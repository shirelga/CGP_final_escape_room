//
//  Model.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Model__
#define __ex0__Model__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr

using namespace glm;

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <vector>

class Model {
	
    GLuint _vao, _vbo[2], _ibo;

    // Attribute handle:
    GLint _posAttrib;
	
    // Uniform handle:
    GLint _fillColorUV, _gpuW, _gpuV, _gpuP;
    
    size_t _nVertices;

//    std::vector<float>  _vertices;
//    std::vector<unsigned char> _indices;
    // View port frame:
    float _width, _height, _offsetX, _offsetY;

public:
    Model();
    virtual ~Model();
    void init();
    void draw(mat4 w, mat4 v, mat4 p);
    void resize(int width, int height);
    std::vector<float> midMap(std::vector<glm::vec4>& base);
};

#endif /* defined(__ex0__Model__) */
