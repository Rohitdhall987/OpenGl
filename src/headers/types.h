#pragma once
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"

struct Material {
    Texture* diffuseMap;   
    Texture* specularMap;
    Texture* emissionMap;
    float shininess;
    float emissionStrength;
};

struct DirLight {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight {
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};