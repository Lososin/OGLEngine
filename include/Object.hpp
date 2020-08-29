#pragma once

#include <vector>
#include <string>

#include "CoreMinimal.hpp"

struct Transform_S
{
    Transform_S(glm::vec3 NewLocation = glm::vec3(0.f), glm::vec3 NewRotation = glm::vec3(0.f), glm::vec3 NewScale = glm::vec3(1.f)) :
        Location(NewLocation),
        Rotation(NewRotation),
        Scale(NewScale)
    {};
    
    glm::vec3 Location;
    glm::vec3 Rotation;
    glm::vec3 Scale;
};

class Object
{
public:
    friend class ObjectManager;
    
    Object() :
        Transform(),
        ID(0),
        Name("NONE")
    {};
    
protected:
    Transform_S Transform;
    
    uint32_t ID;
    
    std::string Name;

public:
    virtual void BeginPlay() {};
    
    virtual void Tick(float DeltaTime) {};
    
    virtual void DrawGUI()
    {
        if (ImGui::TreeNode("Transform"))
        {
            glm::vec3 NewLocation = GetLocation();
            glm::vec3 NewRotation = glm::degrees(GetRotation());
            glm::vec3 NewScale = GetScale();
            
            ImGui::DragFloat3("Location", &NewLocation[0], 0.01, -100, 100);
            ImGui::DragFloat3("Rotation", &NewRotation[0], 0.1f);
            ImGui::DragFloat3("Scale", &NewScale[0], 0.01, -100, 100);
            
            SetLocation(NewLocation);
            SetRotation(glm::radians(NewRotation));
            SetScale(NewScale);
            
            ImGui::TreePop();
        }
    };
    
    void SetLocation(glm::vec3 NewLocation) {Transform.Location = NewLocation;};
    void AddLocation(glm::vec3 LocationDelta) {Transform.Location += LocationDelta;};
    glm::vec3 GetLocation() const {return Transform.Location;};
    
    void SetRotation(glm::vec3 NewRotation) {Transform.Rotation = NewRotation;};
    void AddRotation(glm::vec3 RotationDelta) {Transform.Rotation += RotationDelta;};
    glm::vec3 GetRotation() const {return Transform.Rotation;};
    
    void SetScale(glm::vec3 NewScale) {Transform.Scale = NewScale;};
    void AddScale(glm::vec3 ScaleDelta) {Transform.Scale += ScaleDelta;};
    glm::vec3 GetScale() const {return Transform.Scale;};
    
    void SetTransform(Transform_S NewTransform) {Transform = NewTransform;};
    Transform_S GetTransform() const {return Transform;};
    
    //TODO: Implement Global functions
    void SetGlobalLocation(glm::vec3 NewLocation) {Transform.Location = NewLocation;};
    void AddGlobalLocation(glm::vec3 LocationDelta) {Transform.Location += LocationDelta;};
    glm::vec3 GetGlobalLocation() const {return Transform.Location;};
    
    void SetGlobalRotation(glm::vec3 NewRotation) {Transform.Rotation = NewRotation;};
    void AddGlobalRotation(glm::vec3 RotationDelta) {Transform.Rotation += RotationDelta;};
    glm::vec3 GetGlobalRotation() const {return Transform.Rotation;};
    
    void SetGlobalScale(glm::vec3 NewScale) {Transform.Scale = NewScale;};
    void AddGlobalScale(glm::vec3 ScaleDelta) {Transform.Scale += ScaleDelta;};
    glm::vec3 GetGlobalScale() const {return Transform.Scale;};
    
    void SetGlobalTransform(Transform_S NewTransform) {Transform = NewTransform;};
    Transform_S GetGlobalTransform() const {return Transform;};
    
    uint32_t GetID() const {return ID;};
    std::string GetName() const {return Name;};
    void SetName(std::string NewName) {Name = NewName;};
    
    glm::vec3 GetFrontVector() const {return ViewLibrary::RotateVector(glm::vec3(0.f, 0.f, -1.f), GetRotation());};
    glm::vec3 GetUpVector() const {return ViewLibrary::RotateVector(glm::vec3(0.f, 1.f, 0.f), GetRotation());};
    
protected:
    // need to call every time when transform modified
    void TransformUpdate()
    {
        
    };
};
