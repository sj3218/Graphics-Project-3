/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: FrameBuffer.cpp
Purpose: This file is source file for Framebuffer class. There are functions for managing frame buffer
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 11-20-2021
End Header --------------------------------------------------------*/
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
    frameBuffer = 0;
    renderedTexture[0] = { 0 };
    renderBuffer = 0;

    windowWidth = 0;
    windowHeight = 0;
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::SetUpBuffers()
{
    GenerateBuffers();
    SetBuffers();
}

void FrameBuffer::GenerateBuffers()
{
    glGenFramebuffers(1, &frameBuffer);
    glGenTextures(6, renderedTexture);
    glGenRenderbuffers(1, &renderBuffer);
}

void FrameBuffer::SetBuffers()
{
    BindFrameBuffer();

    for (int i = 0; i < 6; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, renderedTexture[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        AttachTexture(i);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    //GLenum drawBuffers[6] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
    //                            GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };

    //glDrawBuffers(6, drawBuffers);
    glDrawBuffers(1, drawBuffers);

    CheckFrameBufferStatus();

    UnBindFrameBuffer();
}

void FrameBuffer::CleanUPBuffers()
{
    for (int i = 0; i < 6; ++i)
    {
        glDeleteTextures(6, renderedTexture);
    }
    glDeleteRenderbuffers(1, &renderBuffer);
    glDeleteFramebuffers(1, &frameBuffer);
}

void FrameBuffer::CheckFrameBufferStatus()
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "FrameBuffer error: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
    }
}

void FrameBuffer::BindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void FrameBuffer::UnBindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::AttachTexture(int index)
{
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture[index], 0);
}

void FrameBuffer::SetWindowSize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

void FrameBuffer::UseSkyTextures(GLuint* programID)
{
    for (int i = 0; i < 6; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, renderedTexture[i]);
        std::string stringName = "skyTexture[" + std::to_string(i) + "]";
        GLint textureLocation = glGetUniformLocation(*programID, stringName.c_str());
        glUniform1i(textureLocation, i);
    }
}
