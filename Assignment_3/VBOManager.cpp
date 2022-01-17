/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VBOManager.cpp
Purpose: This file is source file for VBOManager class. This file manages for vertex buffers and vaoManager.
In this file, buffers are created, deleted and binded. And It helps drawing objects and lines for objectManager.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/

#include "VBOManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

VBOManager::VBOManager()
{
    SetVertexBuffer();
    SetSkyBoxFile();
}

VBOManager::~VBOManager()
{

}

void VBOManager::CleanUPBuffers()
{
    for (int i = 0; i < vertexBuffer.size(); ++i)
    {
        glDeleteBuffers(3, vertexBuffer.at(i));
    }

    for (int i = 0; i < lineBuffer.size(); ++i)
    {
        glDeleteBuffers(1, lineBuffer.at(i));
    }

    for (int i = 0; i < faceVertexBuffer.size(); ++i)
    {
        glDeleteBuffers(2, faceVertexBuffer.at(i));
    }

    for (int i = 0; i < faceNormalBuffer.size(); ++i)
    {
        glDeleteBuffers(1, faceNormalBuffer.at(i));
    }

    glDeleteBuffers(1, &uboGlobal);
    glDeleteBuffers(1, &uboLight);

    vaoManager.CleanUPVAO();

    glDeleteTextures(1, &diffuseTexture);
    glDeleteTextures(1, &specularTexture);
    glDeleteTextures(1, &UVTexture);

    glDeleteTextures(6, skyTextures);
}

void VBOManager::UseVertexBuffers(ObjectTypeEnum objType)
{
    int objectIndex = static_cast<int>(objType);

    vaoManager.UseVertexArrayID(objType);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.at(objectIndex)[vertex]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.at(objectIndex)[normal]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.at(objectIndex)[texture]);

    glEnableVertexAttribArray(vertex);
    glEnableVertexAttribArray(normal);
    glEnableVertexAttribArray(texture);
}

void VBOManager::UnUseVertexBuffers()
{
    glDisableVertexAttribArray(vertex);
    glDisableVertexAttribArray(normal);
    glDisableVertexAttribArray(texture);
    vaoManager.UnUseVertexArrayID();
}

void VBOManager::UseLineBuffers(LineTypeEnum objType)
{
    int objectIndex = static_cast<int>(objType);

    vaoManager.UseVertexArrayID(objType);
    glEnableVertexAttribArray(vertex);
    glBindBuffer(GL_ARRAY_BUFFER, *lineBuffer.at(objectIndex));
}

void VBOManager::UnUseLineBuffers()
{
    glDisableVertexAttribArray(vertex);
    vaoManager.UnUseVertexArrayID();
}

void VBOManager::UseFaceVertexBuffers(ObjectTypeEnum objType)
{
    int objectIndex = static_cast<int>(objType);

    vaoManager.UseFaceVertexArrayID(objType);
    glBindBuffer(GL_ARRAY_BUFFER, faceVertexBuffer.at(objectIndex)[vertex]);
    glBindBuffer(GL_ARRAY_BUFFER, faceVertexBuffer.at(objectIndex)[normal]);
    glBindBuffer(GL_ARRAY_BUFFER, faceVertexBuffer.at(objectIndex)[texture]);

    glEnableVertexAttribArray(vertex);
    glEnableVertexAttribArray(normal);
    glEnableVertexAttribArray(texture);
}

void VBOManager::UnUseFaceVertexBuffers()
{
    glDisableVertexAttribArray(vertex);
    glDisableVertexAttribArray(normal);
    glDisableVertexAttribArray(texture);
    vaoManager.UnUseVertexArrayID();
}

void VBOManager::UseFaceNormalBuffers(ObjectTypeEnum objType)
{
    int objectIndex = static_cast<int>(objType);

    vaoManager.UseFaceNormalVertexArrayID(objType);
    glEnableVertexAttribArray(vertex);
    glBindBuffer(GL_ARRAY_BUFFER, *faceNormalBuffer.at(objectIndex));
}

