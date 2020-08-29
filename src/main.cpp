#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>

#include "CoreMinimal.hpp"

#include "Texture.hpp"
#include "ShaderProgram.hpp"
#include "Object.hpp"
#include "CameraManager.hpp"
#include "ObjectManager.hpp"

#include "StaticMeshComponent.hpp"
#include "PointLightComponent.hpp"
#include "LogSystem.h"
#include "AssetImporter.hpp"

#define PVD_HOST "127.0.0.1"

bool PIE = false;
int Selected = -1;


bool keys[1024];

GLfloat deltaTime = 0.0f;    // Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;      // Время вывода последнего кадра

bool firstMouse = true;

float lastX =  1920 / 2.0;
float lastY =  1080 / 2.0;

void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Window dimensions
GLuint WIDTH = 1920, HEIGHT = 1080;

//using namespace physx;
//
////Physicx
//static PxDefaultErrorCallback gDefaultErrorCallback;
//static PxDefaultAllocator gDefaultAllocatorCallback;
//static PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;

//PxFoundation* mFoundation = nullptr;
//PxPvd* mPvd = nullptr;
//PxPhysics* mPhysics = nullptr;
//PxScene* gScene = nullptr;
//
//std::vector<PxRigidActor*> boxes;

int main()
{
//    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
//
//    bool recordMemoryAllocations = true;
//
//    mPvd = PxCreatePvd(*mFoundation);
//    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
//    mPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);
//
//
//    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
//        PxTolerancesScale(), recordMemoryAllocations, mPvd);
//
//    PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
//    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
//
//    if(!sceneDesc.cpuDispatcher) {
//        PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
//        sceneDesc.cpuDispatcher = mCpuDispatcher;
//    }
//    if(!sceneDesc.filterShader)
//        sceneDesc.filterShader = gDefaultFilterShader;
//    gScene = mPhysics->createScene(sceneDesc);
//
//    gScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0);
//    gScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
//
//    PxMaterial* mMaterial = mPhysics->createMaterial(0.5, 0.5, 0.5);
//
//    PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
//
//    PxRigidStatic* plane = mPhysics->createRigidStatic(pose);
//
//    PxShape* shape = mPhysics->createShape(PxPlaneGeometry(), *mMaterial);
//    plane->attachShape(*shape);
//
//    gScene->addActor(*plane);
//
//    //2) Create cube
//    PxReal density = 1000.0f;
//    PxTransform transform(PxVec3(0.0f, 5.0, 0.0f), PxQuat(1.0f));
//    PxVec3 dimensions(0.5, 0.5, 0.5);
//    PxBoxGeometry geometry(dimensions);
//
//    for(int i = 0; i<1000; i++) {
//        transform.p = PxVec3(0.0f, 20.0f+10*i, 0.0f);
//        PxRigidDynamic *actor = PxCreateDynamic(*mPhysics, transform, geometry, *mMaterial, density);
//
//        actor->setAngularDamping(0.75);
//
//        actor->setLinearVelocity(PxVec3(0, 0, 0));
//        gScene->addActor(*actor);
//        boxes.push_back(actor);
//    }
    
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWmonitor* MyMonitor =  glfwGetPrimaryMonitor(); // The primary monitor.. Later Occulus?..

    const GLFWvidmode* mode = glfwGetVideoMode(MyMonitor);
    WIDTH = mode->width;
    HEIGHT = mode->height;

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width , height);

    
    Shader ProgramShader("Shaders/VSDefault.glsl", "Shaders/FSDefault.glsl");
    Shader LightSourceShader("Shaders/VSDefault.glsl", "Shaders/FSLightSource.glsl");
    
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    PointLightComponent* NewObject = ObjectManager::Get()->CreateObject<PointLightComponent>("MyName");
    
    std::vector<Object> MyObjects;
//    for (int i = 0; i < 10; i++)
//    {
//        Object NewObj(Cube, ProgramShader);
//        NewObj.Location = cubePositions[i];
//        NewObj.Scale = glm::vec3(0.5, 0.5, 0.5);
//        MyObjects.push_back(std::move(NewObj));
//    }
    
    
    //Creating LightSource
