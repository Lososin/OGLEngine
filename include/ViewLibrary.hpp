#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ViewLibrary
{
public:
    static glm::mat4 TranslateMatrix(glm::mat4 InMatrix, glm::vec3 Location)
    {
        return glm::translate(InMatrix, Location);
    }
    
    static glm::mat4 RotateMatrix(glm::mat4 InMatrix, glm::vec3 Rotation)
    {
        glm::mat4 RotationMatrix = InMatrix;
        
        RotationMatrix = glm::rotate(RotationMatrix, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        RotationMatrix = glm::rotate(RotationMatrix, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        RotationMatrix = glm::rotate(RotationMatrix, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        
        return RotationMatrix;
    }
    
    static glm::mat4 ScaleMatrix(glm::mat4 InMatrix, glm::vec3 Scale)
    {
        return glm::scale(InMatrix, Scale);
    }
    
    static glm::mat4 ModelMatrix(glm::vec3 Location, glm::vec3 Rotation, glm::vec3 Scale)
    {
        glm::mat4 Model = glm::mat4(1.f);
        
        Model = TranslateMatrix(Model, Location);
        Model = RotateMatrix(Model, Rotation);
        Model = ScaleMatrix(Model, Scale);
        
        return Model;
    }
    
    static glm::vec3 RotateVector(glm::vec3 InVector, glm::vec3 Rotation)
    {
        glm::mat4 RotationMatrix = RotateMatrix(glm::mat4(1.f), Rotation);
        
        return RotationMatrix * glm::vec4(InVector, 1.f);
    }
};
