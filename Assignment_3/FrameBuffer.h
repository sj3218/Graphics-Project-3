/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: FrameBuffer.h
Purpose: This file is header file for FrameBuffer class.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 11-20-2021
End Header --------------------------------------------------------*/
#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();

    void SetUpBuffers();
    void GenerateBuffers();
    void SetBuffers();
    void CleanUPBuffers();

    void CheckFrameBufferStatus();

    void BindFrameBuffer();
    void UnBindFrameBuffer();

    void AttachTexture(int index);

    void SetWindowSize(int width, int height);

    void UseSkyTextures(GLuint* programID);

private:
    GLuint frameBuffer;
    GLuint renderedTexture[6];
    GLuint renderBuffer;
    

    int windowWidth, windowHeight = 0;
};