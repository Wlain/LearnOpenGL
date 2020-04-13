#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shinness;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_normal;
in vec2 v_texCoord;
in vec3 v_worldPosition;

uniform vec3 u_eyePosotion;
uniform Material u_material;
uniform Light u_light;

out vec4 fragColor;

void main()
{
    // ambient
    vec3 ambientMaterial = vec3(texture(u_material.diffuse, v_texCoord));
    vec3 ambientColor = ambientMaterial * u_light.ambient;
    
    // diffuse
    vec3 diffuseMaterial = vec3(texture(u_material.diffuse, v_texCoord));
    vec3 normalDir = normalize(v_normal);
    vec3 lightDir = normalize(u_light.position - v_worldPosition);
    vec3 diffuseColor = diffuseMaterial * u_light.diffuse * max(0.0, dot(lightDir, normalDir));
    
    // specular
    float shininess = u_material.shinness;
    vec3 specularMaterial = vec3(texture(u_material.specular, v_texCoord));
    vec3 reflectDir = reflect(-lightDir, normalDir);
    vec3 viewDir = normalize(u_eyePosotion - v_worldPosition);
    vec3 specularColor = specularMaterial * u_light.specular * pow(max(0.0, dot(viewDir, reflectDir)), shininess);
    
    // emission
    vec3 emissionColor = vec3(texture(u_material.emission, v_texCoord));
    
    fragColor = vec4(ambientColor + diffuseColor + specularColor + emissionColor, 1.0);
}