void VBOManager::UnUseFaceNormalBuffers()
{
    glDisableVertexAttribArray(vertex);
    vaoManager.UnUseVertexArrayID();
}

void VBOManager::UseUBOGlobal(int numlights, float far, float near, glm::vec3 attenuation, glm::vec3 fogColor, glm::vec3 globalAmbientColor)
{
    glBindBuffer(GL_UNIFORM_BUFFER, uboGlobal);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &numlights);
    glBufferSubData(GL_UNIFORM_BUFFER, 4, sizeof(float), &far);
    glBufferSubData(GL_UNIFORM_BUFFER, 8, sizeof(float), &near);
    glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(glm::vec3), &attenuation);
    glBufferSubData(GL_UNIFORM_BUFFER, 32, sizeof(glm::vec3), &fogColor);
    glBufferSubData(GL_UNIFORM_BUFFER, 48, sizeof(glm::vec3), &globalAmbientColor);
}

void VBOManager::UseUBOLight(int index, int lightType, float innerAngle, float outerAngle, float fallOff, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3 lightPosition, glm::vec3 direction)
{
    int lightStride = 96;
    int lightStrideWithIndex = lightStride * index;

    int firstStride = lightStrideWithIndex + 0;
    int secondStride = lightStrideWithIndex + 4;
    int thirdStride = lightStrideWithIndex + 8;
    int forthStride = lightStrideWithIndex + 12;
    int fifthStride = lightStrideWithIndex + 16;
    int sixStride = lightStrideWithIndex + 32;
    int sevenStride = lightStrideWithIndex + 48;
    int eightStride = lightStrideWithIndex + 64;
    int nineStride = lightStrideWithIndex + 80;

    glBindBuffer(GL_UNIFORM_BUFFER, uboLight);

    glBufferSubData(GL_UNIFORM_BUFFER, firstStride, sizeof(int), &lightType);
    glBufferSubData(GL_UNIFORM_BUFFER, secondStride, sizeof(float), &innerAngle);
    glBufferSubData(GL_UNIFORM_BUFFER, thirdStride, sizeof(float), &outerAngle);
    glBufferSubData(GL_UNIFORM_BUFFER, forthStride, sizeof(float), &fallOff);
    glBufferSubData(GL_UNIFORM_BUFFER, fifthStride, sizeof(glm::vec3), &ambientColor);
    glBufferSubData(GL_UNIFORM_BUFFER, sixStride, sizeof(glm::vec3), &diffuseColor);
    glBufferSubData(GL_UNIFORM_BUFFER, sevenStride, sizeof(glm::vec3), &specularColor);
    glBufferSubData(GL_UNIFORM_BUFFER, eightStride, sizeof(glm::vec3), &lightPosition);
    glBufferSubData(GL_UNIFORM_BUFFER, nineStride, sizeof(glm::vec3), &direction);

}

void VBOManager::UnUseUBO()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void VBOManager::UseTextures(bool useUV)
{
    if (useUV)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, UVTexture);
        GLint diffuseLocation = glGetUniformLocation(*programID, "diffuseTexture");
        glUniform1i(diffuseLocation, 2);
    }
    else
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);
        GLint diffuseLocation = glGetUniformLocation(*programID, "diffuseTexture");
        glUniform1i(diffuseLocation, 0);
    }

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture);
    GLint specularLocation = glGetUniformLocation(*programID, "specularTexture");
    glUniform1i(specularLocation, 1);
}

void VBOManager::UseSkyTextures()
{
    for (int i = 0; i < 6; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, skyTextures[i]);
        std::string stringName = "skybox[" + std::to_string(i) + "]";
        GLint textureLocation = glGetUniformLocation(*programID, stringName.c_str());
        glUniform1i(textureLocation, i);
    }
}

