#pragma once

#include <glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include "Transform.h"
#include "Camera.h"
#include "LightBase.h"
using namespace std;
class Shader
{
public:
	enum ShaderTypes
	{
		VERTEX,
		FRAGMENT,
		NUM_SHADER_TYPES
	};
	enum UniformNames
	{
		MODEL_U,
		PROJECTION_U,
		VIEW_U,
		FRAG_LIGHTCOLOR_U,
		FRAG_LIGHTPOS_U,
		FRAG_CAMERAPOS_U,
		NUM_UNIFORMS
	};
	Shader(const string FileLocation, Camera& camera);
	~Shader();
	void Update(Transform& transform, LightBase& light);
	void Bind();
	GLuint GetProgram();
private:
	string Name;
	GLuint m_Program;
	Camera* m_Camera;
	GLuint m_Shaders[NUM_SHADER_TYPES];
	GLuint m_Uniforms[NUM_UNIFORMS];
};

