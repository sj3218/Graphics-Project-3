/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS300_Project_Scene.cpp
Purpose: This file is source file for creating assignment scene.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-18-2021
End Header --------------------------------------------------------*/
#include <iostream>
#include "CS300_Project_Scene.hpp"
#include "shader.hpp"

const glm::vec3 SALMON(1, 0.453f, 0.453f),
LIGHTPINK(1, 0.68f, 0.906f),
RED(1.f, 0.f, 0.f),
GREEN(0.f, 1.f, 0.f),
BLUE(0.f, 0.f, 1.f),
LIGHTBLUE(0.4824f, 0.6275f, 0.8941f),
WHITE(1.f, 1.f, 1.f),
YELLOW(1.f, 1.f, 0.2f);


CS300_Project_Scene::CS300_Project_Scene(int windowWidth, int windowHeight) : Scene(windowWidth, windowHeight)
{
    InitMembers();

    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    tempWidth = windowWidth;
    tempHeight = windowHeight;

    glm::vec3 eye(0.f, 0.5f, 3.0f);
    glm::vec3 target(0.f, 0.f, -1.f);
    glm::vec3 up(0.f, 1.f, 0.f);
    float fov = 0.5f * PI;//90 degree
    float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    float near = 0.1f;
    float far = 10.f;

    cam.CameraSetUP(eye, target, up, fov, aspect, near, far);
    skybox_cam.CameraSetUP(glm::vec3(0.f), glm::vec3(0.f), up, fov, 1.f, near, far);
    camera = CameraToWorld(cam);
    projection = glm::perspective(glm::radians(45.f), aspect, 0.1f, 100.f);
    skybox_projection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 100.f);

    shader_type = 1;
    calculate_UV_GPU = false;
    projection_mode = 1;
    entities = 0;
    useUV = false;

    blendingPhong = 1.f;
    environmentType = 1;
    refractionIndex = 1.f;
    chromatic = 0.2f;
    fresnel = 5.f;
}

CS300_Project_Scene::~CS300_Project_Scene()
{
    InitMembers();
    delete imGui;
}

int CS300_Project_Scene::Init()
{
    programID = LoadShaders("../Common/shaders/VertexShader.vert",
        "../Common/shaders/FragmentShader.frag");

    phongLightingShader = LoadShaders("../Common/shaders/PhongLighting.vert", "../Common/shaders/PhongLighting.frag");
    phongShadingShader = LoadShaders("../Common/shaders/PhongShading.vert", "../Common/shaders/PhongShading.frag");
    blinnShadingShader = LoadShaders("../Common/shaders/BlinnShading.vert", "../Common/shaders/BlinnShading.frag");
    lightingShader = LoadShaders("../Common/shaders/LightObject.vert", "../Common/shaders/LightObject.frag");
    cubeShader = LoadShaders("../Common/shaders/skybox.vert", "../Common/shaders/skybox.frag");
    environmentShader = LoadShaders("../Common/shaders/EnvironmentMapping.vert", "../Common/shaders/EnvironmentMapping.frag");

    SetupBuffers();

    return Scene::Init();
}

void CS300_Project_Scene::CleanUp()
{
    glDeleteProgram(programID);
    objectManager.CleanUP();
}

void CS300_Project_Scene::ResizeWindow(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

void CS300_Project_Scene::KeyboardInput(GLFWwindow* window)
{
    const float angleIncrement = PI / 480.f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cam.Pitch(angleIncrement);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cam.Pitch(-angleIncrement);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        cam.Yaw(-angleIncrement);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        cam.Yaw(angleIncrement);
    }
}

void CS300_Project_Scene::MouseInput(GLFWwindow* window, double xPos, double yPos)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        currX = xPos;
        currY = yPos;
        return;
    }
}

