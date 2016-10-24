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
#include <string.h>

#include "NullTech.h"

NullTechnique::NullTechnique()
{   
}

void NullTechnique::Init()
{
	if(!programManager::sharedInstance().programWithID("null"))
	{
		_program = programManager::sharedInstance().createProgram("null",
																  SHADER_DIR "/null_technique.vs",
																  SHADER_DIR "/null_technique.fs");
	}

    m_WVPLocation = getUniformLocation("gWVP");
}


void NullTechnique::SetWVP(const glm::mat4& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(WVP));
}

