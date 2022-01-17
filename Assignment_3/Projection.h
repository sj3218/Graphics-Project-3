/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Projection.h
Purpose: This file is header file for camera and ndc matrix.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#pragma once
#include "Camera.h"
#include <glm/glm.hpp>

glm::mat4 CameraToWorld(const Camera& cam);
glm::mat4 CameraToNDC(const Camera& cam);