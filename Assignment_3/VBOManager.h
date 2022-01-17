/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VBOManager.h
Purpose: This file is header file for VBOManager class.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "ObjectTypeEnum.h"
#include "Mesh.h"
#include "OBJReader.h"
#include "VAOManager.h"

const int vertex = 0,
normal = 1,
texture = 2,
indices = 3;

class VBOManager
{
public:
    VBOManager();
    ~VBOManager();

    void CleanUPBuffers();

    void LoadOBJFile();
    void SetUpBuffers();

    void UseVertexBuffers(ObjectTypeEnum objType);
    void UnUseVertexBuffers();

    void UseLineBuffers(LineTypeEnum objType);
    void UnUseLineBuffers();

    void UseFaceVertexBuffers(ObjectTypeEnum objType);
    void UnUseFaceVertexBuffers();

    void UseFaceNormalBuffers(ObjectTypeEnum objType);
    void UnUseFaceNormalBuffers();

    void UseUBOGlobal(int numlights, float far, float near, glm::vec3 attenuation, glm::vec3 fogColor, glm::vec3 globalAmbientColor);
    void UseUBOLight(int index, int lightType, float innerAngle, float outerAngle, float fallOff,
                    glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3 lightPosition, glm::vec3 direction);
    void UnUseUBO();

    void UseTextures(bool useUV);
    void UseSkyTextures();

    void SetUVBuffers(ObjectTypeEnum type,  Mesh* mesh);

    void DrawLine(LineTypeEnum type);
    void DrawMesh(ObjectTypeEnum type, bool isTexture);

    void DrawFaceMesh(ObjectTypeEnum type);
    void DrawFaceNormalLine(ObjectTypeEnum type);

    Mesh* FindMesh(ObjectTypeEnum type);
    Mesh* FindMesh(LineTypeEnum type);

    void SetShaderProgramID(GLuint* id);

    void SetWindowSize(int width, int height);
    void ResizeSkyTextures();

private:
    void GenerateVBO();
    void SetVertexBuffer();
    void SetSkyBoxFile();

    void SetUVBuffers(GLuint* buffer, Mesh* mesh);
    void SetArrayBuffers(GLuint* buffer, int type);
    void SetIndexBuffers(GLuint* buffer, int type);
    void SetLineBuffers(GLuint* buffer, int type);
    void SetFaceVertexBuffers(GLuint* buffer, int type);
    void SetFaceNormalBuffers(GLuint* buffer, int type);

    void SetUniformBuffers(GLuint* buffer, int index, int size);
    void SetTextureBuffers(GLuint* buffer, const char* file, int type);
    void SetSkyBoxTextureBuffers();

    void CalculateOrbit(float distance);
    
    VAOManager vaoManager;
    GLuint* programID=nullptr;

    std::vector<GLuint*> vertexBuffer;
    std::vector<GLuint*> lineBuffer;
    std::vector<GLuint*> faceVertexBuffer;
    std::vector<GLuint*> faceNormalBuffer;

    OBJReader objReader;

    GLuint vboSphere[4];
    GLuint vboSphereModified[4];
    GLuint vboSphere_4[4];
    GLuint vboCube[4];
    GLuint vboBunny[4];
    GLuint vboCreateSphere[4];
    GLuint vboQuad[4];

    GLuint vboSphereLine[1];
    GLuint vboSphereModifiedLine[1];
    GLuint vboSphere_4Line[1];
    GLuint vboCubeLine[1];
    GLuint vboBunnyLine[1];
    GLuint vboCreateSphereLine[1];
    GLuint vboQuadLine[1];
    GLuint vboOrbitLine[1];

    GLuint vboSphereFace[3];
    GLuint vboSphereModifiedFace[3];
    GLuint vboSphere_4Face[3];
    GLuint vboCubeFace[3];
    GLuint vboBunnyFace[3];
    GLuint vboCreateSphereFace[3];
    GLuint vboQuadFace[3];

    GLuint vboSphereFaceNormal[1];
    GLuint vboSphereModifiedFaceNormal[1];
    GLuint vboSphere_4FaceNormal[1];
    GLuint vboCubeFaceNormal[1];
    GLuint vboBunnyFaceNormal[1];
    GLuint vboCreateSphereFaceNormal[1];
    GLuint vboQuadNormal[1];
    
    GLuint uboGlobal;
    GLuint uboLight;
    GLuint UVTexture;
    GLuint diffuseTexture;
    GLuint specularTexture;

    std::vector<std::string> skyBoxesString;
    GLuint skyTextures[6];

    Mesh sphere;
    Mesh sphereModified;
    Mesh sphere_4;
    Mesh cube;
    Mesh bunny;
    Mesh createSphere;
    Mesh quad;

    std::vector<glm::vec3> orbitVertex;
   
    int windowWidth, windowHeight = 0;
};