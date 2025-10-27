#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
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
uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{   
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light.ambient * material.ambient;
    vec3 diffuse  = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);  

    vec4 t =texture(ourTexture,TextureCoord);
    
    vec3 result = (ambient + diffuse +specular) * mix(vec3(1.0),t.rgb,t.a);
    FragColor =  vec4(result, 1.0);
} 