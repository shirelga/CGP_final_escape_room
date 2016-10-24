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

#ifndef NULL_TECHNIQUE_H
#define	NULL_TECHNIQUE_H

#include "ShaderIO.h"
#include "ShadersTech.h"
#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

class NullTechnique : public ShadersTech {
public:

    NullTechnique();

    void Init();

    void SetWVP(const glm::mat4& WVP);

private:

    GLuint m_WVPLocation;
};


#endif