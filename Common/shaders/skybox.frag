/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: skybox.frag
Purpose: This file is fragment shader with skybox.
Language: glsl, Visual Studio Code
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 11-21-2021
End Header --------------------------------------------------------*/
#version 450 core

out vec4 fragColor;

in vec3 fragPosition;

uniform sampler2D skybox[6]; //left, right, bottom, top, back, front

void main()
{
    float x = fragPosition.x;
    float y = fragPosition.y;
    float z = fragPosition.z;

    float absX = abs(x);
    float absY = abs(y);
    float absZ = abs(z);

    int isXPositive = x > 0 ? 1 : 0;
    int isYPositive = y > 0 ? 1 : 0;
    int isZPositive = z > 0 ? 1 : 0;

    float maxAxis =  0.f;
    vec2 uv = vec2(0.f);

    vec3 resultTexture = vec3(0.f);

    if((absX >= absY) && (absX >= absZ))
    {
        if(isXPositive == 1) // positive x // right
        {
            maxAxis = absX;

            uv.x = 0.5f * (-z / maxAxis + 1.f);
            uv.y = 0.5f * (y / maxAxis + 1.f);

            resultTexture = texture(skybox[1], uv).rgb;
        }
        else if(isXPositive == 0) // negative x // left
        {
            maxAxis = absX;

            uv.x = 0.5f * (z / maxAxis + 1.f);
            uv.y = 0.5f * (y / maxAxis + 1.f);

            resultTexture = texture(skybox[0], uv).rgb;
        }
    }
    else if((absY >= absX) && (absY >= absZ))
    {
        if(isYPositive == 1) //positive y // top
        {
            maxAxis = absY;
            
            uv.x = 0.5f * (x / maxAxis + 1.f);
            uv.y = 0.5f * (-z / maxAxis + 1.f);

            resultTexture = texture(skybox[3], uv).rgb;
        }
        else if(isYPositive == 0) // negative y // bottom
        {
            maxAxis = absY;
            
            uv.x = 0.5f * (x / maxAxis + 1.f);
            uv.y = 0.5f * (z / maxAxis + 1.f);

            resultTexture = texture(skybox[2], uv).rgb;
        }
    }
    else if((absZ >= absX) && (absZ >= absY))
    {
        if(isZPositive == 1) // positive z // front
        {
            maxAxis = absZ;
            
            uv.x = 0.5f * (x / maxAxis + 1.f);
            uv.y = 0.5f * (y / maxAxis + 1.f);

            resultTexture = texture(skybox[5], uv).rgb;
        }
        else if(isZPositive == 0) //negative z // back
        {
            maxAxis = absZ;
            
            uv.x = 0.5f * (-x / maxAxis + 1.f);
            uv.y = 0.5f * (y / maxAxis + 1.f);

            resultTexture = texture(skybox[4], uv).rgb;
        }
    }
    fragColor = vec4(resultTexture, 1.f);
    
}