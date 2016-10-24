#version 330 core

// Interpolated values from the vertex shaders
//in vec3 PositionWorldPass;
//in vec3 NormalViewPass;
//in vec3 EyeDirectionViewPass;
//in vec3 LightDirectionViewPass[2];
in vec2 TexCoordPass;
in vec3 cubeTexCoordPass;
in vec3 normal0;
in vec3 position0;

struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight
{
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation
{
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight
{
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

// Ouput data
out vec3 outColor;

// Values that stay constant for the whole mesh.
uniform vec4 gMaterialColor;
uniform mat4 gView;
uniform mat4 gModel;
uniform vec4 gLightPosition[2]; // light (in world)
uniform vec3 gEyePosition; // camera (in world)
uniform vec3 gLightsColor[2];
uniform float diffuseCoeff;
uniform float ambientCoeff;
uniform int numOfPointLights;

uniform PointLight gPointLights[2];
uniform DirectionalLight gDirectLight;

uniform float gSpecularIntensity;

uniform sampler2D gSampler0;
uniform samplerCube gCubemapTexture;
uniform bool texType;


//vec3 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)
//{
//    vec3 AmbientColor = Light.Color * Light.AmbientIntensity;
//    float DiffuseFactor = dot(Normal, -LightDirection);
//
//    vec3 DiffuseColor  = vec3(0, 0, 0);
//    vec3 SpecularColor = vec3(0, 0, 0);
//
//    if (DiffuseFactor > 0) {
//        DiffuseColor = Light.Color * Light.DiffuseIntensity * DiffuseFactor;
//
//        vec3 VertexToEye = normalize(gEyePosition  - PositionWorldPass);
//        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
//        float SpecularFactor = dot(VertexToEye, LightReflect);
//        if (SpecularFactor > 0) {
//            SpecularFactor = pow(SpecularFactor, 10);
//            SpecularColor = Light.Color * gSpecularIntensity * SpecularFactor;
//        }
//    }
//
//    return (AmbientColor + DiffuseColor + SpecularColor);
//}
//
//vec3 CalcDirectionalLight(vec3 Normal)
//{
//    return CalcLightInternal(gDirectLight.Base, gDirectLight.Direction, Normal);
//}
//
//vec3 CalcPointLight(PointLight l, vec3 Normal)
//{
//    vec3 LightDirection = PositionWorldPass - l.Position;
//    float Distance = length(LightDirection);
//    LightDirection = normalize(LightDirection);
//
//    vec3 Color = CalcLightInternal(l.Base, LightDirection, Normal);
//    float attenuation =  l.Atten.Constant +
//                         l.Atten.Linear * Distance +
//                         l.Atten.Exp * Distance * Distance;
//
//    return Color;
//}

vec3 blinShading()
{
	vec3 LightColor = vec3(1.f,1.f, 1.f);

    vec3 color = vec3(0.f);

    vec3 N = normalize(normal0);

//    vec3 MaterialDiffuseColor;
	vec3 MaterialDiffuseColor = gMaterialColor.xyz;
	vec3 MaterialAmbientColor;
	vec3 MaterialSpecularColor = vec3(0.3f);

	if(texType)
	{
	    MaterialDiffuseColor = MaterialDiffuseColor;
	}
	else
	{
        MaterialDiffuseColor = MaterialDiffuseColor * texture(gSampler0, TexCoordPass).xyz;
	}

	MaterialAmbientColor = 0.5  * MaterialDiffuseColor;

	vec3 ambient = LightColor * ambientCoeff * MaterialAmbientColor;

	vec3 L = normalize(gLightPosition[0].xyz - position0);
	float cosTheta = max(0.f, dot(L, N));

	vec3 diffuse =  LightColor * diffuseCoeff * MaterialDiffuseColor * cosTheta;

    vec3 V = normalize(gEyePosition - position0);
    vec3 R = normalize(reflect(-L, N));

    vec3 H = normalize(L + gEyePosition);
    float cosAlpha = max(0.f, dot(V, R));
    vec3 specular = LightColor * pow(cosAlpha, 35) * MaterialSpecularColor;

    color += ambient + diffuse + specular;
	return color;
}

void main()
{

////    vec3 Normal = normalize(NormalViewPass);
////    vec3 TotalLight = CalcDirectionalLight(Normal);
////    for (int i = 0 ; i < numOfPointLights ; i++) {
////        TotalLight += CalcPointLight(gPointLights[i], Normal);
////    }
////
////    outColor = TotalLight * gMaterialColor.xyz;
////	// Light emission properties
//	// You probably want to put them as uniforms
//	vec3 LightColor = vec3(1.f,1.f, 1.f);
//
////	outColor = texture(gTextureSampler, TexCoordPass).rgb;
//
//	// Material properties
//
////	vec3 MaterialDiffuseColor = texture(gTextureSampler, TexCoordPass).rgb * gMaterialColor.xyz;
//    vec3 MaterialDiffuseColor = gMaterialColor.xyz;
//	//vec3 MaterialDiffuseColor = gMaterialColor.xyz;
//	vec3 MaterialAmbientColor =  MaterialDiffuseColor;
//	vec3 MaterialSpecularColor = vec3(0.3f);
//
//	// Normal of the computed fragment, in camera space
//	vec3 N = normalize(NormalViewPass);
//
//    outColor = vec4(LightColor *  ambientCoeff * MaterialAmbientColor, 1.f);
//	for(int i = 0; i < 1; i++)
//	{
////        outColor += gLightsColor[i] * ambientCoeff * MaterialAmbientColor;
//		// Direction of the light (from the fragment to the light)
//    	vec3 L = normalize(LightDirectionViewPass[i]);
//    	// Cosine of the angle between the normal and the light direction,
//    	// clamped above 0
//    	//  - light is at the vertical of the triangle -> 1
//    	//  - light is perpendicular to the triangle -> 0
//    	//  - light is behind the triangle -> 0
//    	float cosTheta = clamp(dot(N, L), 0, 1);
//
//    	// Eye vector (towards the camera)
//    	vec3 V = normalize(EyeDirectionViewPass);
//    	// Direction in which the triangle reflects the light
//    	vec3 R = normalize(reflect(-L, N));
//    	// Cosine of the angle between the Eye vector and the Reflect vector,
//    	// clamped to 0
//    	//  - Looking into the reflection -> 1
//    	//  - Looking elsewhere -> < 1
//    	float cosAlpha = clamp(dot(V,R), 0, 1);
//        outColor += vec4(// Diffuse : "color" of the object
//                    MaterialDiffuseColor * LightColor * diffuseCoeff * cosTheta +
//                    //Specular : reflective highlight, like a mirror
//                    MaterialSpecularColor * LightColor * pow(cosAlpha,35), 1.f);
//	}
//	if(texType)
//	{
////	    if (cubeTexCoordPass.z < 0.0001 || cubeTexCoordPass.z > 0.9999)
////	    	outColor = texture(gSampler0, cubeTexCoordPass.xy) * outColor;
////        else if(cubeTexCoordPass.x< 0.0001 || cubeTexCoordPass.x> 0.9999)
////	    	outColor = texture(gSampler1, cubeTexCoordPass.yz) * outColor;
////        else if(cubeTexCoordPass.y< 0.0001 || cubeTexCoordPass.y> 0.9999)
////	    	outColor = texture(gSampler2, cubeTexCoordPass.xz) * outColor;
//        outColor = texture(gCubemapTexture, normalize(normal0));
////        outColor = outColor;
//	}
//	else
//	{
//	    outColor = texture(gSampler0, TexCoordPass) * outColor;
//    }
//     outColor = vec3(1, 1, 1);
    outColor = blinShading();
}