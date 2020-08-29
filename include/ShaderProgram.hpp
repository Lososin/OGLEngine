#pragma once

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Texture.hpp"

struct S_Material
{
    S_Material()
    {
        Diffuse = std::make_shared<Texture2D>("Resources/Textures/Default/Grid_D.png", ETextureType::DIFFUSE);
        Specular = std::make_shared<Texture2D>("Resources/Textures/Default/Grid_S.png", ETextureType::SPECULAR);
        Roughness = std::make_shared<Texture2D>("Resources/Textures/Default/Grid_R.png", ETextureType::ROUGHNESS);
        Metallic = std::make_shared<Texture2D>("Resources/Textures/Default/Grid_M.png", ETextureType::METALLIC);
        Normal = std::make_shared<Texture2D>("Resources/Textures/Default/Grid_N.png", ETextureType::NORMAL);
        Emissive = std::make_shared<Texture2D>("Resources/Textures/Default/Grid_E.png", ETextureType::EMISSIVE);
    }
    
    std::shared_ptr<Texture2D> Diffuse;
    std::shared_ptr<Texture2D> Specular;
    std::shared_ptr<Texture2D> Roughness;
    std::shared_ptr<Texture2D> Metallic;
    std::shared_ptr<Texture2D> Normal;
    std::shared_ptr<Texture2D> Emissive;
};

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        CheckCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void Use() const
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void SetBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void SetInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void SetFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void SetVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void SetVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void SetVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void SetVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void SetVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void SetVec4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void SetMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void SetMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void SetMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void SetMaterial(std::shared_ptr<S_Material> Mat)
    {
        SetTexture(Mat->Diffuse);
        SetTexture(Mat->Specular);
        SetTexture(Mat->Roughness);
        SetTexture(Mat->Metallic);
        SetTexture(Mat->Normal);
        SetTexture(Mat->Emissive);
    }
    void SetTexture(const std::string &name, GLuint TextureID, GLuint TextureIndex)
    {
        glActiveTexture(TextureIndex + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glUniform1i(glGetUniformLocation(ID, name.c_str()), TextureIndex);
    }
    
    void SetTexture(std::shared_ptr<Texture2D> Texture)
    {
        GLuint TextureNum = (GLuint)Texture->GetTextureType();
        std::string MaterialPin = GetPinNameByTextureType(Texture->GetTextureType());
        
        glActiveTexture(TextureNum + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture->GetTextureID());
        glUniform1i(glGetUniformLocation(ID, MaterialPin.c_str()), TextureNum);
    }
    
//    void ResetTextures()
//    {
//        for (uint i = 0; i < 6; i++)
//        {
//            Texture2D EmptyTexture("Resources/EmptyTexture.png", (ETextureType)i);
//            SetTexture(&EmptyTexture);
//        }
//    }
    
private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void CheckCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
    
    std::string GetPinNameByTextureType(ETextureType Type)
    {
        switch (Type) {
            case ETextureType::DIFFUSE:
                return "material.diffuse";
            case ETextureType::SPECULAR:
                return "material.specular";
            case ETextureType::ROUGHNESS:
                return "material.roughness";
            case ETextureType::METALLIC:
                return "material.metallic";
            case ETextureType::NORMAL:
                return "material.normal";
            case ETextureType::EMISSIVE:
                return "material.emission";
        }
    }
};