void VBOManager::DrawLine(LineTypeEnum type)
{
    if (type == LineTypeEnum::eORBIT)
    {
        glDrawArrays(GL_LINES, 0, (GLsizei)orbitVertex.size());
        return;
    }

    Mesh* mesh = FindMesh(type);
    glDrawArrays(GL_LINES, 0, mesh->getVertexNormalCount());
}

void VBOManager::DrawMesh(ObjectTypeEnum type, bool isTexture)
{
    Mesh* mesh = FindMesh(type);
    if (isTexture == true)
    {
        glDrawElements(GL_TRIANGLES, mesh->getVertexUVSize() * sizeof(glm::vec2), GL_UNSIGNED_INT, (void*)0);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, mesh->getIndexBufferSize(), GL_UNSIGNED_INT, (void*)0);
    }
}

void VBOManager::DrawFaceMesh(ObjectTypeEnum type)
{
    Mesh* mesh = FindMesh(type);
    glDrawArrays(GL_TRIANGLES, 0, mesh->getFaceBufferSize());
}

void VBOManager::DrawFaceNormalLine(ObjectTypeEnum type)
{
    Mesh* mesh = FindMesh(type);
    glDrawArrays(GL_LINES, 0, mesh->getFaceNormalCount());
}

void VBOManager::GenerateVBO()
{
    vaoManager.GenerateVAO();

    for (int i = 0; i < vertexBuffer.size(); ++i)
    {
        glGenBuffers(4, vertexBuffer.at(i));
    }

    for (int i = 0; i < lineBuffer.size(); ++i)
    {
        glGenBuffers(1, lineBuffer.at(i));
    }

    for (int i = 0; i < faceVertexBuffer.size(); ++i)
    {
        glGenBuffers(3, faceVertexBuffer.at(i));
    }

    for (int i = 0; i < faceNormalBuffer.size(); ++i)
    {
        glGenBuffers(1, faceNormalBuffer.at(i));
    }

    glGenBuffers(1, &uboGlobal);
    glGenBuffers(1, &uboLight);

    glGenTextures(1, &diffuseTexture);
    glGenTextures(1, &specularTexture);
    glGenTextures(1, &UVTexture);

    glGenTextures(6, skyTextures);
}

void VBOManager::SetVertexBuffer()
{
    vertexBuffer.push_back(vboSphere);
    vertexBuffer.push_back(vboSphereModified);
    vertexBuffer.push_back(vboSphere_4);
    vertexBuffer.push_back(vboCube);
    vertexBuffer.push_back(vboBunny);
    vertexBuffer.push_back(vboCreateSphere);
    vertexBuffer.push_back(vboQuad);

    lineBuffer.push_back(vboSphereLine);
    lineBuffer.push_back(vboSphereModifiedLine);
    lineBuffer.push_back(vboSphere_4Line);
    lineBuffer.push_back(vboCubeLine);
    lineBuffer.push_back(vboBunnyLine);
    lineBuffer.push_back(vboOrbitLine);
    lineBuffer.push_back(vboCreateSphereLine);
    lineBuffer.push_back(vboQuadLine);

    faceVertexBuffer.push_back(vboSphereFace);
    faceVertexBuffer.push_back(vboSphereModifiedFace);
    faceVertexBuffer.push_back(vboSphere_4Face);
    faceVertexBuffer.push_back(vboCubeFace);
    faceVertexBuffer.push_back(vboBunnyFace);
    faceVertexBuffer.push_back(vboCreateSphereFace);
    faceVertexBuffer.push_back(vboQuadFace);

    faceNormalBuffer.push_back(vboSphereFaceNormal);
    faceNormalBuffer.push_back(vboSphereModifiedFaceNormal);
    faceNormalBuffer.push_back(vboSphere_4FaceNormal);
    faceNormalBuffer.push_back(vboCubeFaceNormal);
    faceNormalBuffer.push_back(vboBunnyFaceNormal);
    faceNormalBuffer.push_back(vboCreateSphereFaceNormal);
    faceNormalBuffer.push_back(vboQuadNormal);
}

