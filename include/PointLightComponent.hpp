#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ShaderProgram.hpp"

#include "LightComponent.hpp"

class PointLightComponent : public LightComponent
{
public:
    PointLightComponent() :
        Constant(1.f),
        Linear(0.09f),
        Quadratic(0.032f),
        LightNum(0)
    {};
    
    virtual void DrawGUI() override
    {
        LightComponent::DrawGUI();
        
        if (ImGui::TreeNode("Point Light"))
        {
            ImGui::DragFloat("Constant", &Constant, 0.01, 0, 2);
            ImGui::DragFloat("Linear", &Linear, 0.0001, 0, 1);
            ImGui::DragFloat("Quadratic", &Quadratic, 0.0001, 0, 2);
            
            ImGui::TreePop();
        }
    };
    
    virtual void SetShaderData(Shader* ProgramShader) override
    {
        ProgramShader->SetFloat("pointLights[0].constant", Constant);
        ProgramShader->SetFloat("pointLights[0].linear", Linear);
        ProgramShader->SetFloat("pointLights[0].quadratic", Quadratic);
        ProgramShader->SetVec3("pointLights[0].position", GetLocation());
        ProgramShader->SetVec3("pointLights[0].ambient", AmbientColor);
        ProgramShader->SetVec3("pointLights[0].diffuse", DiffuseColor);
        ProgramShader->SetVec3("pointLights[0].specular", SpecularColor);
    };
    
protected:
    float Constant;
    float Linear;
    float Quadratic;
    
    uint32_t LightNum;
};
