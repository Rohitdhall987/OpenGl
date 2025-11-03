#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"

class Model {
public:
    Model() = default;
    explicit Model(const std::string& path);
    void Draw(const Shader& shader) const;

private:
    std::vector<Mesh> meshes;
    std::vector<Texture> loaded_textures;
    std::string directory;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
    unsigned int TextureFromFile(const char* path);
};
