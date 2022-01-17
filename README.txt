/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: README.txt
Purpose: This file gives information for assignment_3.
Language: english
Platform: txt files
Project: s.park_CS300_3
Author: Sejeong Park, s.park, 180002121
Creation date: 12-03-2021
End Header --------------------------------------------------------*/ 

a.
On imgui window,

In "Normal Vector" tap,
	If click the "Vertex Normal", then it shows vertex normal line. 
	If click the "Face Normal", then it shows face normal line.
	If click the "None", then it does not shows any normal lines.
	If click the "Vertex Based Normal", then it draws object based by vertex normal, 
	and If you click the "Face Based Normal", then it draws object based by face normal.

In "Shader" tap,
	If you click "Reload Shader", then it can reload all of shaders.

In "Environment"tap,
	In "Blending", you can control blending value of environment with phong shading (0.0~1.0).
	In "Environment Mapping", you can select "Reflection only, Refraction only, Fresnel" in Environment Mapping Mode
		If you click "Refraction only", and "Fresnel", you can change Refraction index (1.0 ~ 100.0) and 
						     you can change Chromatic Aberration (0.0 ~ 1.0).
		You can select refraction index of materials like Air, Hydrogen, and so on.
		You can change Fresnel Power (0.1 ~ 10.0).
	
In "Material" tap,
	In "Surface Color", you can change ambient color and emissive of center object.

In "Global Constants" tap,
	You can change attenuation constant, global ambient, fog color.
	And also you can change the fog distance(the first one is near value, the second is far value).

In "Lighting" tap,
	In "Light Orbit", 
		If you click the "Rotation Orbit" button, the light spheres can be rotated or not.
		You can change the speed of the light spheres.
	In "Light Scenarios",
		There are three scenarios,
		In "Scenario 1", There are 8 lights. These lights of type is Point, and all of the color are same.
		In "Scenario 2", There are 7 lights. These lights of type is Spot, and all of the color are different.
		In "Scenario 3", There are 12 lights. These lights of type is different, and all of the color are also different.
	In "Light Source",
		You can change the number of lights (1 ~ 16).
		There are informations of each lights,
			You can change the type of light(Point, Directional, Spot).
			You can change the color of Ambient, Diffuse and Specular.
			There is information of position (x, z) of the light.
			If you choose spot light, You can change the value of inner angle, outer angle, and fall off.

b.
If you click minimize in window, it happens error.

c.
I completed all of parts in this assignment.

d.
nothing.

e. 
File path : Common\shaders
	- skybox.vert, skybox.frag
		: this shaders are for rendering skybox texture. In skybox.frag, there is cube mapping.
	- EnvironmentMapping.vert, EnvironmentMapping.frag
		: this shaders are for rendering center object. In vertex shader, It calculates reflection vector and refraction vector.
		In fragment shader, It calculates uv cube mapping, and also calculates lights and fog.
		The result of color in fragment shader is mixing center textures, lights and fog.
	- PhongLightinng.vert, PhongLighting.frag 
		: this shaders are for phong lighting. Lights are calculated in vertex shader.
	- PhongShading.vert, PhongShading.frag
		: this shaders are for phong shading. Lights are calculated in fragment shader.
	- BlinnShading.vert, BlinnShading.frag
		: this shaders are for blinn shading. Lights are calculated in fragment shader. 
		The reflection vector is different with phong lighting and phong shading, 
		It has half vector between light vector and view vector.
	- LightObject.vert, LightObject.frag
		: this shaders are for lights spheres. The light models have only diffuse color.
	- VertexShader.vert, FragmentShader.frag 
		: this shaders are just standard shaders.

File path : Common
	- shader.cpp : 
		//functions
		- LoadShaders : load shader

	- OBJReader.cpp :
		//functions
		- ReadOBJFile : read obj file

	- Mesh.cpp : In this file, set up vertex, indices, vertex normals, vertex normal displays, face, face normals, face normal displays
		//functions
		- CreateSphere : create sphere which is not from obj file.

