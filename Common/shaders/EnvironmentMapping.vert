/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: EnvironmentMapping.vert
Purpose: This file is vertex shader with environment mapping for center object.
Language: glsl, Visual Studio Code
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 11-21-2021
End Header --------------------------------------------------------*/
#version 450 core

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec2 textureCoord;

uniform mat4 transform;
uniform mat4 cameraMatrix;
uniform mat4 ndcMatrix;

uniform float chromatic;
uniform vec3 viewPosition;
uniform float fresnelPower;
uniform float refractiveIndex;

out vec3 fragPosition;
out vec3 outNormal;
out vec2 texCoord;

out vec3 cameraPosition;

out vec3 reflectVector;
out vec3 refractVector;
out float ratio;

out vec3 RefractVectorR;
out vec3 RefractVectorB;

float etr = 1-chromatic;
float etb = 1-chromatic;

const float camera_refractive_index = 1.f;

vec3 GetReflectionVector(vec3 n, vec3 view)
{
    return 2.f * dot(n, view) * n - view;
}

vec3 GetRefractionVector(vec3 n, vec3 view, float Eta)
{
    float dot_normal_view = max(dot(n, view), 0.0);
    
    float k = 1.f - (Eta*Eta) * (1.f - (dot_normal_view * dot_normal_view));
    
    k = max(k, 0.f);

    return (Eta * dot_normal_view - sqrt(k)) * n - Eta * view;
}

void main()
{
    fragPosition = vec3(transform * vec4(position, 1.0));
    outNormal = mat3(transpose(inverse(transform))) * normal;
    texCoord = textureCoord;
    
    vec3 viewVector = normalize(viewPosition - fragPosition);

    vec3 n = normalize(outNormal);
    
    float eta = camera_refractive_index / refractiveIndex;

    float f = ((1.f - eta) * (1.f - eta)) / ((1.f + eta) * (1.f + eta));

    ratio = f + (1.f - f) * pow((1.f- dot(viewVector, n)), fresnelPower);
    cameraPosition = viewPosition;
    reflectVector = GetReflectionVector(n, viewVector);

    if(chromatic == 0)
    {
        RefractVectorR = GetRefractionVector(n, viewVector, eta);
        refractVector = GetRefractionVector(n, viewVector, eta);
        RefractVectorB = GetRefractionVector(n, viewVector, eta);
    }
    else
    {
        RefractVectorR = GetRefractionVector(n, viewVector, etr);
        refractVector = GetRefractionVector(n, viewVector, eta);
        RefractVectorB = GetRefractionVector(n, viewVector, etb);
    }
    
    gl_Position = ndcMatrix* inverse(cameraMatrix) * transform * vec4(position, 1.0);
}