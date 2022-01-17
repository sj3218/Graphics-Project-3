/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectManager.cpp
Purpose: This file is source file for ObjectManager class. This file manages for objects and vbomanager.
There are functions for drawing objects and setting uniforms values and matrix for shader.
And also in this file, vboManager can load obj files, set up buffers and clean up buffers.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/

#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
    lights = new Light;
}

ObjectManager::~ObjectManager()
{
    delete lights;
}

void ObjectManager::Init()
{
    vboManager.LoadOBJFile();
    vboManager.SetUpBuffers();
    fboManager.SetUpBuffers();

    sphere = vboManager.FindMesh(ObjectTypeEnum::eSPHERE);
    bunny = vboManager.FindMesh(ObjectTypeEnum::eBUNNY);
    cloud = vboManager.FindMesh(ObjectTypeEnum::e4SPHERE);
    sphere_modified = vboManager.FindMesh(ObjectTypeEnum::eSPHERE_MODIFIED);
    cube = vboManager.FindMesh(ObjectTypeEnum::eCUBE);
    quad = vboManager.FindMesh(ObjectTypeEnum::eQUAD);

    lights->SetNumberOfLights(1);

    GlobalInfo global_info;

    global_info.attenuation = glm::vec3(1.f, 0.22f, 0.22f);
    global_info.fogColor = glm::vec3( 0.5f,0.5f,0.5f );
    global_info.globalAmbientColor = glm::vec3(0.f, 0.f, 0.2f);
    global_info.fogFar = 20.f;
    global_info.fogNear = 0.1f;

    lights->SetGlobalInformation(global_info);

    LightInfo light_info;
    light_info.lightType = 1;
    light_info.innerAngle = 30.f;
    light_info.outerAngle = 45.f;
    light_info.fallOff = 1.f;
    light_info.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    light_info.diffuseColor = glm::vec3(1.f, 1.f, 1.f);
    light_info.specularColor = glm::vec3(1.f, 1.f, 1.f);
    light_info.lightPosition = glm::vec3(1.f, 1.f, 0.f);
    light_info.direction = glm::vec3(10.f, 0.f, 0.f);

    lights->SetLightInformation(0, light_info);
}

void ObjectManager::CleanUP()
{
    vboManager.CleanUPBuffers();
    fboManager.CleanUPBuffers();
}

void ObjectManager::AddObject(std::string name, glm::mat4* transform)
{
    objects.insert(std::make_pair(name, transform));
}

void ObjectManager::DrawObject(ObjectTypeEnum type, glm::mat4 projection, glm::mat4 camera, glm::vec3 color, bool isTexture)
{
    SetUniforms(projection, camera);
    SetColor(color);

    if (isDrawVertexNormalBased == true)
    {
        vboManager.UseVertexBuffers(type);
        vboManager.DrawMesh(type, isTexture);
        vboManager.UnUseVertexBuffers();
    }
    else
    {
        vboManager.UseFaceVertexBuffers(type);
        vboManager.DrawFaceMesh(type);
        vboManager.UnUseFaceVertexBuffers();
    }

    int index = static_cast<int>(type);

    if (drawVertexNormalVector)
    {
        LineTypeEnum lineType = static_cast<LineTypeEnum>(type);
        DrawVertexNormalVector(lineType);
    }

    if (drawFaceNormalVector)
    {
        DrawFaceNormalVector(type);
    }
}

void ObjectManager::SetVersionOfNormalBased(bool vertexVersion)
{
    isDrawVertexNormalBased = vertexVersion;
}

bool ObjectManager::GetVersionOfNormalBased()
{
    return isDrawVertexNormalBased;
}

void ObjectManager::DrawVertexNormalVector(LineTypeEnum type)
{
    vboManager.UseLineBuffers(type);
    vboManager.DrawLine(type);
    vboManager.UnUseLineBuffers();
}

void ObjectManager::SetDrawVertexNormalVector(bool draw)
{
    drawVertexNormalVector = draw;
}

bool ObjectManager::GetDrawVertexNormalVector()
{
    return drawVertexNormalVector;
}

void ObjectManager::DrawFaceNormalVector(ObjectTypeEnum type)
{
    vboManager.UseFaceNormalBuffers(type);
    vboManager.DrawFaceNormalLine(type);
    vboManager.UnUseFaceNormalBuffers();
}

void ObjectManager::SetDrawFaceNormalVector(bool draw)
{
    drawFaceNormalVector = draw;
}

bool ObjectManager::GetDrawFaceNormalVector()
{
    return drawFaceNormalVector;
}

void ObjectManager::UseTextures(bool useUV)
{
    vboManager.UseTextures(useUV);
}

void ObjectManager::SetEnvironmentUniform(float fresnel, float blending, int type, glm::vec3 viewPosition, float refractiveIndex, float chromaticIndex)
{
    SetFloat("fresnelPower", fresnel);
    SetFloat("blending", blending);
    SetInt("environmentType", type);
    SetUniformVector("viewPosition", viewPosition);
    SetFloat("refractiveIndex", refractiveIndex);
    SetFloat("chromatic", chromaticIndex);
}

