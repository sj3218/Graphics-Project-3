/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: EnvironmentMapping.frag
Purpose: This file is fragment shader with environment mapping for center object.
Language: glsl, Visual Studio Code
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 11-21-2021
End Header --------------------------------------------------------*/
#version 450 core

in vec3 fragPosition;
in vec3 outNormal;
in vec2 texCoord;

struct lightInformation
{
    int lightType; // dir, point, spot
    float innerAngle;
    float outerAngle;
    float fallOff;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 lightPosition;
    vec3 direction;
};

layout (std140, binding =0) uniform global
{
    int numbLights;
    float far;
    float near;
    vec3 attenuation;
    vec3 fogColor;
    vec3 globalAmbientColor;
};

layout (std140, binding =1) uniform light
{
    lightInformation lights[16];
};

uniform float ambientStrength;
uniform vec3 ambientMaterial;
uniform vec3 emissive;
uniform vec3 color;

in vec3 cameraPosition;
in vec3 reflectVector;
in vec3 refractVector;
in float ratio;

in vec3 RefractVectorR;
in vec3 RefractVectorB;

out vec4 fragColor;

uniform sampler2D skyTexture[6];
uniform float blending;
uniform int environmentType; //0 = reflection, 1 = refraction, 2 = combination

vec3 CalculateDirectionalLight(int index, vec3 normal, vec3 view, vec2 tex)
{
    int Ns = 64;
    vec3 lightDir = normalize(-lights[index].direction);
    vec3 reflectDir = 2*dot(normal, lightDir)*normal - lightDir;

    float diffuseValue = max(dot(normal, lightDir),0.f);
    float specularValue = pow(max(dot(view, reflectDir), 0.f), Ns);

    vec3 ambient = lights[index].ambientColor * ambientMaterial * ambientStrength;
    vec3 diffuse = lights[index].diffuseColor * diffuseValue;
    vec3 specular = lights[index].specularColor * specularValue;

    return ambient + diffuse + specular;
}

vec3 CalculatePointLight(int index, vec3 normal, vec3 view, vec3 pos, vec2 tex)
{
    float Ns = 64;

    vec3 lightDir = lights[index].lightPosition - pos;
    float lightDirLength = length(lightDir);
    
    float attenuationValue = min(1/(attenuation.x + attenuation.y * lightDirLength + attenuation.z * lightDirLength*lightDirLength), 1.f);

    lightDir = normalize(lightDir);

    vec3 reflectDir = 2*dot(normal, lightDir)*normal - lightDir;

    float diffuseValue = max(dot(normal, lightDir), 0.f);
    float specularValue = pow(max(dot(view, reflectDir), 0.f), Ns);

    vec3 ambient = lights[index].ambientColor * ambientMaterial * ambientStrength;
    vec3 diffuse = lights[index].diffuseColor * diffuseValue;
    vec3 specular = lights[index].specularColor * specularValue;

    return (ambient + diffuse + specular) * attenuationValue;
}

vec3 CalculateSpotLight(int index, vec3 normal, vec3 view, vec3 pos, vec2 tex)
{
    int Ns = 64;
    vec3 lightDir = lights[index].lightPosition- pos;
    float lightDirLength = length(lightDir);

    float attenuationValue = min(1/(attenuation.x + attenuation.y * lightDirLength + attenuation.z * lightDirLength*lightDirLength), 1.f);

    lightDir = normalize(lightDir);

    vec3 reflectDir = 2*dot(normal, lightDir)*normal - lightDir;

    float diffuseValue = max(dot(normal, lightDir), 0.f);
    float specularValue = pow(max(dot(view, reflectDir), 0.f), Ns);

    float cosAlpha = dot(lightDir, normalize(-lights[index].direction));

    float spotLightEffect =0;
    if(cosAlpha < cos(lights[index].outerAngle))
    {
        spotLightEffect = 0.f;
    }
    else if (cosAlpha > cos(lights[index].innerAngle))
    {
        spotLightEffect = 1.f;
    }
    else
    {
        spotLightEffect = pow((cosAlpha - cos(lights[index].outerAngle))/(cos(lights[index].innerAngle)-cos(lights[index].outerAngle)), lights[index].fallOff);
    }

    vec3 ambient = lights[index].ambientColor * ambientMaterial * ambientStrength;
    vec3 diffuse = lights[index].diffuseColor * diffuseValue;
    vec3 specular = lights[index].specularColor * specularValue;

    return attenuationValue*(ambient + spotLightEffect*(diffuse + specular));
}

