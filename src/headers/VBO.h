#pragma once
#include <GL/glew.h>

class VBO {
private:
    GLuint ID;

public:
    VBO(float* vertices, size_t size);
    ~VBO();

    void Bind() const;
    void Unbind() const;
};
