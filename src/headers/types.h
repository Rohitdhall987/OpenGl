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

struct Light {
    //glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};