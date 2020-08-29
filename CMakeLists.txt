cmake_minimum_required(VERSION 3.17)
project(OGLEngine)
set(CMAKE_CXX_STANDARD 17)
set(APP_TARGET OGLEngine) # Target Name
set(THIRD_PARTY_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty) # Include folder

find_package(OpenGL REQUIRED)
set(GL_SILENCE_DEPRECATION)

# Libs
## Imgui lib
set(imgui_Includes "${THIRD_PARTY_FOLDER}/imgui/")
add_library(imgui STATIC
        ${THIRD_PARTY_FOLDER}/imgui/imgui.cpp
        ${THIRD_PARTY_FOLDER}/imgui/imgui_draw.cpp
        ${THIRD_PARTY_FOLDER}/imgui/imgui_demo.cpp
        ${THIRD_PARTY_FOLDER}/imgui/imgui_impl_glfw.cpp
        ${THIRD_PARTY_FOLDER}/imgui/imgui_impl_opengl3.cpp
        ${THIRD_PARTY_FOLDER}/imgui/imgui_widgets.cpp)

## Glad lib
set(glad_Includes "${THIRD_PARTY_FOLDER}/glad/include")

add_subdirectory(${THIRD_PARTY_FOLDER}/glfw)
add_subdirectory(${THIRD_PARTY_FOLDER}/soil)
add_subdirectory(${THIRD_PARTY_FOLDER}/assimp)
add_subdirectory(${THIRD_PARTY_FOLDER}/minizip)

include_directories(${THIRD_PARTY_FOLDER}/glfw/include)
include_directories(${THIRD_PARTY_FOLDER}/glm)
include_directories(${THIRD_PARTY_FOLDER}/glad/include)
include_directories(${THIRD_PARTY_FOLDER}/imgui)
include_directories(${THIRD_PARTY_FOLDER}/assimp/include)
include_directories(${THIRD_PARTY_FOLDER}/soil/inc)
include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") # compiler version and compiler flags.

add_executable(OGLEngine
        src/main.cpp
        src/AssetImporter.cpp
        src/Camera.cpp
        src/CameraManager.cpp
        src/ObjectManager.cpp
        src/ShaderProgram.cpp
        src/StaticMeshComponent.cpp
        src/Texture.cpp
        ${THIRD_PARTY_FOLDER}/glad/src/glad.c)

target_link_libraries(OGLEngine glfw assimp minizip imgui soil ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})