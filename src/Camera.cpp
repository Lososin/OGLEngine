#include "Camera.hpp"

void Camera::AddPitch(float Offset)
{
    pitch += Offset;
    
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    
    RecalcVectors();
};

void Camera::AddYaw(float Offset)
{
    yaw += Offset;
    
    RecalcVectors();
};

void Camera::AddLocationA(glm::vec3 Offset)
{
    cameraPos += Offset.x * cameraFront;
    cameraPos += Offset.y * glm::normalize(glm::cross(cameraFront, cameraUp));
    cameraPos += Offset.z * cameraUp;
};

void Camera::SetFOV(float FOV)
{
    
};

void Camera::RecalcVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
};

glm::mat4 Camera::GetView()
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
};
