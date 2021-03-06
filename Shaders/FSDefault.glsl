#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3  position;
    
    vec3  direction;
    float cutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 30
uniform PointLight pointLights[NR_POINT_LIGHTS];
#define NR_SPOT_LIGHTS 30
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform DirLight dirLight;

uniform Material material;

uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 norm, vec3 FragPos, vec3 viewDir);

void main()
{

    // свойства
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // фаза 1: Направленный источник освещения
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    // фаза 2: Точечные источники
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    
    // фаза 3: фонарик
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
    
    //emission
    vec3 emission = vec3(texture(material.emission, TexCoords));

    FragColor = vec4(result + emission, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // диффузное освещение
    float diff = max(dot(normal, lightDir), 0.0);
    // освещение зеркальных бликов
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // комбинируем результаты
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // диффузное освещение
    float diff = max(dot(normal, lightDir), 0.0);
    // освещение зеркальных бликов
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // затухание
    float dist    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist +
                   light.quadratic * (dist * dist));
    // комбинируем результаты
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    
    if(theta > light.cutOff)
    {
        // диффузное освещение
        float diff = max(dot(normal, lightDir), 0.0);
        // освещение зеркальных бликов
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        
        vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
        vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
        
        return (ambient + diffuse + specular);
    }
    // иначе используем только фоновую компоненту, чтобы сцена не была
    // полностью черной вне конуса прожектора
    else
    {
        return light.ambient * vec3(texture(material.diffuse, TexCoords));
    }
    
    return vec3(0,0,0);
}
