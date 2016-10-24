//
// Created by Shirel Gazit on 30/09/2016.
//

#include <iostream>
#include "Texture.h"

Texture::Texture(const std::string textue_fn) : _texture_type(false), _texture_sampler(0) {
        _fn.push_back(textue_fn);
}

Texture::Texture(const std::string *textue_fn, int num_of_textures) : _texture_type(true) {
    _fn.assign(textue_fn, &textue_fn[num_of_textures]);
}

Texture::~Texture() {
    if(_texture_sampler != 0)
    {
        glDeleteTextures(1, &_texture_sampler);
    }
}

void Texture::load() {
    cv::Mat img;
    if(_texture_type)
    {
        glGenTextures(1, &_texture_sampler);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _texture_sampler);

        for (unsigned int i = 0 ; i < 6 ; i++) {
           img = cv::imread(_fn[i]);
//        cv::imshow(_fn[i],img);
            if(!img.data)
            {
                std::cout << "Error loading texture '" << _fn[i] << std::endl;
            }
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, img.cols, img.rows, 0, GL_BGR,
                         GL_UNSIGNED_BYTE, img.ptr());

        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        return;
    }
        img =  cv::imread(_fn[0]);
//    cv::imshow(_fn[0], img);
        glGenTextures(1, &_texture_sampler);
        glBindTexture(GL_TEXTURE_2D, _texture_sampler);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE,
                     img.ptr());
        glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(GLenum unit) {
    GLenum type = GL_TEXTURE_2D;
    if(_texture_type)
    {
        type = GL_TEXTURE_CUBE_MAP;
    }
    glActiveTexture(unit);
    glBindTexture(type, _texture_sampler);
}

void Texture::unbind(GLenum unit) {
    return;
}