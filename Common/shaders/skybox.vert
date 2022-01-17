/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: skybox.vert
Purpose: This file is vertex shader with skybox.
Language: glsl, Visual Studio Code
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 11-21-2021
End Header --------------------------------------------------------*/
#version 450 core

in layout (location = 0) vec3 position;

out vec3 fragPosition;

uniform mat4 transform;
uniform mat4 cameraMatrix;
uniform mat4 ndcMatrix;

void main()
{
    fragPosition = vec3(transform* vec4(position, 1.f));
    
    mat4 newCamera= mat4(mat3(inverse(cameraMatrix)));
    vec4 pos = ndcMatrix * newCamera * vec4(position, 1.f);

    gl_Position = pos.xyww;
}