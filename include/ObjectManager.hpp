#pragma once

#include <map>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.hpp"

class ObjectManager
{
public:
    static ObjectManager* Get()
    {
        if(!ManagerInstance)
            ManagerInstance = new ObjectManager();
        return ManagerInstance;
    }
    
    template<typename T>
    T* CreateObject(std::string Name, glm::vec3 Location = glm::vec3(0.f), glm::vec3 Rotation = glm::vec3(0.f), glm::vec3 Scale = glm::vec3(1.f))
    {
        T* NewObject = new T();
        if (NewObject == nullptr) return nullptr;
        
        NewObject->Name = Name;
        NewObject->ID = ID;
        ID++;
        
        NewObject->SetLocation(Location);
        NewObject->SetRotation(Rotation);
        NewObject->SetScale(Scale);
        
        RegistredObjects.insert({NewObject->ID, NewObject});
        return NewObject;
    }
    
    template<typename T>
    std::vector<T*> GetAllObjectsOfClass()
    {
        std::vector<T*> OutputObjects;
        
        for (auto& elm : RegistredObjects)
        {
            if (T* CastedObj = dynamic_cast<T*>(elm.second); CastedObj != nullptr)
            {
                OutputObjects.push_back(CastedObj);
            }
        }
        
        return OutputObjects;
    }
    
    std::vector<Object*> GetAllExistsObjects() const
    {
        std::vector<Object*> ObjectsVector;
        for (auto& elm : RegistredObjects)
            ObjectsVector.push_back(elm.second);
        
        return ObjectsVector;
    };
    
    Object* GetObjectByID(int ID)
    {
        return RegistredObjects[ID];
    }
    
    void DestroyObject(uint32_t ID)
    {
        
    }
    
    void DestroyObject(Object* ObjectToDestroy)
    {
        
    }
    
    std::map<uint32_t, Object*> RegistredObjects;
    
    uint32_t ID = 1;
    
    static ObjectManager* ManagerInstance;
};