//    Object LightSource(Cube, LightSourceShader);
//    LightSource.Location = glm::vec3(0, -5, 0);
//    LightSource.Scale = glm::vec3(0.5, 0.5, 0.5);

    CameraManager::Get()->CreateCamera();
    CameraManager::Get()->GetCurrentCamera()->AddLocationA(glm::vec3(-10, 0, 0));
    AssetImporter::Get();
    std::vector<std::shared_ptr<S_Geometry>> GuitarPack = AssetImporter::Get().ImportMesh("Resources/Models/backpack.obj");
    std::vector<StaticMeshComponent*> GuitarMeshes;
    std::shared_ptr<S_Material> mat = std::make_shared<S_Material>();
    std::shared_ptr<Shader> MeshShader(&ProgramShader);
    
    std::shared_ptr<Texture2D> AlbedoGuitar = std::make_shared<Texture2D>("Resources/Models/1001_albedo.jpg", ETextureType::DIFFUSE);
    std::shared_ptr<Texture2D> SpecularGuitar = std::make_shared<Texture2D>("Resources/Models/1001_metalic.jpg", ETextureType::SPECULAR);
    mat->Diffuse = AlbedoGuitar;
    
    for (int i = 0; i < GuitarPack.size(); i++)
    {
        StaticMeshComponent* NewComponent = ObjectManager::Get()->CreateObject<StaticMeshComponent>("None");
        NewComponent->SetMaterial(mat);
        NewComponent->SetGeometry(GuitarPack[i]);
        NewComponent->SetShader(MeshShader);
        GuitarMeshes.push_back(NewComponent);
    }
    
    StaticMeshComponent* Mesh = ObjectManager::Get()->CreateObject<StaticMeshComponent>("Mesh");
    StaticMeshComponent* AnotherMesh = ObjectManager::Get()->CreateObject<StaticMeshComponent>("AnotherMesh");
    Mesh->SetShader(MeshShader);
    Mesh->SetMaterial(mat);
    AnotherMesh->SetShader(MeshShader);
    AnotherMesh->SetMaterial(mat);
    
    glfwGetCursorPos(window, (double*)&lastX, (double*)&lastY);
    lastX = 0;
    lastY = 0;
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    while (!glfwWindowShouldClose(window))
    {
        // Обрабатываем события
        glfwPollEvents();
        do_movement();
        
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // render your GUI
        bool* popen;
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("MyWindow", popen, window_flags);
        ImGui::SetWindowPos(ImVec2(1790 - ImGui::GetWindowSize().x, 0));
        ImGui::SetWindowSize(ImVec2(350, 1074));

        std::vector<Object*> ObjectsVector = ObjectManager::Get()->GetAllExistsObjects();
        static int selected = 0;
        {
            ImGui::BeginChild("left pane", ImVec2(320, 400), true);
            for (int i = 0; i < ObjectsVector.size(); i++)
            {
                char label[128];
                sprintf(label, ObjectsVector[i]->GetName().c_str(), i);
                if (ImGui::Selectable(label, Selected == ObjectsVector[i]->GetID()))
                {
                    Selected = ObjectsVector[i]->GetID();
                }
            }
            
            ImGui::EndChild();
        }
        
        if (ImGui::BeginPopupContextItem("item context menu"))
        {
            if (ImGui::Selectable("Create Mesh")) ObjectManager::Get()->CreateObject<StaticMeshComponent>("AnotherOne");
            if (ImGui::Selectable("Create Light")) ObjectManager::Get()->CreateObject<PointLightComponent>("AnotherOne");
            ImGui::EndPopup();
        }
        
        if (Selected >= 0)
        {
            ObjectManager::Get()->GetObjectByID(Selected)->DrawGUI();
        }
        
        ImGui::End();
        
        ImGui::ShowDemoWindow();
        

        ProgramShader.Use();

        ProgramShader.SetVec3("viewPos", CameraManager::Get()->GetCurrentCamera()->cameraPos);
        ProgramShader.SetVec3("dirLight.direction", 0.1, 0.1, 0.1);
        ProgramShader.SetVec3("dirLight.ambient",  0.4f, 0.4f, 0.4f);
        ProgramShader.SetVec3("dirLight.diffuse",  0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
        ProgramShader.SetVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
        
        ProgramShader.SetVec3("spotLights[0].position", CameraManager::Get()->GetCurrentCamera()->cameraPos);
        ProgramShader.SetVec3("spotLights[0].direction", CameraManager::Get()->GetCurrentCamera()->cameraFront);
        ProgramShader.SetVec3("spotLights[0].ambient", 0.0, 0.0, 0.0);
        ProgramShader.SetVec3("spotLights[0].diffuse", 0.5, 0.5, 0.5);
        ProgramShader.SetVec3("spotLights[0].specular", 0.5f, 0.5f, 0.5);
        ProgramShader.SetFloat("spotLights[0].cutOff",   glm::cos(glm::radians(12.5f)));
        
        ProgramShader.SetFloat("material.shininess", 64.0f);
        
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 1000.0f);

        ProgramShader.SetMat4("view", CameraManager::Get()->GetCurrentCamera()->GetView());
        ProgramShader.SetMat4("projection", proj);
        
        //gScene->simulate(deltaTime);
//        while(!gScene->fetchResults()) {
//            // do something useful
//        }

//        for (auto& obj : MyObjects)
//        {
//            obj.Tick(deltaTime);
//        }
    

        for (int i = 0; i < 10; i++)
        {
            
//            PxTransform Trf = boxes[i]->getGlobalPose();
//            MyObjects[i].Scale = glm::vec3(1.f, 1.f, 1.f);
//
//            MyObjects[i].Location.x = Trf.p.x;
//            MyObjects[i].Location.y = Trf.p.y;
//            MyObjects[i].Location.z = Trf.p.z;
//            float angle;
//            PxVec3 axis;
//            Trf.q.toRadiansAndUnitAxis(angle, axis);
//
//            //glm::eulerAngles(<#const qua<T, Q> &x#>)
//            //MyObjects[i].Rotation.x = axis.x;// * 3.14159f / 180.f;
//            //MyObjects[i].Rotation.y = axis.y;// * 3.14159f / 180.f;
//            //MyObjects[i].Rotation.z = axis.z;// * 3.14159f / 180.f;
            //MyObjects[i].Render(angle, glm::vec3(axis.x, axis.y, axis.z));
            //MyObjects[i].Render(0, glm::vec3(1, 0, 0));
        }
        NewObject->SetShaderData(&ProgramShader);
        //Mesh->Render();
        //AnotherMesh->Render();
        
        for (auto& elm : GuitarMeshes)
        {
            elm->Render();
        }
        
        Log.AddLog("X=%f, Y=%f, Z=%f\n", AnotherMesh->GetFrontVector().x, AnotherMesh->GetFrontVector().y, AnotherMesh->GetFrontVector().z);
        AddLog("", "");
        
//        LightSourceShader.Use();
//        LightSourceShader.SetMat4("view", CameraManager::Get()->GetCurrentCamera()->GetView());
//        LightSourceShader.SetMat4("projection", proj);
//        LightSource.Render(0.f, glm::vec3(0, 0, 1));
        

        
        //std::cout << 1.f / deltaTime << std::endl;

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    //Cube.~Geometry();
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if(action == GLFW_PRESS)
      keys[key] = true;
    else if(action == GLFW_RELEASE)
      keys[key] = false;
    
    if(key == GLFW_KEY_T && action == GLFW_PRESS)
        CameraManager::Get()->NextCamera();
    
    if(key == GLFW_KEY_P && action == GLFW_PRESS && keys[GLFW_KEY_O])
        PIE = !PIE;
    if(key == GLFW_KEY_O && action == GLFW_PRESS && keys[GLFW_KEY_P])
        PIE = !PIE;
}

