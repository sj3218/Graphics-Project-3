/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.cpp
Purpose: This file is for giving information about mesh. There are buffers of vertex and face.
And there is function of creating sphere.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/

//#include <gl/GL.h>
#include <GL/glew.h>
#include <iostream>
#include <set>
#include "Mesh.h"


#include <glm/gtc/epsilon.hpp>

// Initialize the data members in the mesh
void Mesh::initData()
{
    vertexBuffer.clear();
    vertexIndices.clear();
    vertexUVs.clear();
    vertexNormals.clear();
    vertexNormalDisplay.clear();

    normalLength = 0.00f;

    return;
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
GLfloat* Mesh::getVertexBuffer()
{
    return reinterpret_cast<GLfloat*>(vertexBuffer.data());
}

GLfloat* Mesh::getVertexNormals()
{
    return reinterpret_cast<GLfloat*>(vertexNormals.data());
}

GLfloat* Mesh::getVertexUVs()
{
    return reinterpret_cast<GLfloat*>(vertexUVs.data());
}

GLfloat* Mesh::getVertexNormalsForDisplay()
{
    return reinterpret_cast<GLfloat*>(vertexNormalDisplay.data());
}

GLfloat* Mesh::getFaceBuffer()
{
    return reinterpret_cast<GLfloat*>(faceBuffer.data());
}

GLfloat* Mesh::getFaceNormals()
{
    return reinterpret_cast<GLfloat*>(faceNormals.data());
}

GLfloat* Mesh::getFaceNormalsForDisplay()
{
    return reinterpret_cast<GLfloat*>(faceNormalDisplay.data());
}

GLuint* Mesh::getIndexBuffer()
{
    return vertexIndices.data();
}

////////////////////////////////////
////////////////////////////////////
////////////////////////////////////
unsigned int Mesh::getVertexBufferSize()
{
    return (unsigned int)vertexBuffer.size();
}

unsigned int Mesh::getIndexBufferSize()
{
    return (unsigned int)vertexIndices.size();
}

unsigned int Mesh::getTriangleCount()
{
    return getIndexBufferSize() / 3;
}

unsigned int Mesh::getVertexCount()
{
    return getVertexBufferSize();
}

unsigned int Mesh::getVertexNormalCount()
{
    return (unsigned int)vertexNormalDisplay.size();
}

unsigned int Mesh::getFaceBufferSize()
{
    return (unsigned int)faceBuffer.size();
}

unsigned int Mesh::getFaceCount()
{
    return getFaceNormalCount();
}

unsigned int Mesh::getFaceNormalCount()
{
    return (unsigned int)faceNormalDisplay.size();
}

unsigned int Mesh::getVertexUVSize()
{
    return (unsigned int)vertexUVs.size();
}

glm::vec3  Mesh::getModelScale()
{
    glm::vec3 scale = boundingBox[1] - boundingBox[0];

    if (scale.x == 0.0)
        scale.x = 1.0;

    if (scale.y == 0.0)
        scale.y = 1.0;

    if (scale.z == 0.0)
        scale.z = 1.0;

    return scale;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

glm::vec3  Mesh::getModelCentroid()
{
    return glm::vec3(boundingBox[0] + boundingBox[1]) * 0.5f;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

glm::vec3 Mesh::getCentroidVector(glm::vec3 vVertex)
{
    return glm::normalize(vVertex - getModelCentroid());
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

struct compareVec
{
    bool operator() (const glm::vec3& lhs, const glm::vec3& rhs) const
    {
        float V = glm::dot(lhs, rhs);
        //bool bRetCode = glm::epsilonEqual( V, 1.0f, FLT_EPSILON);
        bool bRetCode = glm::epsilonNotEqual(V, 1.0f, FLT_EPSILON);

        return bRetCode;
    }
};

/////////////////////////////////////////////////////////
int Mesh::calcVertexNormals(GLboolean bFlipNormals)
{
    int rFlag = -1;

    // vertices and indices must be populated
    if (vertexBuffer.empty() || vertexIndices.empty())
    {
        std::cout << "Cannot calculate vertex normals for empty mesh." << std::endl;
        return rFlag;
    }

    // Pre-built vertex normals
//    if( !vertexNormals.empty() )
//    {
//        std::cout << "Vertex normals specified by the file. Skipping this step." << std::endl;
//        return rFlag;
//    }

    // Initialize vertex normals
    unsigned int  numVertices = getVertexCount();
    vertexNormals.resize(numVertices, glm::vec3(0.0f));
    int resizeValue = numVertices * 2;
    vertexNormalDisplay.resize(resizeValue, glm::vec3(0.0f));

    //std::vector< std::set< glm::vec3, compareVec > >  vNormalSet;
    std::vector< std::vector<glm::vec3> >  vNormalSet;
    vNormalSet.resize(numVertices);

    // For every face
    glm::uint index = 0;
    for (; index < vertexIndices.size(); )
    {
        GLuint a = vertexIndices.at(index++);
        GLuint b = vertexIndices.at(index++);
        GLuint c = vertexIndices.at(index++);

        glm::vec3  vA = vertexBuffer[a];
        glm::vec3  vB = vertexBuffer[b];
        glm::vec3  vC = vertexBuffer[c];

        // Edge vectors
        glm::vec3  E1 = vB - vA;
        glm::vec3  E2 = vC - vA;

        glm::vec3  N = glm::normalize(glm::cross(E1, E2));

        if (bFlipNormals)
            N = N * -1.0f;

        // For vertex a
        vNormalSet.at(a).push_back(N);
        vNormalSet.at(b).push_back(N);
        vNormalSet.at(c).push_back(N);
        /*vNormalSet.at( a ).insert( N );
        vNormalSet.at( b ).insert( N );
        vNormalSet.at( c ).insert( N );*/
    }

    // Now sum up the values per vertex
    for (int index = 0; index < vNormalSet.size(); ++index)
    {
        glm::vec3  vNormal(0.0f);

        //        if( vNormalSet[index].size() <= 0 )
        //            std::cout << "[Normal Set](" << index << ") Size = " << vNormalSet[index].size() << std::endl;

        setNormalLength(0.08f);

        auto nIt = vNormalSet[index].begin();
        while (nIt != vNormalSet[index].end())
        {
            vNormal += (*nIt);
            ++nIt;
        }

        // save vertex normal
        vertexNormals[index] = glm::normalize(vNormal);

        // save normal to display
        glm::vec3  vA = vertexBuffer[index];

        int indexForVertexNormalDisplay = 2 * index;
        int indexForVertexNormalDisplay2 = indexForVertexNormalDisplay+1;

        vertexNormalDisplay[indexForVertexNormalDisplay] = vA;
        vertexNormalDisplay[indexForVertexNormalDisplay2] = vA + (normalLength * vertexNormals[index]);

    }

    // success
    rFlag = 0;

    return rFlag;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void Mesh::calcVertexNormalsForDisplay(GLboolean bFlipNormals)
{
    unsigned int numVertices = getVertexCount();
    int resize = numVertices * 2;
    vertexNormalDisplay.resize(resize, glm::vec3(0.0f));

    for (int iNormal = 0; iNormal < vertexNormals.size(); ++iNormal)
    {
        glm::vec3 normal = vertexNormals[iNormal] * normalLength;

        int indexOfVertexNormalDisplay = 2 * iNormal;
        int indexOfVertexNormalDisplay2 = indexOfVertexNormalDisplay + 1;
        vertexNormalDisplay[indexOfVertexNormalDisplay] = vertexBuffer[iNormal];
        vertexNormalDisplay[indexOfVertexNormalDisplay2] = vertexBuffer[iNormal] + normal;
    }
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

GLfloat& Mesh::getNormalLength()
{
    return normalLength;
}

void Mesh::setNormalLength(GLfloat nLength)
{
    normalLength = nLength;
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

int Mesh::calcUVs(Mesh::UVType uvType)
{
    int rFlag = -1;

    // clear any existing UV
    vertexUVs.clear();

    glm::vec3 delta = getModelScale();

    for (int nVertex = 0; nVertex < vertexBuffer.size(); ++nVertex)
    {
        glm::vec3 V = vertexBuffer[nVertex];
        glm::vec2 uv(0.0f);

        glm::vec3 normVertex = glm::vec3((V.x - boundingBox[0].x) / delta.x,
            (V.y - boundingBox[0].y) / delta.y,
            (V.z - boundingBox[0].z) / delta.z);

        //        normVertex = (2.0f * normVertex) - glm::vec3(1.0f);

        glm::vec3 centroidVec = getCentroidVector(V);

        float theta(0.0f);
        float z(0.0f);
        float phi(0.0f);

        switch (uvType)
        {
        case Mesh::UVType::PLANAR_UV:
            uv.x = (normVertex.x - (-1.0f)) / (2.0f);
            uv.y = (normVertex.y - (-1.0f)) / (2.0f);
            break;

        case Mesh::UVType::CYLINDRICAL_UV:
            theta = glm::degrees(static_cast<float>(glm::atan(centroidVec.z, centroidVec.x)));
            theta += 180.0f;

            z = (centroidVec.y + 1.0f) * 0.5f;

            uv.x = theta / 360.0f;
            uv.y = z;
            break;

        case Mesh::UVType::SPHERICAL_UV:
            theta = glm::degrees(static_cast<float>(glm::atan(centroidVec.z, centroidVec.x)));
            theta += 180.0f;

            z = centroidVec.y;
            phi = glm::degrees(glm::acos(z / centroidVec.length()));

            uv.x = theta / 360.0f;
            uv.y = 1.0f - (phi / 180.0f);
            
            break;

        case Mesh::UVType::CUBE_MAPPED_UV:
            uv = calcCubeMap(centroidVec);
            break;
        }

        vertexUVs.push_back(uv);
    }

    return rFlag;
}

int Mesh::calcUVs(bool entities, Mesh::UVType uvType)
{
    int rFlag = -1;

    // clear any existing UV
    vertexUVs.clear();

    glm::vec3 delta = getModelScale();

    for (int nVertex = 0; nVertex < vertexBuffer.size(); ++nVertex)
    {
        glm::vec3 V = vertexBuffer[nVertex];
        glm::vec2 uv(0.0f);

        glm::vec3 normVertex = glm::vec3((V.x - boundingBox[0].x) / delta.x,
            (V.y - boundingBox[0].y) / delta.y,
            (V.z - boundingBox[0].z) / delta.z);

        //        normVertex = (2.0f * normVertex) - glm::vec3(1.0f);

        glm::vec3 centroidVec = getCentroidVector(V);

        if (entities)
        {
            centroidVec = glm::normalize(vertexNormals.at(nVertex));
        }

        float theta(0.0f);
        float z(0.0f);
        float phi(0.0f);

        switch (uvType)
        {
        case Mesh::UVType::PLANAR_UV:
            uv.x = (normVertex.x - (-1.0f)) / (2.0f);
            uv.y = (normVertex.y - (-1.0f)) / (2.0f);
            break;

        case Mesh::UVType::CYLINDRICAL_UV:
            theta = glm::degrees(static_cast<float>(glm::atan(centroidVec.z, centroidVec.x)));
            theta += 180.0f;

            z = (centroidVec.y + 1.0f) * 0.5f;

            uv.x = theta / 360.0f;
            uv.y = z;
            break;

        case Mesh::UVType::SPHERICAL_UV:
            theta = glm::degrees(static_cast<float>(glm::atan(centroidVec.z, centroidVec.x)));
            theta += 180.0f;

            z = centroidVec.y;
            phi = glm::degrees(glm::acos(z / centroidVec.length()));

            uv.x = theta / 360.0f;
            uv.y = 1.0f - (phi / 180.0f);

            break;

        case Mesh::UVType::CUBE_MAPPED_UV:
            uv = calcCubeMap(centroidVec);
            break;
        }

        vertexUVs.push_back(uv);
    }

    return rFlag;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

glm::vec2 Mesh::calcCubeMap(glm::vec3 vEntity)
{
    float x = vEntity.x;
    float y = vEntity.y;
    float z = vEntity.z;

    float absX = abs(x);
    float absY = abs(y);
    float absZ = abs(z);

    int isXPositive = x > 0 ? 1 : 0;
    int isYPositive = y > 0 ? 1 : 0;
    int isZPositive = z > 0 ? 1 : 0;

    float maxAxis, uc, vc;
    glm::vec2 uv = glm::vec2(0.0);

    // POSITIVE X
    if (bool(isXPositive) && (absX >= absY) && (absX >= absZ))
    {
        // u (0 to 1) goes from +z to -z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = -z;
        vc = y;
    }

    // NEGATIVE X
    else if (!bool(isXPositive) && absX >= absY && absX >= absZ)
    {
        // u (0 to 1) goes from -z to +z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = z;
        vc = y;
    }

    // POSITIVE Y
    else if (bool(isYPositive) && absY >= absX && absY >= absZ)
    {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from +z to -z
        maxAxis = absY;
        uc = x;
        vc = -z;
    }

    // NEGATIVE Y
    else if (!bool(isYPositive) && absY >= absX && absY >= absZ)
    {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -z to +z
        maxAxis = absY;
        uc = x;
        vc = z;
    }

    // POSITIVE Z
    else if (bool(isZPositive) && absZ >= absX && absZ >= absY)
    {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = x;
        vc = y;
    }

    // NEGATIVE Z
    else if (!bool(isZPositive) && absZ >= absX && absZ >= absY)
    {
        // u (0 to 1) goes from +x to -x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = -x;
        vc = y;
    }

    // Convert range from -1 to 1 to 0 to 1
    uv.s = 0.5f * (uc / maxAxis + 1.0f);
    uv.t = 0.5f * (vc / maxAxis + 1.0f);

    return uv;
}

void Mesh::SetFaceBuffer(GLboolean bFlipNormals)
{
    for (int i = 0; i < vertexIndices.size(); ++i)
    {
        GLuint index_1 = vertexIndices.at(i);
        GLuint index_2 = vertexIndices.at(++i);
        GLuint index_3 = vertexIndices.at(++i);

        glm::vec3 P = vertexBuffer.at(index_1);
        glm::vec3 Q = vertexBuffer.at(index_2);
        glm::vec3 R = vertexBuffer.at(index_3);

        glm::vec3 QP = Q - P;
        glm::vec3 RP = R - P;
        glm::vec3 normalVector = glm::normalize(glm::cross(QP, RP));

        faceBuffer.push_back(P);
        faceBuffer.push_back(Q);
        faceBuffer.push_back(R);

        faceNormals.push_back(normalVector);
        faceNormals.push_back(normalVector);
        faceNormals.push_back(normalVector);
    }
    calcFaceVertexNormalsForDisplay(bFlipNormals);
}

void Mesh::calcFaceVertexNormalsForDisplay(GLboolean bFlipNormals)
{
    int numFaces = int(getFaceBufferSize()) / 3;
    int resize = numFaces * 2;
    faceNormalDisplay.resize(resize, glm::vec3(0.f));
    for (int iFace = 0; iFace < numFaces; ++iFace)
    {
        int index = iFace * 3;
        glm::vec3 normal = faceNormals[index] * normalLength;
        
        int index2 = index + 1;
        int index3 = index + 2;
        glm::vec3 centerOfTriangle = faceBuffer[index] + faceBuffer[index2] + faceBuffer[index3];
        centerOfTriangle /= glm::vec3(3.f);

        if (bFlipNormals)
        {
            normal = -normal;
        }

        int indexOfFaceNormalDisplay = 2 * iFace;
        int indexOfFaceNormalDisplay2 = indexOfFaceNormalDisplay + 1;
        faceNormalDisplay[indexOfFaceNormalDisplay] = centerOfTriangle;
        faceNormalDisplay[indexOfFaceNormalDisplay2] = centerOfTriangle + normal;
    }
}

void Mesh::CreateSphere(float radius, int numDivisions)
{
    glm::vec3 point;

    float stepTheta = 2 * PI / numDivisions;
    float stepPI = PI / numDivisions;
    float thetaAngle, pIAngle;

    for (int i = 0; i <= numDivisions; ++i)
    {
        pIAngle = PI / 2 - i * stepPI;

        for (int j = 0; j <= numDivisions; ++j)
        {
            thetaAngle = j * stepTheta;

            point.x = radius * cosf(pIAngle) * cosf(thetaAngle);
            point.y = radius * cosf(pIAngle) * sinf(thetaAngle);
            point.z = radius * sinf(pIAngle);
            vertexBuffer.push_back(point);
        }
    }

    unsigned int index1, index2;

    for (int i = 0; i <= numDivisions; ++i)
    {
        index1 = i * (numDivisions);
        index2 = index1 + numDivisions;

        for (int j = 0; j <= numDivisions; ++j)
        {
            if (i != 0)
            {
                vertexIndices.push_back(index1);
                vertexIndices.push_back(index2);
                vertexIndices.push_back(index1 +1);
            }

            if (i != (numDivisions))
            {
                vertexIndices.push_back(index1 +1);
                vertexIndices.push_back(index2);
                vertexIndices.push_back(index2 + 1);
            }
            ++index1;
            ++index2;
        }
    }
    this->calcVertexNormals();
    this->calcVertexNormalsForDisplay();
    this->SetFaceBuffer();
}
