#pragma once

#include <vector>

#include "CoreMinimal.hpp"

#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "Object.hpp"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct S_Geometry
{
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
};

class StaticMeshComponent : public Object
{
public:
    StaticMeshComponent() = default;
    ~StaticMeshComponent();
    
    void SetGeometry(std::shared_ptr<S_Geometry>& OtherGeometry);
    
    virtual void Render();
    
    virtual void DrawGUI() override;
    
    void SetShader(std::shared_ptr<Shader>& NewShader);
    void SetMaterial(std::shared_ptr<S_Material>& NewMaterial);

    std::vector<glm::vec3> GetGeometry() const;
    
private:
    void InitGeometry();
    void DeleteGeometryPointers();
    
    std::shared_ptr<Shader> MeshShader;
    std::shared_ptr<S_Geometry> Geometry;
    std::shared_ptr<S_Material> Material;
    
    GLuint VAO, VBO, EBO;
};
