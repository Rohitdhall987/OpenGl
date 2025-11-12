#pragma once
#include <glm/gtc/type_ptr.hpp>

//#include "texture.h"

struct Material {
    //Texture* diffuseMap;   
    //Texture* specularMap;
    //Texture* emissionMap;
    float shininess = 32.0f;
    float emissionStrength = 0.5f;
    float specular = 1.0f;
    bool face_culling = false;
};

struct DirLight {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
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

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};


