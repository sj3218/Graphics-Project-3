/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Light.cpp
Purpose: This file is source file for Light class, 
This file implements light information and global information.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-18-2021
End Header --------------------------------------------------------*/
#include "Light.h"

Light::Light()
{
    numberOfLights = MAX_NUMBER_OF_LIGHTS;
}

Light::~Light()
{
}

void Light::SetLightInformation(int index, LightInfo info)
{
    lights[index].lightType = info.lightType;
    lights[index].innerAngle = info.innerAngle;
    lights[index].outerAngle = info.outerAngle;
    lights[index].fallOff = info.fallOff;
    lights[index].ambientColor = info.ambientColor;
    lights[index].diffuseColor = info.diffuseColor;
    lights[index].specularColor = info.specularColor;
    lights[index].lightPosition = info.lightPosition;
    lights[index].direction = info.direction;
}

LightInfo Light::GetLightInformation(int index)
{
    return lights[index];
}

int Light::GetNumberOfLights()
{
    return numberOfLights;
}

void Light::SetNumberOfLights(int num)
{
    numberOfLights = num;
}

void Light::SetGlobalInformation(GlobalInfo info)
{
    globalInformation.fogFar = info.fogFar;
    globalInformation.fogNear = info.fogNear;
    globalInformation.fogColor = info.fogColor;
    globalInformation.attenuation = info.attenuation;
    globalInformation.globalAmbientColor = info.globalAmbientColor;
}

GlobalInfo Light::GetGlobalInformation()
{
    return globalInformation;
}

void Light::SetLightPosition(int index, glm::vec3 modelPos, glm::vec3 translate, glm::vec3 rotationXis, float angle)
{
    glm::vec4 pos = glm::rotate(angle, rotationXis) * glm::vec4(translate, 1.f);

    lights[index].lightPosition = glm::vec3(pos.x, pos.y, pos.z);
    lights[index].direction = modelPos - lights[index].lightPosition;
}

glm::vec3 Light::GetLightPosition(int index)
{
    return lights[index].lightPosition;
}

glm::vec3 Light::GetLightDirection(int index)
{
    return lights[index].direction;
}

