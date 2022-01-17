/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Light.h
Purpose: This file is header file for Light class,
This file has Light information(LightInfo struct) and Global information(GlbalInfo).
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-18-2021
End Header --------------------------------------------------------*/
#pragma once

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

const int MAX_NUMBER_OF_LIGHTS = 16;
struct LightInfo
{
    int lightType = 0;              // point, dir, spot
    float innerAngle = 30.f;           // spot
    float outerAngle = 45.f;           // spot
    float fallOff = 1.f;              // spot
    glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);     // dir, point, spot
    glm::vec3 diffuseColor = glm::vec3(1.f, 1.f, 1.f);     // dir, point, spot
    glm::vec3 specularColor = glm::vec3(1.f, 1.f, 1.f);    // dir, point, spot
    glm::vec3 lightPosition = glm::vec3(1.f, 1.f, 0.f);    // point
    glm::vec3 direction = glm::vec3(10.f, 0.f, 0.f);        // dir, spot
};

struct GlobalInfo
{
    float fogFar = 0.1f;
    float fogNear = 20.f;
    glm::vec3 attenuation = glm::vec3(1.f, 0.22f, 0.22f);
    glm::vec3 fogColor = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 globalAmbientColor = glm::vec3(0.f, 0.f, 0.2f);
};


class Light
{
public:
    Light();
    ~Light();

    void SetLightInformation(int index, LightInfo info);
    LightInfo GetLightInformation(int index);

    int GetNumberOfLights();
    void SetNumberOfLights(int num);

    void SetGlobalInformation(GlobalInfo info);
    GlobalInfo GetGlobalInformation();

    void SetLightPosition(int index, glm::vec3 modelPos, glm::vec3 translate, glm::vec3 rotationXis, float angle);
    glm::vec3 GetLightPosition(int index);
    glm::vec3 GetLightDirection(int index);


private:
    int numberOfLights;
    LightInfo lights[MAX_NUMBER_OF_LIGHTS];

    GlobalInfo globalInformation;

};