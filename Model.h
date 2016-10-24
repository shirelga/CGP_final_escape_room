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
#include "Texture.h"

class Model {
	
    GLuint _vao, _vbo[4], _ibo;

    // Attribute handle:
    GLint _posAttrib;
	
    // Uniform handle:
    GLint _fillColorUV, _gpuW, _gpuV, _gpuP;
    
    size_t _nVertices;
    
    glm::mat4 _model;
    glm::mat4 _scale;
    glm::mat4 _rotatation;
    glm::mat4 _translation;
    Texture* tex;
//    std::vector<float>  _vertices;
//    std::vector<unsigned char> _indices;
    // View port frame:
    float _width, _height, _offsetX, _offsetY;

    void update_model();

public:
    Model();
    virtual ~Model();
    void init(const std::string &texture_fn);
    void draw();
    void scale(const glm::vec3 &v);
    void rotate(float angel, const glm::vec3& axis);
    void translte(const glm::vec3& v);
    std::vector<float> midMap(std::vector<glm::vec4>& base);
    
    inline const glm::mat4& get_model()
    {
        return _model;
    }
};

#endif /* defined(__ex0__Model__) */
