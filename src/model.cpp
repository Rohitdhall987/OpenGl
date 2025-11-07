#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stb/stb_image.h>

#include "headers/model.h"
#include "headers/utils/windoes_utils.h"

Model::Model(const std::string& path, bool inverted) {
    loadModel(path, inverted);
}

void Model::Draw(const Shader& shader) const {
    for (const auto& mesh : meshes)
        mesh.Draw(shader);
}

void Model::loadModel(const std::string& path, bool inverted) {
    Assimp::Importer importer;
    
    const aiScene* scene;
    
    if (inverted) {
        scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    }
    else {
        scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
    }

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ASSIMP ERROR: " << importer.GetErrorString() << "\n";
        return;
    }

    directory = path.substr(0, path.find_last_of("/\\"));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex v;
        v.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        v.Normal = mesh->HasNormals() ?
            glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) :
            glm::vec3(0.0f);
        v.TexCoords = mesh->mTextureCoords[0]
            ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
            : glm::vec2(0.0f);
        vertices.push_back(v);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
        auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {

    std::cout << "Loading texture\n";

    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {

        std::cout << "inside loop " << i << std::endl;

        aiString str;
        mat->GetTexture(type, i, &str);

        std::cout << "Raw bytes (" << str.length << "): ";
        for (unsigned int b = 0; b < str.length; ++b)
            printf("%02X ", (unsigned char)str.C_Str()[b]);
        std::cout << std::endl;


        std::string texName = std::string(str.C_Str());


        std::string filename = directory + "/" + texName;
        std::replace(filename.begin(), filename.end(), '\\', '/');


        std::cout << "Finding file " << filename << std::endl;

#ifdef _WIN32
        // Convert UTF-8 path to UTF-16 for Windows
        int wlen = MultiByteToWideChar(CP_UTF8, 0, filename.c_str(), -1, nullptr, 0);
        std::wstring wfilename(wlen, 0);
        MultiByteToWideChar(CP_UTF8, 0, filename.c_str(), -1, &wfilename[0], wlen);

        // Use wide version to check existence
        std::ifstream f(wfilename, std::ios::binary);
#else
        std::ifstream f(filename, std::ios::binary);
#endif

        if (!f) {
            std::cerr << "!!! Missing texture: " << filename << std::endl;
            continue;
        }


        bool skip = false;
        for (const auto& tex : loaded_textures) {
            if (tex.path == filename) {
                textures.push_back(tex);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(filename.c_str());
            texture.type = typeName;
            texture.path = filename;
            textures.push_back(texture);
            loaded_textures.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char* path) {
    int w, h, comp;
    stbi_set_flip_vertically_on_load(true);

#ifdef _WIN32
    // Convert UTF-8 to UTF-16 for Windows file system
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, path, -1, nullptr, 0);
    std::wstring wpath(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, path, -1, &wpath[0], size_needed);


    std::ifstream file(wpath, std::ios::binary);
    if (!file) {
        std::wcerr << L"[TextureFromFile] Cannot open: " << wpath << std::endl;
        return 0;
    }


    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), {});
    file.close();

    unsigned char* data = stbi_load_from_memory(buffer.data(), buffer.size(), &w, &h, &comp, 0);
#else
    unsigned char* data = stbi_load(path, &w, &h, &comp, 0);
#endif

    if (!data) {
        std::cerr << "Texture load failed: " << path << std::endl;
        return 0;
    }
    else {
        std::cout << "Texture loaded successfully: " << path << std::endl;
    }

    GLenum format = (comp == 1) ? GL_RED : (comp == 3 ? GL_RGB : GL_RGBA);
    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texID;
}
