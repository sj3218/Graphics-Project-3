/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VAOManager.h
Purpose: This file is header file for VAOManager class.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#pragma once

#include <GL/glew.h>
#include "ObjectTypeEnum.h"

const int VAOCount = static_cast<int>(ObjectTypeEnum::COUNT);
const int VAOLineCount = static_cast<int>(LineTypeEnum::COUNT);

class VAOManager
{
public:
    VAOManager();
    ~VAOManager();

    void GenerateVAO();
    void CleanUPVAO();

    void UseVertexArrayID(ObjectTypeEnum objType);
    void UseVertexArrayID(LineTypeEnum objType);

    void UseFaceVertexArrayID(ObjectTypeEnum objType);
    void UseFaceNormalVertexArrayID(ObjectTypeEnum objType);

    void UnUseVertexArrayID();

private:
    GLuint vertexArrayID[VAOCount] = {0};
    GLuint lineVertexArrayID[VAOLineCount] = { 0 };

    GLuint faceVertexArrayID[VAOCount] = { 0 };
    GLuint faceNormalVertexArrayID[VAOCount] = { 0 };
};