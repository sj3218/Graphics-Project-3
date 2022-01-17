/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VertexShader.vert
Purpose: This file is vertex shader with lighting.
Language: glsl, Visual Studio Code
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#version 410 core

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;

uniform mat4 transform;
uniform mat4 cameraMatrix;
uniform mat4 ndcMatrix;

//out vec3 color;
out vec3 outNormal;
out vec3 fragPosition;

void main()
{
    fragPosition = vec3(transform * vec4(position,1.0));
    outNormal = mat3(transpose(inverse(transform)))*normal;
    gl_Position = ndcMatrix * inverse(cameraMatrix) * transform * vec4(position,1.0);
}