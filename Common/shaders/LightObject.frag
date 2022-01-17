/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: LightObject.frag
Purpose: This file is fragment shader for light object. It has only diffuse color.
Language: glsl, Visual Studio Code
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-20-2021
End Header --------------------------------------------------------*/
#version 450 core

out vec4 fragColor;

uniform vec3 color;

void main()
{
    fragColor = vec4(color, 1.0f);
}