//
// Created by Shirel Gazit on 21/08/2016.
//

#ifndef FINAL_PROJECT_CUBE_H
#define FINAL_PROJECT_CUBE_H


#include "Object.h"

class Cube : public Object{
public:
    Cube();
    virtual ~Cube();
    void init(glm::vec3 &center, float x, float y);

private:
    glm::vec3 square[4];
};


#endif //FINAL_PROJECT_CUBE_H
