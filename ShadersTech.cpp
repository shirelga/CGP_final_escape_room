//
// Created by Shirel Gazit on 23/09/2016.
//

#include "ShadersTech.h"

ShadersTech::ShadersTech() { }

ShadersTech::~ShadersTech() { }

void ShadersTech::init() { }

void ShadersTech::use() {
    glUseProgram(_program);
}

GLint ShadersTech::getUniformLocation(const std::string &unifrom_name) {
    GLint uniform_loc = glGetUniformLocation(_program, unifrom_name.c_str());
    return uniform_loc;
}