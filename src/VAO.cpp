#include "headers/VAO.h"
#include <GL/glew.h>

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

void VAO::AddAttrib(int index, int size, unsigned int type, bool normalized, unsigned int stride, const void* pointer) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
}
