#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "model.h"

struct Object {
    unsigned int id;
    std::string name;
    Model model;
    std::string shader;
    glm::mat4 transform;
};