void do_movement()
{
  // Camera controls
  GLfloat cameraSpeed = 5.0f * deltaTime;
    
  Camera* CurrentCamera = CameraManager::Get()->GetCurrentCamera();
    
    if(keys[GLFW_KEY_W])
        CurrentCamera->AddLocationA(glm::vec3(cameraSpeed, 0, 0));
    if(keys[GLFW_KEY_S])
        CurrentCamera->AddLocationA(glm::vec3(-cameraSpeed, 0, 0));
    if(keys[GLFW_KEY_A])
        CurrentCamera->AddLocationA(glm::vec3(0, -cameraSpeed, 0));
    if(keys[GLFW_KEY_D])
        CurrentCamera->AddLocationA(glm::vec3(0, +cameraSpeed, 0));
    if(keys[GLFW_KEY_E])
        CurrentCamera->AddLocationA(glm::vec3(0, 0, -cameraSpeed));
    if(keys[GLFW_KEY_Q])
        CurrentCamera->AddLocationA(glm::vec3(0, 0, cameraSpeed));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    
    
    CameraManager::Get()->GetCurrentCamera()->AddYaw(xoffset);
    CameraManager::Get()->GetCurrentCamera()->AddPitch(yoffset);
}

float
triangle_intersection(const glm::vec3& orig,
                      const glm::vec3& dir,
                      const glm::vec3& v0,
                      const glm::vec3& v1,
                      const glm::vec3& v2) {
    glm::vec3 e1 = v1 - v0;
    glm::vec3 e2 = v2 - v0;
    // Вычисление вектора нормали к плоскости
    glm::vec3 pvec = glm::cross(dir, e2);
    float det = glm::dot(e1, pvec);

    // Луч параллелен плоскости
    if (det < 1e-8 && det > -1e-8) {
        return -1;
    }

    float inv_det = 1 / det;
    glm::vec3 tvec = orig - v0;
    float u = glm::dot(tvec, pvec) * inv_det;
    if (u < 0 || u > 1) {
        return -1;
    }

    glm::vec3 qvec = glm::cross(tvec, e1);
    float v = glm::dot(dir, qvec) * inv_det;
    if (v < 0 || u + v > 1) {
        return -1;
    }
    return glm::dot(e2, qvec) * inv_det;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    
    if(!keys[GLFW_KEY_Z])
        return;
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    width /= 2;
    height /= 2;
    
    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    
    float x = (2.0f * mouse_x) / width - 1.0f;
    float y = 1.0f - (2.0f * mouse_y) / height;
    float z = 1.0f;
    glm::vec3 ray_nds = glm::vec3(x, y, z);
    glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
    
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 1000.0f);
    glm::vec4 ray_eye = glm::inverse(proj) * ray_clip;
    
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
    
    glm::mat4 view_matrix = CameraManager::Get()->GetCurrentCamera()->GetView();
    
    glm::vec3 ray_wor = (glm::inverse(view_matrix) * ray_eye);
    // don't forget to normalise the vector at some point
    ray_wor = glm::normalize(ray_wor);
    
    float mouseX = x / (width  * 0.5f) - 1.0f;
    float mouseY = y / (height * 0.5f) - 1.0f;

    //glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 1000.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f), CameraManager::Get()->GetCurrentCamera()->cameraFront, CameraManager::Get()->GetCurrentCamera()->cameraUp);

    glm::mat4 invVP = glm::inverse(proj * view);
    glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
    glm::vec4 worldPos = invVP * screenPos;

    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));
    dir = glm::vec3(dir.x, dir.y, -dir.z);
    
    std::vector<StaticMeshComponent*> SM = ObjectManager::Get()->GetAllObjectsOfClass<StaticMeshComponent>();
    
    std::map<int, float> DistancePerObject;
    for (auto& elm : SM)
    {
        std::vector<glm::vec3> Vertexes = elm->GetGeometry();
        
        DistancePerObject.insert({elm->GetID(), -1.f});
        
        for (int i = 0; i < Vertexes.size(); i+=3)
        {
            float Intersection = triangle_intersection(
                CameraManager::Get()->GetCurrentCamera()->cameraPos,
                ray_wor,
                Vertexes[i],
                Vertexes[i+1],
                Vertexes[i+2]
            );
            
            if (DistancePerObject[elm->GetID()] < Intersection)
            {
                DistancePerObject[elm->GetID()] = Intersection;
            }
        }
    }
    
    
    for (int i = 0; i < SM.size(); i++)
    {
        if (DistancePerObject[SM[i]->GetID()] < 0)
            DistancePerObject.erase(SM[i]->GetID());
    }
    
    float Min = DistancePerObject.begin()->second;
    
    //float Min = DistancePerObject;
    int sel = -1;
    //std::vector<int> keys = DistancePerObject.ke
    for (auto& elm : DistancePerObject)
    {
        if (elm.second <= Min)
        {
            Min = elm.second;
            sel = elm.first;
        }
    }
    
    if (Min > 0)
        Selected = sel;
    else
        Selected = -1;
    
    std::cout << "Mouse clicked X=" << ray_wor.x << " Y=" << ray_wor.y << " Z=" << ray_wor.z << std::endl;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && PIE)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && PIE)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