void VBOManager::SetSkyBoxFile()
{
    skyBoxesString.push_back("../Common/skybox/left.jpg");
    skyBoxesString.push_back("../Common/skybox/right.jpg");
    skyBoxesString.push_back("../Common/skybox/bottom.jpg");
    skyBoxesString.push_back("../Common/skybox/top.jpg");
    skyBoxesString.push_back("../Common/skybox/back.jpg");
    skyBoxesString.push_back("../Common/skybox/front.jpg");
}

void VBOManager::LoadOBJFile()
{
    objReader.ReadOBJFile("../Common/models/sphere.obj", &sphere);
    objReader.ReadOBJFile("../Common/models/sphere_modified.obj", &sphereModified);
    objReader.ReadOBJFile("../Common/models/4Sphere.obj", &sphere_4);
    objReader.ReadOBJFile("../Common/models/cube.obj", &cube, OBJReader::ReadMethod::LINE_BY_LINE, true);
    objReader.ReadOBJFile("../Common/models/bunny.obj", &bunny);
    objReader.ReadOBJFile("../Common/models/quad.obj", &quad);
    createSphere.CreateSphere(0.1f, 30);

    CalculateOrbit(10.f / PI);
}

void VBOManager::SetUpBuffers()
{
    GenerateVBO();
    int count = static_cast<int>(ObjectTypeEnum::COUNT);

    for (int i = 0; i < count; ++i)
    {
        SetArrayBuffers(vertexBuffer.at(i), i);
        SetIndexBuffers(vertexBuffer.at(i), i);

        SetFaceVertexBuffers(faceVertexBuffer.at(i), i);
        SetFaceNormalBuffers(faceNormalBuffer.at(i), i);
    }

    count = static_cast<int>(LineTypeEnum::COUNT);
    for (int i = 0; i < count; ++i)
    {
        SetLineBuffers(lineBuffer.at(i), i);
    }
    SetUniformBuffers(&uboGlobal, 0, 64);
    SetUniformBuffers(&uboLight, 1, 1604);

    SetTextureBuffers(&diffuseTexture, "../Common/models/metal_roof_diff_512x512.png", 0);
    SetTextureBuffers(&specularTexture, "../Common/models/metal_roof_spec_512x512.png", 1);
    SetTextureBuffers(&UVTexture, "../Common/models/grid.png", 2);
    SetSkyBoxTextureBuffers();
}

