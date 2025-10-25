#include "headers/VBO.h"

VBO::VBO(float* vertices, unsigned int size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VBO::~VBO() {
    glDeleteBuffers(1, &ID);
}

void VBO::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
