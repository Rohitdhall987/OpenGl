#version 330 core

in vec3 Normal; 
in vec3 FragPos;
in vec2 TextureCoord;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{   
    float specularStrength = 0.3;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    float ambient=0.1;
    vec4 t =texture(ourTexture,TextureCoord);
    
    vec3 result = (ambient + diffuse +specular) * mix(vec3(1),t.rgb,t.a);
    FragColor =  vec4(result, 1.0);
} 