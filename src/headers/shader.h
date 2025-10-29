#pragma once
#include <GL/glew.h>
#include <string>

#include "types.h"

class Shader {
private:
    unsigned int ID;

    std::string ReadFile(const std::string& path) const;
    unsigned int CompileVertexShader(const std::string& code) const;
    unsigned int CompileFragmentShader(const std::string& code) const;
    void CheckCompileErrors(unsigned int shader, const std::string& type) const;

public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Use() const;

    unsigned int GetUniform(std::string name) const;

    void SetFloat(std::string name, float value) const;
    void SetInt(std::string name, int value) const;
    void SetVec3(std::string name, glm::vec3 value) const;
    void SetMat4(std::string name, glm::mat4 value) const;
    void SetMaterial( Material value) const;
    void SetPointLight( PointLight value , int index) const;
    void SetDirectionLight(DirLight value) const;

    unsigned int GetID() const { return ID; }
};

