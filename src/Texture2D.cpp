#include <GLAD/glad.h>
#include "stb_image.h"
#include "Texture2D.hpp"
#include "iostream"

//-------------------------------------------------------------//
//--------------- Texture class implementation ----------------//
//-------------------------------------------------------------//

Texture2D::Texture2D() {
    /* generates and bind new texture object */
    glGenTextures(1, &_texID);
    glBindTexture(GL_TEXTURE_2D, _texID);

    /* reverse texture */
    stbi_set_flip_vertically_on_load(true);

    /* settings for texture wrapping and filtering */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/* Sets texture data for given image and color format. */
void Texture2D::setData(const char *imagePath, unsigned int colorFormat) {
    int width, height;
    int nrChannels;
    static int samples = 16;

    /* loads texture from image */
    unsigned char *textureData = stbi_load(imagePath, &width, &height, &nrChannels, 0);    
    if(textureData) {
        /* set texture data on GPU side and creates mipmaps */
        glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, textureData);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, width, height, GL_TRUE);
        
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        throw std::runtime_error("failed to load texture");
    }
    stbi_image_free(textureData);
}

/* Activates given texture. */
void Texture2D::activeTex(unsigned int texture) {
    glActiveTexture(texture);
}

/* Binds texture. */
void Texture2D::bind() const {
    // glBindTexture(GL_TEXTURE_2D, _texID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _texID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _texID, 0);
}

/* Sets uniform sampler for given texture unit. */
void Texture2D::setUniformSampler(unsigned int program, const char *name, int textureUnit) {
    glUniform1i(glGetUniformLocation(program, name), textureUnit - GL_TEXTURE0);
}