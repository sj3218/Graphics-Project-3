/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: BlinnShading.frag
Purpose: This file is fragment shader for Blinn Shading.
Language: glsl, Visual Studio Code
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-20-2021
End Header --------------------------------------------------------*/
#version 450 core
#define PI 4.0f * atan(1.0f)

in vec3 fragPosition;
in vec3 outNormal;
in vec2 texCoord;

out vec4 fragColor;

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
uniform bool isTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform vec3 ambientMaterial;
uniform vec3 emissive;
uniform vec3 cameraPosition;
uniform vec3 color;

uniform bool calculateUV_GPU;
uniform int projectionMode;
uniform bool entities;

uniform mat4 transform;
uniform mat4 cameraMatrix;
uniform mat4 ndcMatrix;

vec3 CalculateDirectionalLight(int index, vec3 normal, vec3 view)
{
    int Ns = 64;
    vec3 lightDir = normalize(-lights[index].direction);
	vec3 reflectDir = (lightDir + view);
    reflectDir = normalize(reflectDir);

    float diffuseValue = max(dot(normal, lightDir),0.f);
    float specularValue = pow(max(dot(normal, reflectDir), 0.f), Ns);

    vec3 ambient = lights[index].ambientColor * ambientMaterial * ambientStrength;
    vec3 diffuse = lights[index].diffuseColor * diffuseValue;
    vec3 specular = lights[index].specularColor * specularValue;

    if(isTexture)
    {
        diffuse *= vec3(texture(diffuseTexture, texCoord).rgb);
        specular *= vec3(texture(specularTexture, texCoord).rgb);
    }

    return ambient + diffuse + specular;
}

vec3 CalculatePointLight(int index, vec3 normal, vec3 view, vec3 pos)
{
    int Ns = 64;
    vec3 lightDir = lights[index].lightPosition - pos;
    float lightDirLength = length(lightDir);
    
    float attenuationValue = min(1/(attenuation.x + attenuation.y * lightDirLength + attenuation.z * lightDirLength*lightDirLength), 1.f);

    lightDir = normalize(lightDir);

	vec3 reflectDir = (lightDir + view);
    reflectDir = normalize(reflectDir);

    float diffuseValue = max(dot(normal, lightDir), 0.f);
    float specularValue = pow(max(dot(normal, reflectDir), 0.f), Ns);

    vec3 ambient = lights[index].ambientColor * ambientMaterial * ambientStrength;
    vec3 diffuse = lights[index].diffuseColor * diffuseValue;
    vec3 specular = lights[index].specularColor * specularValue;

    if(isTexture)
    {
        diffuse *= vec3(texture(diffuseTexture, texCoord).rgb);
        specular *= vec3(texture(specularTexture, texCoord).rgb);
    }

    return (ambient + diffuse + specular) * attenuationValue;
}

vec3 CalculateSpotLight(int index, vec3 normal, vec3 view, vec3 pos)
{
    int Ns = 64;
    vec3 lightDir = lights[index].lightPosition- pos;

    float lightDirLength = length(lightDir);
    
    float attenuationValue = min(1/(attenuation.x + attenuation.y * lightDirLength + attenuation.z * lightDirLength*lightDirLength), 1.f);

    lightDir = normalize(lightDir);
	vec3 reflectDir = (lightDir + view);
    reflectDir = normalize(reflectDir);

    float diffuseValue = max(dot(normal, lightDir), 0.f);
    float specularValue = pow(max(dot(normal, reflectDir), 0.f), Ns);

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

    if(isTexture)
    {
        diffuse *= vec3(texture(diffuseTexture, texCoord).rgb);
        specular *= vec3(texture(specularTexture, texCoord).rgb);
    }


    return attenuationValue*(ambient + spotLightEffect*(diffuse + specular));
}

void main()
{
    vec2 texture_coordinate;
    if(calculateUV_GPU)
    {
        float u = 0.f;
        float v = 0.f;
        float theta = 0.f;
        float phi = 0.f;

        vec3 calculatePosition = fragPosition;

        if(entities)
        {
            calculatePosition = normalize(outNormal);
        }

        if(projectionMode == 1) // cylindrical uv
        {
            theta = atan(calculatePosition.z, calculatePosition.x);
            theta += PI;
            
            u = theta/(2.f*PI);
            v = (calculatePosition.y + 1.0f) *0.5f;
        }
        else if(projectionMode == 2) // spherical uv
        {
            theta = atan(calculatePosition.z , calculatePosition.x);
            theta += PI;

            phi = acos(calculatePosition.y/ length(calculatePosition));
            
            u = theta/(2.f*PI);
            v = 1.f - (phi / PI);
        }
        else if(projectionMode == 3) // cube uv
        {
            float x = calculatePosition.x;
            float y = calculatePosition.y;
            float z = calculatePosition.z;

            float absX = abs(x);
            float absY = abs(y);
            float absZ = abs(z);

            int isXPositive = x > 0 ? 1 : 0;
            int isYPositive = y > 0 ? 1 :0;
            int isZPositive = z > 0 ? 1 :0;

            float maxAxis = 0.f;
            float uc = 0.f;
            float vc = 0.f;
    
            // POSITIVE X
            if (bool(isXPositive) && (absX >= absY) && (absX >= absZ))
            {
                maxAxis = absX;
                uc = -z;
                vc = y;
            }
            // NEGATIVE X
            else if (!bool(isXPositive) && absX >= absY && absX >= absZ)
            {
                maxAxis = absX;
                uc = z;
                vc = y;
            }

            // POSITIVE Y
            else if (bool(isYPositive) && absY >= absX && absY >= absZ)
            {
                maxAxis = absY;
                uc = x;
                vc = -z;
            }

            // NEGATIVE Y
            else if (!bool(isYPositive) && absY >= absX && absY >= absZ)
            {
                maxAxis = absY;
                uc = x;
                vc = z;
            }

            // POSITIVE Z
            else if (bool(isZPositive) && absZ >= absX && absZ >= absY)
            {
                maxAxis = absZ;
                uc = x;
                vc = y;
            }

            // NEGATIVE Z
            else if (!bool(isZPositive) && absZ >= absX && absZ >= absY)
            {
                maxAxis = absZ;
                uc = -x;
                vc = y;
            }

            // Convert range from -1 to 1 to 0 to 1
            u = 0.5f * (uc / maxAxis + 1.0f);
            v = 0.5f * (vc / maxAxis + 1.0f);
        }

        texture_coordinate.x = u;
        texture_coordinate.y = v;
    }
    else
    {
        texture_coordinate = texCoord;
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
            result += CalculatePointLight(i, normalVector, viewVector, fragPosition);
        }
        else if(lights[i].lightType == 1)
        {
            result += CalculateDirectionalLight(i, normalVector, viewVector);
        }
        else if(lights[i].lightType == 2)
        {
            result += CalculateSpotLight(i, normalVector, viewVector, fragPosition);
        }
    }

    //fog
    float tempNear = near;
    if(near > far)
    {
        tempNear = far;
    }
    float fogValue = (far - viewVectorLength) / (far-tempNear);
    fogValue = min(fogValue, 1.f);
    fogValue = max(fogValue, 0.f);


    vec3 IFinal = fogValue* result + (1-fogValue) * fogColor;
    IFinal = vec3(min(IFinal.x, 1.f), min(IFinal.y, 1.f), min(IFinal.z, 1));
    
    fragColor = vec4(IFinal, 1.f);
    
}