#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "model.h"

struct Transform {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
};

struct Object {
    unsigned int id;
    std::string name;
    Model model;
    Shader* shader;
    Transform transform;
    Material material_prop;
    glm::mat4 modelMatrix ;
};