int CS300_Project_Scene::Render(GLFWwindow* pWwindow)
{
    camera = CameraToWorld(cam);
    float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    projection = glm::perspective(glm::radians(90.f), aspect, 0.1f, 100.f);
    skybox_projection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 100.f); // fov - 90, aspect - 1

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //////////////value//////////////////////////
    float distance = 10.f / PI;
    glm::vec3 scaleVector = glm::vec3(1.f);
    glm::vec3 translate = glm::vec3(0.f);
    glm::vec3 rotateVector = glm::vec3(0.f, 1.0f, 0.f);
    glm::vec3 skyboxRotateVector = glm::vec3(0.f);
    float angleOfRotation = 0.f;
    glm::vec3 lightPosition = glm::vec3(4.f, 2.f, 0.f);

    glm::vec3 centerModelScale = glm::vec3(0.7f);
    glm::vec3 centerModelTranslate = glm::vec3(0.f);

    int numlights = objectManager.GetLight()->GetNumberOfLights();
    float k_d = 0.1f;

    int width, height = 0;
    glfwGetFramebufferSize(pWwindow, &width, &height);
    
    objectManager.GetFrameManager()->BindFrameBuffer();
    objectManager.GetFrameManager()->CheckFrameBufferStatus();

    glDisable(GL_DEPTH_TEST);

    ///////////////draw cube map/////////////////////
    for (int i = 0; i < 6; ++i)
    {
        if (i == 1) //left
        {
            skybox_cam.CameraSetUP(centerModelTranslate, glm::vec3(-1.f, 0.f, 0.f), rotateVector, 0.5f * PI, 1.f, 0.1f, 10.f);
        }
        else if (i == 2) //right
        {
            skybox_cam.CameraSetUP(centerModelTranslate, glm::vec3(1.f, 0.f, 0.f), rotateVector, 0.5f * PI, 1.f, 0.1f, 10.f);
        }
        else if (i == 3) //bottom
        {
            skybox_cam.CameraSetUP(centerModelTranslate, glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.f, 0.f, 0.f), 0.5f * PI, 1.f, 0.1f, 10.f);
        }
        else if (i == 4) //top
        {
            skybox_cam.CameraSetUP(centerModelTranslate, glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 0.f, 0.f), 0.5f * PI, 1.f, 0.1f, 10.f);
        }
        else if (i == 5) //back
        {
            skybox_cam.CameraSetUP(centerModelTranslate, glm::vec3(0.f, 0.f, -1.f), rotateVector, 0.5f * PI, 1.f, 0.1f, 10.f);
        }
        else if (i == 0) //front
        {
            skybox_cam.CameraSetUP(centerModelTranslate, glm::vec3(0.f, 0.f, 1.f), rotateVector, 0.5f * PI, 1.f, 0.1f, 10.f);
        }

        camera = CameraToWorld(skybox_cam);

        glUseProgram(cubeShader);
        objectManager.SetShaderProgramID(&cubeShader);
        objectManager.GetVboManager()->UseSkyTextures();
        objectManager.DrawObject(ObjectTypeEnum::eCUBE, skybox_projection, camera, RED, false);

        glUseProgram(programID);
        objectManager.SetShaderProgramID(&programID);
        glm::mat4 orbitMat = glm::mat4(0.50f);
        objectManager.SetUniforms(skybox_projection, camera, orbitMat);
        objectManager.DrawOrbit();

        /////////////////draw lights////////////////
        GlobalInfo global_info = objectManager.GetGlobalInformation();
        glClearColor(global_info.fogColor.x, global_info.fogColor.y, global_info.fogColor.z, 1.f);
        objectManager.SetGlobalUniform(numlights, global_info.fogFar, global_info.fogNear, global_info.attenuation, global_info.fogColor, global_info.globalAmbientColor);

        glUseProgram(lightingShader);
        objectManager.SetShaderProgramID(&lightingShader);

        float k_d = 0.1f;
        objectManager.SetLightsUniform();
        objectManager.SetFloat("diffuseMaterial", k_d);
        objectManager.SetUniformVector("emissive", glm::vec3(0.0f));
        objectManager.SetUniformVector("cameraPosition", cam.GetEyeVector3());

        scaleVector = glm::vec3(0.05f);
        translate = glm::vec3(distance, 0.f, 0.f);
        rotateVector = glm::vec3(0.f, 1.0f, 0.f);
        angleOfRotation = 0.f;

        for (int i = 0; i < numlights; ++i)
        {
            createdSphere[i] = objectManager.SetTransformsForRotation(ObjectTypeEnum::eCREATE_SPHERE, translate, scaleVector, angleOfRotation + rotationAngle, rotateVector);
            objectManager.GetLight()->SetLightPosition(i, centerModelTranslate, translate, rotateVector, angleOfRotation + rotationAngle);
            objectManager.DrawObject(ObjectTypeEnum::eSPHERE, skybox_projection, camera, objectManager.GetLight()->GetLightInformation(i).diffuseColor, false);
            angleOfRotation += (TWOPI / numlights);
        }
        objectManager.SetLightsUniform();

        objectManager.GetFrameManager()->AttachTexture(i);
        objectManager.GetFrameManager()->CheckFrameBufferStatus();
    }

    objectManager.GetFrameManager()->UnBindFrameBuffer();

    glDisable(GL_DEPTH_TEST);

    glUseProgram(cubeShader);
    objectManager.SetShaderProgramID(&cubeShader);
    camera = CameraToWorld(cam);
    objectManager.GetVboManager()->UseSkyTextures();
    cube = objectManager.SetTransforms(ObjectTypeEnum::eCUBE, centerModelTranslate, centerModelScale, 0.f, rotateVector);
    objectManager.DrawObject(ObjectTypeEnum::eCUBE, skybox_projection, camera, RED, false);
   
    glEnable(GL_DEPTH_TEST);

    /////////////////////////////////////draw orbit////////////////////////////////////////////////////////////////////
    glUseProgram(programID);
    objectManager.SetShaderProgramID(&programID);
    glm::mat4 orbitMat = glm::mat4(0.50f);
    objectManager.SetUniforms(projection, camera, orbitMat);
    objectManager.DrawOrbit();

    ////////////////////////////////////////////draw lights////////////////////////////////////////////////////////////
    glUseProgram(lightingShader);
    objectManager.SetShaderProgramID(&lightingShader);

    scaleVector = glm::vec3(0.05f);
    translate = glm::vec3(distance, 0.f, 0.f);
    rotateVector = glm::vec3(0.f, 1.0f, 0.f);
    angleOfRotation = 0.f;

    for (int i = 0; i < numlights; ++i)
    {
        createdSphere[i] = objectManager.SetTransformsForRotation(ObjectTypeEnum::eCREATE_SPHERE, translate, scaleVector, angleOfRotation + rotationAngle, rotateVector);
        objectManager.GetLight()->SetLightPosition(i, centerModelTranslate, translate, rotateVector, angleOfRotation + rotationAngle);
        objectManager.DrawObject(ObjectTypeEnum::eSPHERE, projection, camera, objectManager.GetLight()->GetLightInformation(i).diffuseColor, false);
        angleOfRotation += (TWOPI / numlights);
    }
    objectManager.SetLightsUniform();

    ///////////////////////////////////////draw bunny with environment mapping///////////////////////////////////////////
    glUseProgram(environmentShader);
    objectManager.SetShaderProgramID(&environmentShader);

    blendingPhong = imGui->GetBlendingPhongValue();
    environmentType = imGui->GetEnvironmentMapping();
    refractionIndex = imGui->GetCenterObjectRefractionIndex();
    chromatic = imGui->GetChromatic();
    fresnel = imGui->GetFresnelPower();

    k_a = imGui->GetAmbientMaterial();
    emissive = imGui->GetEmissiveMaterial();
    k_d = 0.5f;
    objectManager.SetFloat("ambientStrength", k_d);
    objectManager.SetUniformVector("ambientMaterial", k_a);
    objectManager.SetUniformVector("emissive", emissive);

    objectManager.SetEnvironmentUniform(fresnel, blendingPhong, environmentType, cam.GetEyeVector3(), refractionIndex, chromatic);
    objectManager.GetFrameManager()->UseSkyTextures(&environmentShader);
    angleOfRotation = 0.f;
    rotateVector = glm::vec3(0.f, 1.0f, 0.f);
    bunny = objectManager.SetTransforms(ObjectTypeEnum::eSPHERE, centerModelTranslate, centerModelScale, angleOfRotation, rotateVector);
    objectManager.DrawObject(ObjectTypeEnum::eSPHERE, projection, camera, RED, false);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(0);

    imGui->ImGuiRender(&objectManager);
    if (imGui->GetReloadButton() == true)
    {
        programID = ReLoadShaders(&programID, "../Common/shaders/VertexShader.vert", "../Common/shaders/FragmentShader.frag");
        phongLightingShader = ReLoadShaders(&phongLightingShader, "../Common/shaders/PhongLighting.vert", "../Common/shaders/PhongLighting.frag");
        phongShadingShader = ReLoadShaders(&phongShadingShader, "../Common/shaders/PhongShading.vert", "../Common/shaders/PhongShading.frag");
        blinnShadingShader = ReLoadShaders(&blinnShadingShader, "../Common/shaders/BlinnShading.vert", "../Common/shaders/BlinnShading.frag");
        lightingShader = ReLoadShaders(&lightingShader, "../Common/shaders/LightObject.vert", "../Common/shaders/LightObject.frag");
        cubeShader = ReLoadShaders(&cubeShader, "../Common/shaders/skybox.vert", "../Common/shaders/skybox.frag");
        environmentShader = ReLoadShaders(&environmentShader, "../Common/shaders/EnvironmentMapping.vert", "../Common/shaders/EnvironmentMapping.frag");
    }

    return 0;
}

int CS300_Project_Scene::postRender()
{
    if (imGui->GetRotationOrbitButton())
    {
        rotationAngle += imGui->GetOrbitSpeed();
    }
    return 0;
}

void CS300_Project_Scene::InitMembers()
{
    programID = 0;
}

void CS300_Project_Scene::SetupNanoGUI(GLFWwindow* window)
{
    imGui = new ImGuiManager(window);
}

void CS300_Project_Scene::SetupBuffers()
{
    objectManager.GetFrameManager()->SetWindowSize(windowWidth, windowHeight);

    objectManager.Init();
    objectManager.SetShaderProgramID(&programID);

    return;
}
