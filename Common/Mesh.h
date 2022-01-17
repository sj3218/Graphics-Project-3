/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.h
Purpose: This file is for giving information about mesh.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#ifndef SIMPLE_OBJ_SCENE_MESH_H
#define SIMPLE_OBJ_SCENE_MESH_H

#include <vector>
#include <glm/glm.hpp>
#define PI 4.0f * atan(1.0f)
#define TWOPI 2.f*PI

class Mesh
{
public:
    friend class OBJReader;

    // Get attribute values buffer
    GLfloat *getVertexBuffer();             // attribute 0
    GLfloat *getVertexNormals();            // attribute 1
    GLfloat *getVertexUVs();                // attribute 2

    GLfloat *getVertexNormalsForDisplay();  // attribute 0

    GLfloat* getFaceBuffer();
    GLfloat* getFaceNormals();
    GLfloat* getFaceNormalsForDisplay();

    unsigned int getVertexBufferSize();
    unsigned int getVertexCount();
    unsigned int getVertexNormalCount();

    unsigned int getFaceBufferSize();
    unsigned int getFaceCount();
    unsigned int getFaceNormalCount();

    unsigned int getVertexUVSize();

    // Get vertex index buffer
    GLuint *getIndexBuffer();
    unsigned int getIndexBufferSize();
    unsigned int getTriangleCount();

    glm::vec3   getModelScale();
    glm::vec3   getModelCentroid();
    glm::vec3   getCentroidVector( glm::vec3 vVertex );


    GLfloat  &  getNormalLength();
    void setNormalLength( GLfloat nLength );

    // initialize the data members
    void initData();

    // calculate vertex normals
    int calcVertexNormals(GLboolean bFlipNormals = false);

    // calculate the "display" normals
    void calcVertexNormalsForDisplay(GLboolean bFlipNormals = false);

    // calculate texture coordinates
    enum class UVType { PLANAR_UV = 0,
                  CYLINDRICAL_UV,
                  SPHERICAL_UV,
                  CUBE_MAPPED_UV};

    int         calcUVs( Mesh::UVType uvType = Mesh::UVType::PLANAR_UV );
    int         calcUVs( bool entities, Mesh::UVType uvType = Mesh::UVType::PLANAR_UV );
    glm::vec2   calcCubeMap( glm::vec3 vEntity );

    void SetFaceBuffer(GLboolean bFlipNormals = false);
    void calcFaceVertexNormalsForDisplay(GLboolean bFlipNormals = false);

    void CreateSphere(float radius, int numDivisions);

private:
    std::vector<glm::vec3>    vertexBuffer;
    std::vector<GLuint>       vertexIndices;
    std::vector<glm::vec2>    vertexUVs;
    std::vector<glm::vec3>    vertexNormals, vertexNormalDisplay;

    std::vector<glm::vec3> faceBuffer;
    std::vector<glm::vec3> faceNormals, faceNormalDisplay;

    glm::vec3               boundingBox[2];
    GLfloat                 normalLength = 0.1f;

};


#endif //SIMPLE_OBJ_SCENE_MESH_H
