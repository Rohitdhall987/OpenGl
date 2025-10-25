#pragma once


class VAO {
private:
    unsigned int ID;

public:
    VAO();
    ~VAO();

    void Bind() const;
    void Unbind() const;

    void AddAttrib(int index, int size, unsigned int type, bool normalized, unsigned int stride, const void* pointer);
};
