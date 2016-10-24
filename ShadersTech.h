//
// Created by Shirel Gazit on 23/09/2016.
//

#ifndef FINAL_PROJECT_SHADERSTECH_H
#define FINAL_PROJECT_SHADERSTECH_H


#include <GL/glew.h>
#include <string>
#include "ShaderIO.h"
#include <glm/gtc/type_ptr.hpp>


#define SHADER_DIR "../shaders"

class ShadersTech {
public:
    ShadersTech();
    virtual ~ShadersTech();
    virtual void init();
    void use();
    GLint getUniformLocation(const std::string& unifrom_name);

protected:
    GLuint _program;
};


#endif //FINAL_PROJECT_SHADERSTECH_H