File path : Assignment_3
	- FrameBuffer.cpp & FrameBuffer.h
		- In "SetUpBuffers", It calls "GenerateBuffers" and "SetBuffers".
		- In "GenerateBuffers", It generates framebuffer, texture buffers and renderbuffer.
		- In "SetBuffers", It sets up texture buffers and render buffer and then attaches them to framebuffer.
		- In "CleanUPBuffers", It deletes textures buffers, render buffer, frame buffer.
		- In "CheckFrameBufferStatus", It checks frame buffer status.
		- In "BindFrameBuffer" and "UnBindFrameBuffer", It binds and unbinds frame buffer.
		- In "AttachTexture", It helps that attaches textures to frame buffer
		- In "UseSkyTextures", It sends texture buffers to shaders.

	- Light.cpp & Light.h : 
		- In "Light.h", It has structs of LightInfo, and GlboalInfo.
		- In "Light.cpp", It has Get, Set function of lights information and global information.

	- VAOManager.cpp : 
		//functions
		- GenerateVAO() : generate vao
		- CleanUPVAO() : delete vao
		- UseVertexArrayID, UseVertexArrayID, UseFaceVertexArrayID, UseFaceNormalVertexArrayID : bind vao
		- UnUseVertexArrayID :unbind vao
	
	- VBOManager.cpp :
		//functions
		- LoadOBJFile() : load object file
		- GenerateVBO() : generate vbo
		- SetUVBuffers, SetArrayBuffers, SetIndexBuffers, SetLineBuffers, SetFaceVertexBuffers, SetFaceNormalBuffers, SetUniformBuffers, SetTextureBuffers : bind data buffers
		- SetUpBuffers() : set up all buffers
		- UseVertexBuffers, UseLineBuffers, UseFaceVertexBuffers, UseFaceNormalBuffers : bind vbo so can use
		- UnUseVertexBuffers, UnUseLineBuffers, UnUseFaceVertexBuffers, UnUseFaceNormalBuffers : unbind vbo
		- DrawLine, DrawMesh, DrawFaceMesh, DrawFaceNormalLine : draw mesh and lines using opengl function
		- CalculateOrbit() : calculate orbit vertex
		- UseUBOGlobal, UseUBOLight : bind uniform block buffers.
		- UnUseUBO : unbind uniform block buffers.
		- UseTextures : bind all of texture buffers.
		- SetUVBuffers(ObjectTypeEnum type,  Mesh* mesh) : set uv buffers of mesh.

	- Projection.cpp :
		//functions
		- CameraToWorld : matrix for camera to world, I used this function and inverse matrix in shader for "world to camera" matrix

	- ObjectManager.cpp :
		//functions
		- SetTransforms, SetTransformsForRotation : set up uniform of transforms for shader
		- DrawObject, DrawVertexNormalVector, DrawFaceNormalVector, DrawOrbit : check bool type and then draw objects and lines
		- SetUniforms, SetColor, SetLightPosition, SetUniformMatrix, SetUniformVector : set uniforms for shader
		- UseTextures : call UseTextures function of vbo manager.
		- SetGlobalUniform, SetLightsUniform : set uniform block buffers of global and lights.
		- SetInt, SetFloat : set uniforms for shader
		- SetLightInformation, SetGlobalInformation : set light information and set global information for lights.
		- GetLightInformation, GetGlobalInformation : get light information and global information, it returns each of structs.
		- GetLight : get light in vbo manager.
		- SetEnvironmentUniform : set environment information, sends these informations to shader.
		- GetFrameManager, GetVboManager : get framebuffer manager, get vbo manager.
				
	- ImGuiManager.cpp :
		//functions
		- ImGuiRender 
			: render imgui window and all of tap windows.
		- ShowEnvironmentWindow
			: it shows environment tap window, which can select reflection, refraction and fresnel, and change refraction index.
		- ShowNormalVector 
			: it shows normal vector tap window, which is showing vectors, and drawing models with vertex or face vector.
		- ShowWindowShader
			: it shows shader tap window, which can reload all of shaders.
		- ShowMaterialWindow
			: it shows material tap window, which can change material information.
		- ShowGlobalConstantsWindow
 			: it shows global constants tap window, which can change global constants information.
		- ShowLightingWindow
			: it shows lighting tap window, which can manage and change lights informations.
	
	- Camera.cpp 
		: it calculates camera matrix and has camera information

	- GLApplication.cpp : In this file, create window
	
	- CS300_Project_Scene.cpp : In this file, create current scene

	
File path : ThirdParty
	- GL, GLEW, GLFW, glm, imgui : this is library.

f.
OS: Microsoft Windows 10 Home
GPU: Intel(R) HD Graphics 620
OpenGL: 4.5

g.
50 ~ 60 hours.

h.
Keyboard Input : up down : changing pitch of camera
	          left right : changing yaw of camera


