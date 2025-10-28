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
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
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
    vec3 viewDir = normalize(viewPos - FragPos);


    float theta = dot(lightDir, normalize(-light.direction));
    float distance = length(light.position - FragPos);

    if (theta > light.outerCutOff)
    {
        float epsilon   = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);


        float attenuation = 1.0 / (
            light.constant +
            light.linear * distance +
            light.quadratic * (distance * distance)
        );


        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);


        vec3 texDiffuse  = vec3(texture(material.diffuse, TextureCoord));
        vec3 texSpecular = vec3(texture(material.specular, TextureCoord));
        vec4 texEmission = texture(material.emission, TextureCoord);


        vec3 ambient  = light.ambient  * texDiffuse;
        vec3 diffuse  = light.diffuse  * diff * texDiffuse;
        vec3 specular = light.specular * spec * texSpecular;

        ambient  *= attenuation * intensity;
        diffuse  *= attenuation * intensity;
        specular *= attenuation * intensity;


        vec3 emission = material.emissionStrength * texEmission.rgb * step(0.1, texEmission.a);


        vec3 result = ambient + diffuse + specular + emission;
        FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 texDiffuse = vec3(texture(material.diffuse, TextureCoord));
        FragColor = vec4(light.ambient * texDiffuse, 1.0);
    }
}
