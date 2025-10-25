#pragma once
#include <GL/glew.h>

class EBO {
private:
    unsigned int ID;

public:
    EBO(unsigned int* indices, unsigned int size);
    ~EBO();

    void Bind() const;
    void Unbind() const;
};