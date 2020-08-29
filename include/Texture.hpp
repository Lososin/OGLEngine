#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include <SOIL/SOIL.h>

enum ETextureType : uint32_t
{
    DIFFUSE = 0,
    SPECULAR,
    ROUGHNESS,
    METALLIC,
    NORMAL,
    EMISSIVE,
};

class Texture2D
{
public:
    Texture2D(std::string Path, ETextureType Type);
    ~Texture2D();
    
    GLuint GetTextureID() const;
    ETextureType GetTextureType() const;
    GLint GetHeight() const;
    GLint GetWeight() const;
    
private:
    GLuint TextureObject;
    GLint TxWidth, TxHeight;
    unsigned char* TextureData;
    ETextureType TextureType;
};
