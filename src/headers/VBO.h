#pragma once
#include <GL/glew.h>

class VBO {
private:
    unsigned int ID;

public:
    VBO(float* vertices, unsigned int size);
    ~VBO();

    void Bind() const;
    void Unbind() const;
};
