/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Projection.cpp
Purpose: This file is source file for camera and ndc matrix. 
I use inverse matrix and CameraToWorld function for calculating world to camera.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#include "Projection.h"

glm::mat4 CameraToWorld(const Camera& cam)
{
    return glm::mat4(cam.GetRightVector(), cam.GetUpVector(), cam.GetBackVector(), cam.GetEyePoint());
}

glm::mat4 CameraToNDC(const Camera& cam)
{
    glm::mat4 ndc(0.f);

    glm::vec4 viewport = cam.Viewport();
    float viewportWidth = viewport.x;
    float viewportHeight = viewport.y;
    float viewportDistanceTwice = viewport.z * 2.f;

    float nearPlane = cam.GetNearDistance();
    float farPlane = cam.GetFarDistance();

    ndc[0].x = viewportDistanceTwice / viewportWidth;
    ndc[1].y = viewportDistanceTwice / viewportHeight;
    ndc[2].z = (nearPlane + farPlane) / (nearPlane - farPlane);
    ndc[2].w = (2 * nearPlane * farPlane) / (nearPlane - farPlane);
    ndc[3].z = -1.f;
    
    return ndc;
}
