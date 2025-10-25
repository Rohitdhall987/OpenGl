#pragma once
#include <string>

class Texture {
	unsigned int ID;
public:
	Texture(std::string path);
	~Texture();

	void Bind() const;
	void Unbind() const;
};