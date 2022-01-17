/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectManager.h
Purpose: This file is header file for ObjectManager class.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/

#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "VBOManager.h"
#include "FrameBuffer.h"
#include "ObjectTypeEnum.h"
#include "Mesh.h"
#include "Light.h"

class ObjectManager
{
public:
    ObjectManager();
    ~ObjectManager();

    void Init();
    void CleanUP();

    void AddObject(std::string name, glm::mat4* transform);

    glm::mat4 SetTransforms(ObjectTypeEnum type, glm::vec3 translate, glm::vec3 scale, float rotation, glm::vec3 rotationVector);
    glm::mat4 SetTransformsForRotation(ObjectTypeEnum type, glm::vec3 translate, glm::vec3 scale, float rotation, glm::vec3 rotationVector);
    
    void DrawObject(ObjectTypeEnum type, glm::mat4 projection, glm::mat4 camera, glm::vec3 color, bool isTexture);

    void SetVersionOfNormalBased(bool vertexVersion);
    bool GetVersionOfNormalBased();

    void DrawVertexNormalVector(LineTypeEnum type);
    void SetDrawVertexNormalVector(bool draw);
    bool GetDrawVertexNormalVector();

    void DrawFaceNormalVector(ObjectTypeEnum type);
    void SetDrawFaceNormalVector(bool draw);
    bool GetDrawFaceNormalVector();

    void UseTextures(bool useUV);

    void SetEnvironmentUniform(float fresnel, float blending, int type, glm::vec3 viewPosition, float refractiveIndex, float chromaticIndex);

    void SetGlobalUniform(int numlights, float far, float near, glm::vec3 attenuation, glm::vec3 fogColor, glm::vec3 globalAmbientColor);
    void SetLightsUniform();
    void SetLightsUniform(int index);

    void SetUniforms(glm::mat4 ndc, glm::mat4 camera);
    void SetUniforms(glm::mat4 ndc, glm::mat4 camera, glm::mat4 tramsform);
    void SetColor(glm::vec3 color);
    void SetLightPosition(glm::vec3 lightPos);
    void SetUniformMatrix(const GLchar* location, glm::mat4 matrix);
    void SetUniformVector(const GLchar* location, glm::vec3 vec);
    void SetInt(const GLchar* location, int value);
    void SetFloat(const GLchar* location, float value);

    void SetShaderProgramID(GLuint* id);

    void DrawOrbit();

    void SetLightInformation(int index, LightInfo info);
    void SetGlobalInformation(GlobalInfo info);
    LightInfo GetLightInformation(int index);
    GlobalInfo GetGlobalInformation();
    Light* GetLight();

    Mesh* GetMesh(int index);
    VBOManager* GetVboManager();
    FrameBuffer* GetFrameManager();

private:
    GLuint* programID = nullptr;

    VBOManager vboManager;
    FrameBuffer fboManager;

    std::map<std::string, glm::mat4*> objects;

    bool isDrawVertexNormalBased = true;

    bool drawVertexNormalVector = false;
    bool drawFaceNormalVector = false;

    Mesh* sphere = nullptr;
    Mesh* sphere_modified = nullptr;
    Mesh* cube = nullptr;
    Mesh* bunny = nullptr;
    Mesh* cloud = nullptr;
    Mesh* quad = nullptr;

    std::vector<glm::vec3> orbitVertex;
    GLuint vertexArrayID = 0;
    GLuint orbitBuffer = 0;

    Light* lights;
};