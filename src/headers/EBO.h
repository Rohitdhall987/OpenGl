#pragma once
#include <GL/glew.h>

class EBO {
private:
    GLuint ID;

public:
    EBO(unsigned int* indices, size_t size);
    ~EBO();

    void Bind() const;
    void Unbind() const;
};