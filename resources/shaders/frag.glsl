#version 330 core

struct Material {
    sampler2D emission;
    float shininess;
    float emissionStrength;
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

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;

uniform Material material;
uniform DirLight dirLight;

#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform vec3 viewPos;

// Multiple diffuse and specular maps
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

out vec4 FragColor;

// Function declarations
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// Blend helper
vec3 GetDiffuseColor();
vec3 GetSpecularColor();

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Calculate lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    //for (int i = 0; i < NR_POINT_LIGHTS; i++)
    //    result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);       
    
    // Add emissive contribution
    //vec3 emission = texture(material.emission, TextureCoord).rgb * material.emissionStrength;
    //result += emission;

    FragColor = vec4(result, 1.0);
}

// === Texture sampling ===
vec3 GetDiffuseColor()
{
    vec3 diffuse1 = texture(texture_diffuse1, TextureCoord).rgb;
    vec3 diffuse2 = texture(texture_diffuse2, TextureCoord).rgb;
    vec3 diffuse3 = texture(texture_diffuse3, TextureCoord).rgb;
    return (diffuse1 + diffuse2 + diffuse3) / 3.0; // blend equally
}

vec3 GetSpecularColor()
{
    vec3 spec1 = texture(texture_specular1, TextureCoord).rgb;
    vec3 spec2 = texture(texture_specular2, TextureCoord).rgb;
    return (spec1 + spec2) / 2.0;
}

// === Directional Light ===
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 diffuseTex = GetDiffuseColor();
    vec3 specularTex = GetSpecularColor();

    vec3 ambient  = light.ambient  * diffuseTex;
    vec3 diffuse  = light.diffuse  * diff * diffuseTex;
    vec3 specular = light.specular * spec * specularTex;
    return (ambient + diffuse + specular);
}  

// === Point Light ===
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));    

    vec3 diffuseTex = GetDiffuseColor();
    vec3 specularTex = GetSpecularColor();

    vec3 ambient  = light.ambient  * diffuseTex;
    vec3 diffuse  = light.diffuse  * diff * diffuseTex;
    vec3 specular = light.specular * spec * specularTex;
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}
