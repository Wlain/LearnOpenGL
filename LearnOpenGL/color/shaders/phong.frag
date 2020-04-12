#version 330 core
in vec2 v_texCoord;

uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform vec3 u_ambientColor;
uniform vec3 u_diffuseColor;
uniform vec3 u_specularColor;
uniform vec3 u_lightPosition;
uniform vec3 u_eyePosotion;

in vec3 v_normal;
in vec3 v_worldPosition;

out vec4 fragColor;

void main()
{
    // ambient
    float ambientMaterial = 0.1;
    vec3 ambientColor = ambientMaterial * u_ambientColor;
    
    // diffuse
    float diffuseMaterial = 0.8;
    vec3 normalDir = normalize(v_normal);
    vec3 lightDir = normalize(u_lightPosition - v_worldPosition);
    vec3 diffuseColor = diffuseMaterial * u_diffuseColor * max(0.0, dot(lightDir, normalDir));
    
    // specular
    float shininess = 64.0;
    float specularMaterial = 1.0;
    vec3 reflectDir = reflect(-lightDir, normalDir);
    vec3 viewDir = normalize(u_eyePosotion - v_worldPosition);
    vec3 specularColor = specularMaterial * u_specularColor * pow(max(0.0, dot(viewDir, reflectDir)), shininess);
    
    fragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
}
