/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VAOManager.cpp
Purpose: This file is source file for VAOManager class. This files manages for vertex array id.
In this file, VAOs are created, deleted and binded.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#include "VAOManager.h"

VAOManager::VAOManager()
{
}

VAOManager::~VAOManager()
{
    
}

void VAOManager::GenerateVAO()
{
    glGenVertexArrays(VAOCount, vertexArrayID);
    glGenVertexArrays(VAOLineCount, lineVertexArrayID);
    glGenVertexArrays(VAOCount, faceVertexArrayID);
    glGenVertexArrays(VAOCount, faceNormalVertexArrayID);
}

void VAOManager::CleanUPVAO()
{
    glDeleteVertexArrays(VAOCount, vertexArrayID);
    glDeleteVertexArrays(VAOLineCount, lineVertexArrayID);
    glDeleteVertexArrays(VAOCount, faceVertexArrayID);
    glDeleteVertexArrays(VAOCount, faceNormalVertexArrayID);
}

void VAOManager::UseVertexArrayID(ObjectTypeEnum objType)
{
    int objectIndex = static_cast<int>(objType);
    glBindVertexArray(vertexArrayID[objectIndex]);
}

void VAOManager::UseVertexArrayID(LineTypeEnum objType)
{
    int lineIndex = static_cast<int>(objType);
    glBindVertexArray(lineVertexArrayID[lineIndex]);
}

void VAOManager::UseFaceVertexArrayID(ObjectTypeEnum objType)
{
    int objectIndex = static_cast<int>(objType);
    glBindVertexArray(faceVertexArrayID[objectIndex]);
}

void VAOManager::UseFaceNormalVertexArrayID(ObjectTypeEnum objType)
{
    int faceIndex = static_cast<int>(objType);
    glBindVertexArray(faceNormalVertexArrayID[faceIndex]);
}

void VAOManager::UnUseVertexArrayID()
{
    glBindVertexArray(0);
}
