#include "Texture.hpp"

Texture2D::Texture2D(std::string Path, ETextureType Type)
{
    glGenTextures(1, &TextureObject);
    glBindTexture(GL_TEXTURE_2D, TextureObject);
    TextureData = SOIL_load_image(Path.c_str(), &TxWidth, &TxHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TxWidth, TxHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(TextureData);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    TextureType = Type;
};

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &TextureObject);
};

GLuint Texture2D::GetTextureID() const
{
    return TextureObject;
};

GLint Texture2D::GetHeight() const
{
    return TxHeight;
};

GLint Texture2D::GetWeight() const
{
    return TxWidth;
};

ETextureType Texture2D::GetTextureType() const
{
    return TextureType;
};