vec3 CubeMapping(vec3 pos)
{
    float x = pos.x;
    float y = pos.y;
    float z = pos.z;

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

            uv.x = 0.5f * (z / maxAxis + 1.f);
            uv.y = 0.5f * (y / maxAxis + 1.f);

            resultTexture = texture(skyTexture[1], uv).rgb;
        }
        else if(isXPositive == 0) // negative x // left
        {
            maxAxis = absX;

            uv.x = 0.5f * (-z / maxAxis + 1.f);
            uv.y = 0.5f * (y / maxAxis + 1.f);

            resultTexture = texture(skyTexture[0], uv).rgb;
        }
    }
    else if((absY >= absX) && (absY >= absZ))
    {
        if(isYPositive == 1) //positive y // top
        {
            maxAxis = absY;
            
            uv.x = 0.5f * (-z / maxAxis + 1.f);
            uv.y = 0.5f * (x / maxAxis + 1.f);

            resultTexture = texture(skyTexture[3], uv).rgb;
        }
        else if(isYPositive == 0) // negative y // bottom
        {
            maxAxis = absY;
            
            uv.x = 0.5f * (z / maxAxis + 1.f);
            uv.y = 0.5f * (x / maxAxis + 1.f);

            resultTexture = texture(skyTexture[2], uv).rgb;
        }
    }
    else if((absZ >= absX) && (absZ >= absY))
    {
        if(isZPositive == 1) // positive z // front
        {
            maxAxis = absZ;
            
            uv.x = 0.5f * (-x / maxAxis + 1.f);
            uv.y = 0.5f * (y / maxAxis + 1.f);

            resultTexture = texture(skyTexture[5], uv).rgb;
        }
        else if(isZPositive == 0) //negative z // back
        {
            maxAxis = absZ;
            
            uv.x = 0.5f * (x / maxAxis + 1.f);
            uv.y = 0.5f * (y / maxAxis + 1.f);

            resultTexture = texture(skyTexture[4], uv).rgb;
        }
    }
    return resultTexture;
}

void main()
{
    vec3 resultTexture = vec3(0.f);
    if(environmentType == 0) // reflection
    {
        resultTexture = CubeMapping(reflectVector);

    }
    else if(environmentType == 1) // refraction
    {
        resultTexture = vec3(CubeMapping(refractVector));
    }
    else if(environmentType == 2) //combination
    {
        vec3 reflectColor = CubeMapping(reflectVector);
        vec3 refractColor;

        refractColor.r = vec3(CubeMapping(RefractVectorR)).r;
        refractColor.g = vec3(CubeMapping(refractVector)).g;
        refractColor.b = vec3(CubeMapping(RefractVectorB)).b;

        resultTexture = mix(refractColor, reflectColor, ratio);
    }

    vec3 result = emissive + globalAmbientColor* ambientStrength;
    vec3 normalVector = normalize(outNormal);

    vec3 viewVector = cameraPosition - fragPosition;
    float viewVectorLength = length(viewVector);

    viewVector = normalize(viewVector);

    for(int i =0; i< numbLights; ++i)
    {
        if(lights[i].lightType == 0)
        {
            result += CalculatePointLight(i, normalVector, viewVector, fragPosition,texCoord);
        }
        else if(lights[i].lightType == 1)
        {
            result += CalculateDirectionalLight(i, normalVector, viewVector, texCoord);
        }
        else if(lights[i].lightType == 2)
        {
            result += CalculateSpotLight(i, normalVector, viewVector, fragPosition, texCoord);
        }
    }

    //fog
    float tempNear = near;
    if(near > far)
    {
        tempNear = far;
    }

    float fogValue = (far - viewVectorLength) / (far - tempNear);
    fogValue = min(fogValue, 1.f);
    fogValue = max(fogValue, 0.f);
    

    vec3 IFinal = fogValue* result + (1-fogValue) * fogColor;
    IFinal = vec3(min(IFinal.x, 1.f), min(IFinal.y, 1.f), min(IFinal.z, 1));
    
    fragColor = vec4(mix(IFinal, resultTexture, blending), 1.f);
}