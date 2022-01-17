/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.h
Purpose: This file is header file for Camera class.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
class Camera
{
public:
    Camera();

    void CameraSetUP(glm::vec3 eyePosition, glm::vec3 lookPosition, glm::vec3 upVector, float fov, float aspect, float near, float far);

    glm::vec4 Viewport()const;

    glm::vec4 GetRightVector()const;
    glm::vec4 GetUpVector()const;
    glm::vec4 GetBackVector()const;
    glm::vec4 GetEyePoint()const;

    glm::vec3 GetEyeVector3()const;
    //void SetEyeVector();

    float GetNearDistance()const;
    float GetFarDistance()const;
    
    Camera& Yaw(float angle);
    Camera& Pitch(float angle);

private:
    glm::vec3 eye;//position

    glm::vec3 look;//vector

    glm::vec3 right, up, back;//vector

    glm::vec4 tempRight, tempUp, tempBack;

    float width, height, distance, nearDistance, farDistance;
};