#include "headers/Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = ReadFile(vertexPath);
    std::string fragmentCode = ReadFile(fragmentPath);

    GLuint vertex = CompileVertexShader(vertexCode);
    GLuint fragment = CompileFragmentShader(fragmentCode);

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

GLuint Shader::CompileVertexShader(const std::string& code) const {
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    const char* ccode = code.c_str();
    glShaderSource(shader, 1, &ccode, nullptr);
    glCompileShader(shader);
    CheckCompileErrors(shader, "VERTEX");
    return shader;
}

GLuint Shader::CompileFragmentShader(const std::string& code) const {
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* ccode = code.c_str();
    glShaderSource(shader, 1, &ccode, nullptr);
    glCompileShader(shader);
    CheckCompileErrors(shader, "FRAGMENT");
    return shader;
}

void Shader::CheckCompileErrors(GLuint shader, const std::string& type) const {
    GLint success;
    GLchar infoLog[1024];
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