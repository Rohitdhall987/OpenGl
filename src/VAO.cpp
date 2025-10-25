#include "headers/VAO.h"

VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &ID);
}

void VAO::Bind() const {
    glBindVertexArray(ID);
}

void VAO::Unbind() const {
    glBindVertexArray(0);
}

void VAO::AddAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
}
