#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ShaderProgram.hpp"
#include "Object.hpp"

class LightComponent : public Object
{
public:
    LightComponent() :
        AmbientColor(1.f),
        DiffuseColor(1.f),
        SpecularColor(1.f)
    {};
    
    virtual void DrawGUI()
    {
        Object::DrawGUI();
        
        if (ImGui::TreeNode("Light Color"))
        {
            ImGui::ColorEdit3("Ambient", &AmbientColor[0]);
            ImGui::ColorEdit3("Diffuse", &DiffuseColor[0]);
            ImGui::ColorEdit3("Specular", &SpecularColor[0]);
            
            ImGui::TreePop();
        }
    };
    
    virtual void SetShaderData(Shader* ProgramShader) = 0;
    
protected:
    glm::vec3 AmbientColor;
    glm::vec3 DiffuseColor;
    glm::vec3 SpecularColor;
};