void ObjectManager::SetGlobalUniform(int numlights, float far, float near, glm::vec3 attenuation, glm::vec3 fogColor, glm::vec3 globalAmbientColor)
{
    vboManager.UseUBOGlobal(numlights, far, near, attenuation, fogColor, globalAmbientColor);
}

void ObjectManager::SetLightsUniform()
{
    int numberOflights = lights->GetNumberOfLights();

    for (int i = 0; i < numberOflights; ++i)
    {
        LightInfo light = lights->GetLightInformation(i);
        vboManager.UseUBOLight(i, light.lightType, light.innerAngle, light.outerAngle, light.fallOff, light.ambientColor, light.diffuseColor, light.specularColor, light.lightPosition, light.direction);
    }
}

void ObjectManager::SetLightsUniform(int index)
{
    LightInfo light = lights->GetLightInformation(index);
    vboManager.UseUBOLight(index, light.lightType, light.innerAngle, light.outerAngle, light.fallOff, light.ambientColor, light.diffuseColor, light.specularColor, light.lightPosition, light.direction);
}

void ObjectManager::SetUniforms(glm::mat4 ndc, glm::mat4 camera)
{
    SetUniformMatrix("ndcMatrix", ndc);
    SetUniformMatrix("cameraMatrix", camera);
}

void ObjectManager::SetUniforms(glm::mat4 ndc, glm::mat4 camera, glm::mat4 tramsform)
{
    SetUniformMatrix("ndcMatrix", ndc);
    SetUniformMatrix("cameraMatrix", camera);
    SetUniformMatrix("transform", tramsform);
}

glm::mat4 ObjectManager::SetTransforms(ObjectTypeEnum type, glm::vec3 translate, glm::vec3 scale, float rotation, glm::vec3 rotationVector)
{
    glm::mat4 modelMat = glm::translate(translate) * glm::rotate(rotation, rotationVector) * glm::scale(scale);
    SetUniformMatrix("transform", modelMat);
    return modelMat;
}

glm::mat4 ObjectManager::SetTransformsForRotation(ObjectTypeEnum type, glm::vec3 translate, glm::vec3 scale, float rotation, glm::vec3 rotationVector)
{
    glm::mat4 modelMat = glm::rotate(rotation, rotationVector) * glm::translate(translate) * glm::scale(scale);
    SetUniformMatrix("transform", modelMat);
    return modelMat;
}

void ObjectManager::SetColor(glm::vec3 color)
{
    GLint uniformLocation = glGetUniformLocation(*programID, "color");
    glUniform3f(uniformLocation, color.x, color.y, color.z);
}

void ObjectManager::SetLightPosition(glm::vec3 lightPos)
{
    SetUniformVector("lightPosition", lightPos);
}

void ObjectManager::SetUniformMatrix(const GLchar* location, glm::mat4 matrix)
{
    GLint uniformLocation = glGetUniformLocation(*programID, location);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &matrix[0][0]);
}

void ObjectManager::SetUniformVector(const GLchar* location, glm::vec3 vec)
{
    GLint uniformLocation = glGetUniformLocation(*programID, location);
    glUniform3f(uniformLocation, vec.x, vec.y, vec.z);
}

void ObjectManager::SetInt(const GLchar* location, int value)
{
    GLint uniformLocation = glGetUniformLocation(*programID, location);
    glUniform1i(uniformLocation, value);
}

void ObjectManager::SetFloat(const GLchar* location, float value)
{
    GLint uniformLocation = glGetUniformLocation(*programID, location);
    glUniform1f(uniformLocation, value);
}

void ObjectManager::SetShaderProgramID(GLuint* id)
{
    programID = id;
    vboManager.SetShaderProgramID(id);
}

void ObjectManager::DrawOrbit()
{
    vboManager.UseLineBuffers(LineTypeEnum::eORBIT);
    vboManager.DrawLine(LineTypeEnum::eORBIT);
    vboManager.UnUseLineBuffers();
}

void ObjectManager::SetLightInformation(int index, LightInfo info)
{
    lights->SetLightInformation(index, info);
}

void ObjectManager::SetGlobalInformation(GlobalInfo info)
{
    lights->SetGlobalInformation(info);
}

LightInfo ObjectManager::GetLightInformation(int index)
{
    return lights->GetLightInformation(index);
}

GlobalInfo ObjectManager::GetGlobalInformation()
{
    return lights->GetGlobalInformation();
}

Light* ObjectManager::GetLight()
{
    return lights;
}

Mesh* ObjectManager::GetMesh(int index)
{
    if (index == 0)
    {
        return sphere;
    }
    else if (index == 1)
    {
        return sphere_modified;
    }
    else if (index == 2)
    {
        return cube;
    }
    else if (index == 3)
    {
        return bunny;
    }
    else if (index == 4)
    {
        return cloud;
    }
    else if (index == 5)
    {
        return quad;
    }

    return nullptr;
}

VBOManager* ObjectManager::GetVboManager()
{
    return &vboManager;
}

FrameBuffer* ObjectManager::GetFrameManager()
{
    return &fboManager;
}


