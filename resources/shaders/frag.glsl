#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
    float     emissionStrength;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal; 
in vec3 FragPos;
in vec2 TextureCoord;

uniform Material material;
uniform Light light;  
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{   
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec4 emissionTex = texture(material.emission.r, TextureCoord);
    vec3 emission = material.emissionStrength * emissionTex.rgb * step(0.1, emissionTex.a);

    vec3 ambient  = light.ambient * vec3(texture(material.diffuse,TextureCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TextureCoord));  
    vec3 specular = light.specular * spec *  vec3(texture(material.specular, TextureCoord));

    vec3 result = (ambient + diffuse +specular + emission) ;


    FragColor =  vec4(result, 1.0);
} 