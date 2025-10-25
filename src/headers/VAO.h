#pragma once
#include <GL/glew.h>

class VAO {
private:
    GLuint ID;

public:
    VAO();
    ~VAO();

    void Bind() const;
    void Unbind() const;

    void AddAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
};
