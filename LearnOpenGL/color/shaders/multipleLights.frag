#version 330 core
out vec4 fragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct DirLight {
    // 光照方向
    vec3 direction;
    
    // 光照
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    // 位置方向信息
    vec3 position;
    
    // 光照
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // 衰减系数
    float constant;
    float linear;
    float quadratic;
};


struct SpotLight {
    // 位置方向信息
    vec3 position;
    vec3 direction;
    float cutOff;
    float outCutOff;
    // 光照
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // 衰减系数
    float constant;
    float linear;
    float quadratic;
};

#define NUM_POINT_LIGHTS 4

in vec3 v_normal;
in vec2 v_texCoord;
in vec3 v_worldPosition;

uniform vec3 u_eyePosotion;
uniform Material u_material;
uniform DirLight u_dirLight;
uniform SpotLight u_spotlight;
uniform PointLight u_pointLights[NUM_POINT_LIGHTS];

// function prototypes
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 worldPosition, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 worldPosition, vec3 viewDir);

void main()
{
    vec3 normal = normalize(v_normal);
    vec3 viewDir = normalize(u_eyePosotion - v_worldPosition);
    
    vec3 result = calcDirLight(u_dirLight, normal, viewDir);
    for(int i = 0; i < NUM_POINT_LIGHTS; ++i)
        result += calcPointLight(u_pointLights[i], normal, v_worldPosition, viewDir);
    result += calcSpotLight(u_spotlight, normal, v_worldPosition, viewDir);
    fragColor = vec4(result, 1.0);
}


vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // ambient shading
    vec3 ambientColor = light.ambient * vec3(texture(u_material.diffuse, v_texCoord));
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = light.diffuse * vec3(texture(u_material.diffuse, v_texCoord));
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specularColor = light.specular * spec * vec3(texture(u_material.specular, v_texCoord));
    // combine results
    return ambientColor + diffuseColor + specularColor;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 worldPosition, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - worldPosition);
    // ambient shading
    vec3 ambientColor = light.ambient * vec3(texture(u_material.diffuse, v_texCoord));
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = light.diffuse * vec3(texture(u_material.diffuse, v_texCoord));
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specularColor = light.specular * spec * vec3(texture(u_material.specular, v_texCoord));
    
    // attenuation
    float dictance = length(light.position - worldPosition);
    float attenuation = 1.0 / (light.constant + light.linear * dictance + light.quadratic * (dictance * dictance));
    ambientColor *= attenuation;
    diffuseColor *= attenuation;
    specularColor *= attenuation;
    // combine results
    return ambientColor + diffuseColor + specularColor;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 worldPosition, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - worldPosition);
    // ambient shading
    vec3 ambientColor = light.ambient * vec3(texture(u_material.diffuse, v_texCoord));
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = light.diffuse * vec3(texture(u_material.diffuse, v_texCoord));
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specularColor = light.specular * spec * vec3(texture(u_material.specular, v_texCoord));
    
    // attenuation
    float dictance = length(light.position - worldPosition);
    float attenuation = 1.0 / (light.constant + light.linear * dictance + light.quadratic * (dictance * dictance));
    // spotLight results
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outCutOff;
    float intensity = clamp((theta - light.outCutOff) / epsilon, 0.0, 1.0);
    ambientColor *= attenuation * intensity;
    diffuseColor *= attenuation * intensity;
    specularColor *= attenuation * intensity;
        // combine results
    return ambientColor + diffuseColor + specularColor;
}

