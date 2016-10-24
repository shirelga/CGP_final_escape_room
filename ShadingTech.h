//
// Created by Shirel Gazit on 23/09/2016.
//

#ifndef FINAL_PROJECT_SHADINGTECH_H
#define FINAL_PROJECT_SHADINGTECH_H


#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_vec.hpp>
#include "ShadersTech.h"
struct BaseLight
{
    glm::vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;

    BaseLight()
    {
        Color = glm::vec3(0.0f, 0.0f, 0.0f);
        AmbientIntensity = 0.0f;
        DiffuseIntensity = 0.0f;
    }
};

struct DirectionalLight : public BaseLight
{
    glm::vec3 Direction;

    DirectionalLight()
    {
        Direction = glm::vec3(0.0f, 0.0f, 0.0f);
    }
};

struct PointLight : public BaseLight
{
    glm::vec3 Position;

    struct
    {
        float Constant;
        float Linear;
        float Exp;
    } Attenuation;

    PointLight()
    {
        Position = glm::vec3(0.0f, 0.0f, 0.0f);
        Attenuation.Constant = 1.0f;
        Attenuation.Linear = 0.0f;
        Attenuation.Exp = 0.0f;
    }
};

//struct SpotLight : public PointLight
//{
//    glm::vec3 Direction;
//    float Cutoff;
//
//    SpotLight()
//    {
//        Direction = glm::vec3(0.0f, 0.0f, 0.0f);
//        Cutoff = 0.0f;
//    }
//};

class ShadingTech : public ShadersTech{
public:
    ShadingTech();
    virtual ~ShadingTech();
    virtual void init();
    void set_wvp(glm::mat4& w, glm::mat4& v, glm::mat4& p);
    void set_light(glm::vec4 *light_pos, int num_of_lights, glm::vec3 *light_color);
    void set_eye(glm::vec3& eye);
    void set_material(glm::vec4& color);
    void set_diffuse(float coeff);
    void set_ambient(float coeff);
    void set_texture_type(bool type);
    void set_directional_light(const DirectionalLight& Light);
    void set_point_lights(unsigned int NumLights, const PointLight* pLights);

private:
    GLint _model_unifrom, _view_uniform, _projection_uniform, _light_pos_unifrom,
            _eye_pos_unifrom, _material_color_unifrom, _diffuse_coeff, _ambient_coeff,
            _num_point_lights_unifrom, _lights_color_uniform, _texture_type_unfirom,
            _sampler_uniform[3];

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Direction;
    } m_dirLightLocation;

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Position;
        struct {
            GLuint Constant;
            GLuint Linear;
            GLuint Exp;
        } Atten;
    } m_pointLightsLocation[2];
};


#endif //FINAL_PROJECT_SHADINGTECH_H
