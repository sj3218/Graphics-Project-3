/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectTypeEnum.h
Purpose: This file is for declaration for ObjectTypeEnum and LineTypeEnum class.
Language: C++, VS
Platform: Visual Studio 2019, OpenGL 4.5, Window
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 10-01-2021
End Header --------------------------------------------------------*/
#pragma once

enum class ObjectTypeEnum
{
    eSPHERE =0,
    eSPHERE_MODIFIED,
    e4SPHERE,
    eCUBE,
    eBUNNY,
    eCREATE_SPHERE,
    eQUAD,
    COUNT
};

enum class LineTypeEnum
{
    eSPHERE = 0,
    eSPHERE_MODIFIED,
    e4SPHERE,
    eCUBE,
    eBUNNY,
    eCREATE_SPHERE,
    eQUAD,
    eORBIT,
    COUNT
};