//
// Created by Shirel Gazit on 21/08/2016.
//

#ifndef FINAL_PROJECT_OBJECT_H
#define FINAL_PROJECT_OBJECT_H

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>


class Object {
public:
    Object();
    virtual ~Object();
    virtual void init(glm::vec3 center, float x, float y) = 0;
    virtual void draw() = 0;

private:
    glm::mat4 _model;
};


#endif //FINAL_PROJECT_OBJECT_H
