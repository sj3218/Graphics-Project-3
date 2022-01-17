/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: FragmentShader.frag
Purpose: This file is fragment shader with lighting.
Language: glsl, Visual Studio Code
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#version 410 core

in vec3 outNormal;
in vec3 fragPosition;

out vec4 fragColor;

uniform vec3 color;
uniform vec3 lightPosition;

void main()
{
    vec3 lightColor = vec3(1.0,1.0,1.0);
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength*lightColor;

    vec3 normal = normalize(outNormal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diffuse_value = max(dot(normal, lightDir),0.f);
    vec3 diffuse = diffuse_value * lightColor;

    vec3 resultColor = (ambient+diffuse)*color;

    fragColor = vec4(resultColor, 1.0);
}