/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <limits.h>
#include <string>


#include "ShadowVolumeTech.h"

using namespace std;

ShadowVolumeTechnique::ShadowVolumeTechnique()
{   
}


void ShadowVolumeTechnique::init()
{
    if(!programManager::sharedInstance().programWithID("volume")) {
        _program = programManager::sharedInstance().createProgram("volume",
                                                                  SHADER_DIR "/shadow_volume.vs",
                                                                  SHADER_DIR "/shadow_volume.gs",
                                                                  SHADER_DIR "/shadow_volume.fs");
    }

    
    m_WVPLocation = getUniformLocation("gWVP");
    m_lightPosLocation = getUniformLocation("gLightPos");
}


void ShadowVolumeTechnique::SetWVP(const glm::mat4& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(WVP));
}


void ShadowVolumeTechnique::SetLightPos(const glm::vec3& Pos)
{
    glUniform3fv(m_lightPosLocation, 1, glm::value_ptr(Pos));
}