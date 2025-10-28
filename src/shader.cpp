#include "headers/Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = ReadFile(vertexPath);
    std::string fragmentCode = ReadFile(fragmentPath);

    unsigned int vertex = CompileVertexShader(vertexCode);
    unsigned int fragment = CompileFragmentShader(fragmentCode);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::Use() const {
    glUseProgram(ID);
}


std::string Shader::ReadFile(const std::string& path) const {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot open shader file: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

unsigned int Shader::CompileVertexShader(const std::string& code) const {
    unsigned int shader = glCreateShader(GL_VERTEX_SHADER);
    const char* ccode = code.c_str();
    glShaderSource(shader, 1, &ccode, nullptr);
    glCompileShader(shader);
    CheckCompileErrors(shader, "VERTEX");
    return shader;
}

unsigned int Shader::CompileFragmentShader(const std::string& code) const {
    unsigned int shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* ccode = code.c_str();
    glShaderSource(shader, 1, &ccode, nullptr);
    glCompileShader(shader);
    CheckCompileErrors(shader, "FRAGMENT");
    return shader;
}

void Shader::CheckCompileErrors(unsigned int shader, const std::string& type) const {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
                << "\n" << infoLog
                << "\n---------------------------------------------\n";
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
                << "\n" << infoLog
                << "\n---------------------------------------------\n";
        }
    }
}


unsigned int Shader::GetUniform(std::string name) const {
    return glGetUniformLocation(ID, name.c_str());
}

void Shader::SetFloat(std::string name, float value) const
{
    unsigned int loc = GetUniform(name);
    glUniform1f(loc, value);
}

void Shader::SetInt(std::string name, int value) const
{
    unsigned int loc = GetUniform(name);
    glUniform1i(loc, value);
}

void Shader::SetVec3(std::string name, glm::vec3 value) const
{
    unsigned int loc = GetUniform(name);
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::SetMat4(std::string name, glm::mat4 value) const
{
    unsigned int loc = GetUniform(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMaterial(Material value) const {
    SetInt("material.diffuse", 0);
    SetInt("material.specular", 1);
    SetInt("material.emission", 2);
    SetFloat("material.shininess", value.shininess);
    SetFloat("material.emissionStrength", value.emissionStrength);
}

void Shader::SetLight(Light value) const
{
    SetVec3("light.position", value.position);
    SetVec3("light.ambient", value.ambient);
    SetVec3("light.diffuse", value.diffuse);
    SetVec3("light.specular", value.specular);
}
