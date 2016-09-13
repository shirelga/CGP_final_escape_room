#version 330 core

// Interpolated values from the vertex shaders
in vec3 PositionWorldPass;
in vec3 NormalViewPass;
in vec3 EyeDirectionViewPass;
in vec3 LightDirectionViewPass[2];
in vec2 TexCoordPass;
// Ouput data
out vec3 outColor;

// Values that stay constant for the whole mesh.
uniform vec4 gMaterialColor;
uniform mat4 gView;
uniform mat4 gModel;
uniform vec4 gLightPosition[2]; // light (in world)

void main()
{


	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(1.f,1.f,1.f);

//	outColor = texture(gTextureSampler, TexCoordPass).rgb;

	// Material properties

//	vec3 MaterialDiffuseColor = texture(gTextureSampler, TexCoordPass).rgb * gMaterialColor.xyz;
    vec3 MaterialDiffuseColor = gMaterialColor.xyz;
	//vec3 MaterialDiffuseColor = gMaterialColor.xyz;
	vec3 MaterialAmbientColor = vec3(0.5,0.5,0.5) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(1.0,1.0,1.0);

	// Normal of the computed fragment, in camera space
	vec3 N = normalize(NormalViewPass);

    outColor = MaterialAmbientColor;
//    outColor = vec3(0.0,0.0,0.0);
	for(int i = 0; i < 1; i++)
	{
		// Direction of the light (from the fragment to the light)
    	vec3 L = normalize(LightDirectionViewPass[i]);
    	// Cosine of the angle between the normal and the light direction,
    	// clamped above 0
    	//  - light is at the vertical of the triangle -> 1
    	//  - light is perpendicular to the triangle -> 0
    	//  - light is behind the triangle -> 0
    	float cosTheta = clamp(dot(N, L), 0, 1);

    	// Eye vector (towards the camera)
    	vec3 V = normalize(EyeDirectionViewPass);
    	// Direction in which the triangle reflects the light
    	vec3 R = reflect(-L, N);
    	// Cosine of the angle between the Eye vector and the Reflect vector,
    	// clamped to 0
    	//  - Looking into the reflection -> 1
    	//  - Looking elsewhere -> < 1
    	float cosAlpha = clamp(dot(V,R), 0, 1);
        outColor += // Diffuse : "color" of the object
                    MaterialDiffuseColor * LightColor * cosTheta +
                    //Specular : reflective highlight, like a mirror
                    MaterialSpecularColor * LightColor * pow(cosAlpha,20);
	}
//     outColor = vec3(1, 1, 1);
}