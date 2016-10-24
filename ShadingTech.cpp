//
// Created by Shirel Gazit on 23/09/2016.
//

#include <iostream>
#include "ShadingTech.h"
#define SHADER_NAME "phong"

ShadingTech::ShadingTech() { }
ShadingTech::~ShadingTech() { }

void ShadingTech::init() {

    if(!programManager::sharedInstance().programWithID(SHADER_NAME))
    {
        _program = programManager::sharedInstance().createProgram(SHADER_NAME,
                                                        SHADER_DIR "/phong.vert",
                                                        SHADER_DIR "/phong.frag");
    }

    _model_unifrom = getUniformLocation("gModel");
    _view_uniform = getUniformLocation("gView");
    _projection_uniform = getUniformLocation("gProjection");
    _light_pos_unifrom = getUniformLocation("gLightPosition");
    _eye_pos_unifrom = getUniformLocation("gEyePosition");
    _material_color_unifrom = getUniformLocation("gMaterialColor");
    _diffuse_coeff = getUniformLocation("diffuseCoeff");
    _ambient_coeff = getUniformLocation("ambientCoeff");
    _num_point_lights_unifrom = getUniformLocation("numOfPointLights");
    _lights_color_uniform = getUniformLocation("gLightsColor");
    _texture_type_unfirom = getUniformLocation("texType");

    glUseProgram(_program);
    std::cout<< getUniformLocation("gSampler" + std::to_string(0))<<"\n"
            <<getUniformLocation("gCubemapTexture")<<"\n";
    glUniform1i(getUniformLocation("gSampler0"), 0);
    glUniform1i(getUniformLocation("gCubemapTexture"), 2);
    glUseProgram(0);
//    m_dirLightLocation.Color = getUniformLocation("gDirectLight.Base.Color");
//    m_dirLightLocation.AmbientIntensity = getUniformLocation("gDirectLight.Base.AmbientIntensity");
//    m_dirLightLocation.Direction = getUniformLocation("gDirectLight.Direction");
//    m_dirLightLocation.DiffuseIntensity = getUniformLocation("gDirectLight.Base.DiffuseIntensity");
//
//    for (unsigned int i = 0 ; i < 2 ; i++) {
//        char Name[128];
//        memset(Name, 0, sizeof(Name));
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
//        m_pointLightsLocation[i].Color = getUniformLocation(Name);
//
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
//        m_pointLightsLocation[i].AmbientIntensity = getUniformLocation(Name);
//
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Position", i);
//        m_pointLightsLocation[i].Position = getUniformLocation(Name);
//
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
//        m_pointLightsLocation[i].DiffuseIntensity = getUniformLocation(Name);
//
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
//        m_pointLightsLocation[i].Atten.Constant = getUniformLocation(Name);
//
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
//        m_pointLightsLocation[i].Atten.Linear = getUniformLocation(Name);
//
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
//        m_pointLightsLocation[i].Atten.Exp = getUniformLocation(Name);
//    }
}

void ShadingTech::set_wvp(glm::mat4 &w, glm::mat4 &v, glm::mat4 &p) {
    glUniformMatrix4fv(_model_unifrom, 1, GL_FALSE, glm::value_ptr(w));
    glUniformMatrix4fv(_view_uniform, 1, GL_FALSE, glm::value_ptr(v));
    glUniformMatrix4fv(_projection_uniform, 1, GL_FALSE, glm::value_ptr(p));
}

void ShadingTech::set_light(glm::vec4 *light_pos, int num_of_lights, glm::vec3 *light_color) {
    glUniform1i(_num_point_lights_unifrom, num_of_lights);
    glUniform4fv(_light_pos_unifrom, num_of_lights, glm::value_ptr(light_pos[0]));
    glUniform4fv(_lights_color_uniform, num_of_lights, glm::value_ptr(light_color[0]));
}

void ShadingTech::set_eye(glm::vec3 &eye) {

    glUniform3fv(_eye_pos_unifrom, 1, glm::value_ptr(eye));
}

void ShadingTech::set_material(glm::vec4 &color) {
    glUniform4fv(_material_color_unifrom, 1, glm::value_ptr(color));
}

void ShadingTech::set_diffuse(float coeff) {
    glUniform1f(_diffuse_coeff, coeff);
}

void ShadingTech::set_ambient(float coeff) {
    glUniform1f(_ambient_coeff, coeff);
}

void ShadingTech::set_directional_light(const DirectionalLight &Light) {
    glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
    glm::vec3 Direction = glm::normalize(Light.Direction);
    glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void ShadingTech::set_point_lights(unsigned int NumLights, const PointLight *pLights) {
    glUniform1i(_num_point_lights_unifrom, NumLights);

    for (unsigned int i = 0 ; i < NumLights ; i++) {
        glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
        glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
        glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
        glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
        glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
        glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
        glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
    }
}

void ShadingTech::set_texture_type(bool type) {
    glUniform1i(_texture_type_unfirom, type);
}
