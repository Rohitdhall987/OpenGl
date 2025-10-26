#pragma once
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
private:
    GLuint ID;

    std::string ReadFile(const std::string& path) const;
    GLuint CompileVertexShader(const std::string& code) const;
    GLuint CompileFragmentShader(const std::string& code) const;
    void CheckCompileErrors(GLuint shader, const std::string& type) const;

public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Use() const;

    unsigned int GetUniform(std::string name) const;

    void SetVec3(std::string name, glm::vec3 value) const;
    void SetMat4(std::string name, glm::mat4 value) const;

    GLuint GetID() const { return ID; }
};

