/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: CS300_Project_Scene.hpp
Purpose: This file is header file for creating assignment scene.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-18-2021
End Header --------------------------------------------------------*/
#pragma once

#include "../Common/Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Mesh.h"
#include "ImGuiManager.hpp"
#include "VBOManager.h"
#include "Camera.h"
#include "Projection.h"
#include "ObjectManager.h"
#include "Light.h"

const int SPHERE_OBJECT_NUM = 16;

class CS300_Project_Scene :public Scene
{
public:
    CS300_Project_Scene() = default;
    CS300_Project_Scene(int windowWidth, int windowHeight);
    virtual ~CS300_Project_Scene();

    int Init() override;
    void CleanUp() override;

    void ResizeWindow(int width, int height) override;
    void KeyboardInput(GLFWwindow* window) override;
    void MouseInput(GLFWwindow* window, double xPos, double yPos) override;

    int Render(GLFWwindow* pWwindow) override;
    int postRender() override;

private:
    void InitMembers();
    void SetupNanoGUI(GLFWwindow* window) override;
    void SetupBuffers();

    GLuint programID = 0;
    GLuint phongLightingShader = 0;
    GLuint phongShadingShader = 0;
    GLuint blinnShadingShader = 0;
    GLuint lightingShader = 0;
    GLuint cubeShader = 0;
    GLuint environmentShader = 0;

    ImGuiManager* imGui = nullptr;

    int windowWidth = 0, windowHeight = 0;
    int tempWidth = 0, tempHeight = 0;
    double currX = 0.f, currY = 0.f;

    Camera cam;
    Camera skybox_cam;
    glm::mat4 camera;
    glm::mat4 projection;
    glm::mat4 skybox_projection;

    ObjectManager objectManager;

    float rotationAngle = 0.f;

    glm::mat4 bunny = glm::mat4(1.0f);
    glm::mat4 createdSphere[SPHERE_OBJECT_NUM] = { glm::mat4(1.0f) };
    glm::mat4 sky = glm::mat4(1.0f);
    glm::mat4 sphereModified = glm::mat4(1.0f);
    glm::mat4 sphere = glm::mat4(1.0f);
    glm::mat4 cube = glm::mat4(1.0f);


    int shader_type;
    bool calculate_UV_GPU;
    int projection_mode;
    bool useUV;
    bool entities;

    float blendingPhong;
    int environmentType;
    float refractionIndex;
    float chromatic;
    float fresnel;

    glm::vec3 k_a;
    glm::vec3 emissive;

};