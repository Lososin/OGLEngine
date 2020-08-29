#pragma once

#include <vector>

#include "Camera.hpp"

class CameraManager
{
public:
    unsigned long CreateCamera()
    {
        Cameras.push_back(new Camera());
        return Cameras.size();
    }
    
    void NextCamera()
    {
        CurrentCameraIndex++;
        if (CurrentCameraIndex == Cameras.size())
        {
            CurrentCameraIndex = 0;
        }
    }
    
    Camera* GetCurrentCamera()
    {
        return Cameras[CurrentCameraIndex];
    }
    
    static CameraManager* Get()
    {
        if(!ManagerInstance)
            ManagerInstance = new CameraManager();
        return ManagerInstance;
    }
    
    std::vector<Camera*> Cameras;
    unsigned long CurrentCameraIndex = 0;
    
    static CameraManager* ManagerInstance;
};

