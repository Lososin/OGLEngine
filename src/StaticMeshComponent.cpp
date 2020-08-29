#include "StaticMeshComponent.hpp"

StaticMeshComponent::~StaticMeshComponent()
{
    DeleteGeometryPointers();
};

void StaticMeshComponent::SetGeometry(std::shared_ptr<S_Geometry>& OtherGeometry)
{
    Geometry = OtherGeometry;
    InitGeometry();
};

void StaticMeshComponent::Render()
{
    //ShaderProgram->Use();
    glBindVertexArray(VAO);
    
    glm::mat4 Model = ViewLibrary::ModelMatrix(GetLocation(), GetRotation(), GetScale());
    
    MeshShader->SetMat4("model", Model);
    
    MeshShader->SetMaterial(Material);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLuint)Geometry->Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
};

void StaticMeshComponent::DrawGUI()
{
    Object::DrawGUI();
    
    if (ImGui::TreeNode("Static Mesh"))
    {
        ImGui::Button("Load Geometry");
        
        ImGui::TreePop();
    }
};

void StaticMeshComponent::SetShader(std::shared_ptr<Shader>& NewShader)
{
    MeshShader = NewShader;
};

void StaticMeshComponent::SetMaterial(std::shared_ptr<S_Material>& NewMaterial)
{
    Material = NewMaterial;
};

std::vector<glm::vec3> StaticMeshComponent::GetGeometry() const
{
    glm::mat4 Model = ViewLibrary::ModelMatrix(GetLocation(), GetRotation(), GetScale());

    std::vector<glm::vec3> OutputVertices;
    for (int i = 0; i < Geometry->Vertices.size(); i++)
    {
        glm::vec4 vert = glm::vec4(Geometry->Vertices[i].Position, 1.f);

        glm::vec4 ModifiedVert = Model * vert;
        OutputVertices.push_back(glm::vec3(ModifiedVert.x, ModifiedVert.y, ModifiedVert.z));
    }

    return OutputVertices;
};

void StaticMeshComponent::InitGeometry()
{
    DeleteGeometryPointers();
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, Geometry->Vertices.size() * sizeof(Vertex), &(Geometry->Vertices[0]), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Geometry->Indices.size() * sizeof(unsigned int),
                 &(Geometry->Indices[0]), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
};

void StaticMeshComponent::DeleteGeometryPointers()
{
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
};

