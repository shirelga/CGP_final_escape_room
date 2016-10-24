//
// Created by Shirel Gazit on 30/09/2016.
//

#ifndef FINAL_PROJECT_TEXTURE_H
#define FINAL_PROJECT_TEXTURE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>
#include <GL/glew.h>

#define MAX_TEXTURES 3

class Texture {

public:
    Texture(const std::string textue_fn);
    Texture(const std::string textue_fn[], int num_of_textures = 6);
    ~Texture();
    void load();
    void bind(GLenum unit);
    void unbind(GLenum unit);

private:
    bool _texture_type;
    std::vector<std::string>  _fn;
    GLuint _texture_sampler;
};


#endif //FINAL_PROJECT_TEXTURE_H
