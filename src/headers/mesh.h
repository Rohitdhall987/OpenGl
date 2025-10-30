#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <string>
#include "Shader.h"


class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<Texture>& textures);

    void Draw(const Shader& shader) const;

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};
