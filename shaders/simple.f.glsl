#version 330

//uniform vec4 fillColor;
//in vec4 in_Color;
in vec4 nnormal;
in vec4 viewer;
in vec4 pos;
vec3 lightColor1 = vec3(1.0, 0.9, 0.7); // First light color
vec3 lightColor2 = vec3(0.6, 0.6, 1.0); // Second light color
vec3 ambientColor = vec3(1.0, 1.0, 1.0); // Ambient light color

//vec3 light_position1 = vec3(pos) - vec3(  3.0, 2.0,  1.0); // First light position
//vec3 light_position2 = vec3(pos) - vec3( -3.0, 0.0,  1.0); // Second light position
vec4 light_position1 = normalize(vec4(  3.0, 2.0,  1.0, 1.f) - pos); // First light position
vec4 light_position2 = normalize(vec4( -3.0, 0.0,  1.0, 1.f) - pos); // Second light position
vec4 light_position1_norm = vec4(vec3(light_position1),1.f);
vec4 light_position2_norm  = vec4(vec3(light_position2), 1.f);
vec3 ka = vec3(0.1, 0.1, 0.1); // Ambient coefficient
vec3 kd = vec3(0.3, 0.3, 0.3); // Diffuse coefficient
vec3 ks = vec3(0.3, 0.3, 0.3); // Specular coefficient
out vec4 outColor;
vec4 phongModel();
void main()
{
//    outColor = vec4(1.f, 1.f, 1.f, 1.f);
    outColor = phongModel();
}

vec4 phongModel()
{
    vec4 normal = normalize(nnormal);
    vec4 cam = normalize(viewer);
    float norm_light_angel_1 = max(dot(normal, light_position1), 0.f);
    float norm_light_angel_2 = max(dot(normal, light_position2), 0.f);
//    vec3 r_1 = (2.f * norm_light_angel_1 * normal) - light_position1;
//    vec3 r_2 = (2.f * norm_light_angel_2 * normal) - light_position2;
    vec4 r_1 = -reflect(light_position1, normal);
    vec4 r_2 = -reflect(light_position2, normal);
    float viewer_reflector_angle_1 = pow(max(dot(r_1, cam), 0.f), 100.f);
    float viewer_reflector_angle_2 = pow(max(dot(r_2, cam), 0.f), 100.f);
//    float viewer_reflector_angle_1 = pow(max(dot(r_1, viewer), 1.f), 10.f);
//    float viewer_reflector_angle_2 = pow(max(dot(r_2, viewer), 1.f), 10.f);
    vec4 i;
    vec3 i_a = ka * (ambientColor);
    vec3 i_d = kd * lightColor1 * norm_light_angel_1 + kd * lightColor2 * norm_light_angel_2;
    vec3 i_s = ks * lightColor1 * viewer_reflector_angle_1 + ks * lightColor2 * viewer_reflector_angle_2;
    i = vec4(i_a + i_d + i_s, 1.f);
    return i;
}