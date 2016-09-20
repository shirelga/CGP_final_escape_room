#version 330 core

#define MY_PI 3.1415926

// Output data will be interpolated for each fragment.
out vec3 PositionWorldPass;
out vec3 NormalViewPass;
out vec3 EyeDirectionViewPass;
out vec3 LightDirectionViewPass[2];
out vec2 TexCoordPass;
// Values that stay constant for the whole mesh.
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
uniform mat4 gModel, gView, gProjection;
uniform vec4 gLightPosition[2]; // light (in world)
uniform vec3 gEyePosition; // camera (in world)

void main()
{
	mat4 MVP = gProjection * gView * gModel;
	mat4 M = gModel;

	// Output position of the vertex, in clip space : gMVP * position
	gl_Position =  MVP * position; // for glutTeapot
//	gl_Position = position; // for glutTeapot

	// Position of the vertex, in worldspace : gWorld * position
	PositionWorldPass = (gModel * position).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 PositionView = (M * position).xyz;
	EyeDirectionViewPass = gEyePosition - PositionView;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPositionView = (M *  gLightPosition[0]).xyz;
	LightDirectionViewPass[0] = LightPositionView + EyeDirectionViewPass;

	LightPositionView = (M * gLightPosition[1]).xyz;
	LightDirectionViewPass[1] = LightPositionView + EyeDirectionViewPass;
	
	// Normal of the the vertex, in camera space
	NormalViewPass = (M * normal).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

//    TexCoordPass.x = (0.5 * position.x) + 0.5;
//    TexCoordPass.y = (0.5 * position.y) + 0.5;
//    TexCoordPass = position.xy;
    	// Set texture coordinates using spherical mapping:
    	{
    		float theta = atan(position.x/position.z);
    		float phi   = atan(position.y/length(vec2(position.x,position.z)));
    		float r     = length(position.xyz);
    		TexCoordPass.x = 1.0 - (theta + MY_PI) / (2*MY_PI);
    		TexCoordPass.y = 1.0 - (phi + MY_PI/2)  / MY_PI;
    	}

//	TexCoordPass = gl_MultiTexCoord0.xy;
}

