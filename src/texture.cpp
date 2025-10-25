#include "headers/texture.h"
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>

Texture::Texture(std::string path)
{
	glGenTextures(1, &ID);

    Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (!data)
        std::cout << "!Failed to load texture: \"" << path << "\"" << std::endl;

    unsigned int format;

    if (nrChannels == 4) {
        format = GL_RGBA;
    }
    else if (nrChannels==3) {
        format = GL_RGB;
    }
    else if (nrChannels == 2) {
        format = GL_RG;
    }
    else {
        format = GL_RED;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    Unbind();
}

Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
};

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, ID);
}