void VBOManager::SetUVBuffers(GLuint* buffer, Mesh* mesh)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer[texture]);
    glBufferData(GL_ARRAY_BUFFER, mesh->getVertexUVSize() * sizeof(glm::vec2), mesh->getVertexUVs(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(texture);
}

void VBOManager::SetUVBuffers(ObjectTypeEnum type, Mesh* mesh)
{
    int index = static_cast<int>(type);
    GLuint* buffer = vertexBuffer.at(index);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[texture]);
    glBufferData(GL_ARRAY_BUFFER, mesh->getVertexUVSize() * sizeof(glm::vec2), mesh->getVertexUVs(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(texture);
}

void VBOManager::SetArrayBuffers(GLuint* buffer, int type)
{
    ObjectTypeEnum objectType = static_cast<ObjectTypeEnum>(type);
    Mesh* mesh = FindMesh(objectType);

    vaoManager.UseVertexArrayID(objectType);
    //vbo for vertex
    glBindBuffer(GL_ARRAY_BUFFER, buffer[vertex]);
    glBufferData(GL_ARRAY_BUFFER, mesh->getVertexBufferSize() * sizeof(glm::vec3), mesh->getVertexBuffer(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertex);

    //vbo for normal vector
    glBindBuffer(GL_ARRAY_BUFFER, buffer[normal]);
    glBufferData(GL_ARRAY_BUFFER, mesh->getVertexBufferSize() * sizeof(glm::vec3), mesh->getVertexNormals(), GL_STATIC_DRAW);
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(normal);

    //vbo for texture
    SetUVBuffers(buffer, mesh);
}

void VBOManager::SetIndexBuffers(GLuint* buffer, int type)
{
    ObjectTypeEnum objectType = static_cast<ObjectTypeEnum>(type);
    Mesh* mesh = FindMesh(objectType);

    vaoManager.UseVertexArrayID(objectType);

    //vbo for index
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[indices]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBufferSize() * sizeof(GLint), mesh->getIndexBuffer(), GL_STATIC_DRAW);

}

void VBOManager::SetLineBuffers(GLuint* buffer, int type)
{
    LineTypeEnum objectType = static_cast<LineTypeEnum>(type);
    vaoManager.UseVertexArrayID(objectType);

    if (objectType == LineTypeEnum::eORBIT)
    {
        glBindBuffer(GL_ARRAY_BUFFER, *buffer);
        glBufferData(GL_ARRAY_BUFFER, orbitVertex.size() * sizeof(glm::vec3), orbitVertex.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(vertex);
        return;
    }
    Mesh* mesh = FindMesh(objectType);

    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, mesh->getVertexNormalCount() * sizeof(glm::vec3), mesh->getVertexNormalsForDisplay(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertex);
}

void VBOManager::SetFaceVertexBuffers(GLuint* buffer, int type)
{
    ObjectTypeEnum objectType = static_cast<ObjectTypeEnum>(type);
    Mesh* mesh = FindMesh(objectType);

    vaoManager.UseFaceVertexArrayID(objectType);
    //vbo for vertex
    glBindBuffer(GL_ARRAY_BUFFER, buffer[vertex]);
    glBufferData(GL_ARRAY_BUFFER, mesh->getFaceBufferSize() * sizeof(glm::vec3), mesh->getFaceBuffer(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertex);

    //vbo for normal vector
    glBindBuffer(GL_ARRAY_BUFFER, buffer[normal]);
    glBufferData(GL_ARRAY_BUFFER, mesh->getFaceBufferSize() * sizeof(glm::vec3), mesh->getFaceNormals(), GL_STATIC_DRAW);
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(normal);

    //vbo for texture
    SetUVBuffers(buffer, mesh);
}

void VBOManager::SetFaceNormalBuffers(GLuint* buffer, int type)
{
    ObjectTypeEnum objectType = static_cast<ObjectTypeEnum>(type);
    vaoManager.UseFaceNormalVertexArrayID(objectType);
    Mesh* mesh = FindMesh(objectType);

    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, mesh->getFaceNormalCount() * sizeof(glm::vec3), mesh->getFaceNormalsForDisplay(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vertex);
}

void VBOManager::SetUniformBuffers(GLuint* buffer, int index, int size)
{
    glBindBuffer(GL_UNIFORM_BUFFER, *buffer);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, index, *buffer);
}

void VBOManager::SetTextureBuffers(GLuint* buffer, const char* file, int type)
{
    if (type == 0)
    {
        glActiveTexture(GL_TEXTURE0);
    }
    else if (type == 1)
    {
        glActiveTexture(GL_TEXTURE1);
    }
    else if (type == 2)
    {
        glActiveTexture(GL_TEXTURE2);
    }

    glBindTexture(GL_TEXTURE_2D, *buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width;
    int height;
    int channel;

    unsigned char* data = stbi_load(file, &width, &height, &channel, 0);
    if (data)
    {
        if (channel == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else if (channel == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Can not load texture" << std::endl;
    }

    stbi_image_free(data);
}

void VBOManager::SetSkyBoxTextureBuffers()
{
    int size = static_cast<int>(skyBoxesString.size());
    int width, height, channel;
    unsigned char* data;

    stbi_set_flip_vertically_on_load(true);

    for (int i = 0; i < size; ++i)
    {
        glActiveTexture(GL_TEXTURE3 + i);
        glBindTexture(GL_TEXTURE_2D, skyTextures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        data = stbi_load(skyBoxesString.at(i).c_str(), &width, &height, &channel, 0);

        if (data)
        {
            if (channel == 3)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }
        else
        {
            std::cout << "Can not load texture" << skyBoxesString.at(i) << std::endl;
        }
        stbi_image_free(data);
    }
}

void VBOManager::CalculateOrbit(float distance)
{
    int countOfVertex = 49;
    float value = 2 * PI / countOfVertex;
    float degree = 0.f;
    float x = distance * cosf(degree);
    float z = distance * sinf(degree);
    orbitVertex.push_back(glm::vec3(x, 0.f, z));

    for (degree = value; degree < 2 * PI; degree += value)
    {
        float x = distance * cosf(degree);
        float z = distance * sinf(degree);
        orbitVertex.push_back(glm::vec3(x, 0.f, z));
        orbitVertex.push_back(glm::vec3(x, 0.f, z));
    }
}

void VBOManager::SetShaderProgramID(GLuint* id)
{
    programID = id;
}

void VBOManager::SetWindowSize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

void VBOManager::ResizeSkyTextures()
{
    int size = static_cast<int>(skyBoxesString.size());
    int width, height, channel;
    unsigned char* data;

    stbi_set_flip_vertically_on_load(true);

    for (int i = 0; i < size; ++i)
    {
        glActiveTexture(GL_TEXTURE3 + i);
        glBindTexture(GL_TEXTURE_2D, skyTextures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        data = stbi_load(skyBoxesString.at(i).c_str(), &width, &height, &channel, 0);

        if (data)
        {
            if (channel == 3)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }
        else
        {
            std::cout << "Can not load texture" << skyBoxesString.at(i) << std::endl;
        }
        stbi_image_free(data);
        /*int width, height, channel = 0;
        unsigned char* data;
        for (int i = 0; i < 6; ++i)
        {
            glActiveTexture(GL_TEXTURE3 + i);
            glBindTexture(GL_TEXTURE_2D, skyTextures[i]);

            data = stbi_load(skyBoxesString.at(i).c_str(), &width, &height, &channel, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }*/
    }
}

Mesh* VBOManager::FindMesh(ObjectTypeEnum type)
{
    Mesh* mesh = nullptr;

    switch (type)
    {
    case ObjectTypeEnum::eSPHERE:
        mesh = &sphere;
        break;
    case ObjectTypeEnum::eSPHERE_MODIFIED:
        mesh = &sphereModified;
        break;
    case ObjectTypeEnum::e4SPHERE:
        mesh = &sphere_4;
        break;
    case ObjectTypeEnum::eCUBE:
        mesh = &cube;
        break;
    case ObjectTypeEnum::eBUNNY:
        mesh = &bunny;
        break;
    case ObjectTypeEnum::eCREATE_SPHERE:
        mesh = &createSphere;
        break;
    case ObjectTypeEnum::eQUAD:
        mesh = &quad;
        break;
    default:
        break;
    }

    return mesh;
}

Mesh* VBOManager::FindMesh(LineTypeEnum type)
{
    Mesh* mesh = nullptr;

    switch (type)
    {
    case LineTypeEnum::eSPHERE:
        mesh = &sphere;
        break;
    case LineTypeEnum::eSPHERE_MODIFIED:
        mesh = &sphereModified;
        break;
    case LineTypeEnum::e4SPHERE:
        mesh = &sphere_4;
        break;
    case LineTypeEnum::eCUBE:
        mesh = &cube;
        break;
    case LineTypeEnum::eBUNNY:
        mesh = &bunny;
        break;
    case LineTypeEnum::eCREATE_SPHERE:
        mesh = &createSphere;
        break;
    case LineTypeEnum::eQUAD:
        mesh = &quad;
        break;
    default:
        break;
    }

    return mesh;
}

