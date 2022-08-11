#include <glad.h>
#include "stb_image.h"
#include "Texture2D.hpp"
#include "iostream"

Texture2D::Texture2D() {
    glGenTextures(1, &_texID);
    glBindTexture(GL_TEXTURE_2D, _texID);
    stbi_set_flip_vertically_on_load(true);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture2D::setData(const char *imagePath, unsigned int colorFormat) {
    int width, height;
    int nrChannels;
    unsigned char *textureData = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if(textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "failed to load texture\n";
    }
    stbi_image_free(textureData);
}

void Texture2D::activeTex(unsigned int texture) {
    glActiveTexture(texture);
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, _texID);
}

void Texture2D::setUniformSampler(unsigned int program, const char *name, int textureUnit) {
    glUniform1i(glGetUniformLocation(program, name), textureUnit - GL_TEXTURE0);
}