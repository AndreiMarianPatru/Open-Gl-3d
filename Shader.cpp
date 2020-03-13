#include "Shader.h"
#include <glm/gtx/string_cast.hpp>
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const string& errormessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };
	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);
	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		cerr << errormessage << " : " << error << "." << endl;
	}
}
static GLuint CreateShader(const string& ShaderSource, GLenum shaderType) 
{
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0) 
	{
		cerr << "error: shader creation failed" << endl;
	}

	const char* tempSourceCode = ShaderSource.c_str();
	glShaderSource(shader, 1, &tempSourceCode, NULL);
	glCompileShader(shader);
	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error:shader compilation failed: ");
	return shader;
}

static string LoadShader(const string& fileName)
{
	std::ifstream file;
	const char* fileNameChar = fileName.c_str();
	file.open(fileNameChar, ifstream::in);
	string output;
	string line;
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		cerr << "unable to load shader: " << fileName << endl;
	}
	/*std::cout << output;*/
	return output;
}

Shader::Shader(const string FileLocation, Camera& camera)
{
	m_Camera = &camera;
	Name = FileLocation;
	m_Program = glCreateProgram();
	m_Shaders[VERTEX] = CreateShader(LoadShader(FileLocation + ".vert"), GL_VERTEX_SHADER);
	m_Shaders[FRAGMENT] = CreateShader(LoadShader(FileLocation + ".frag"), GL_FRAGMENT_SHADER);
	for (GLuint i = 0; i < NUM_SHADER_TYPES; i++)
	{
		glAttachShader(m_Program, m_Shaders[i]);
	}
	glLinkProgram(m_Program);
	CheckShaderError(m_Program, GL_LINK_STATUS, true, "error program linking failed: ");
	glValidateProgram(m_Program);
	CheckShaderError(m_Program, GL_VALIDATE_STATUS, true, "error program is invalid: ");


	m_Uniforms[MODEL_U] = glGetUniformLocation(m_Program, "model");
	m_Uniforms[PROJECTION_U] = glGetUniformLocation(m_Program, "projection");
	m_Uniforms[VIEW_U]  = glGetUniformLocation(m_Program, "view");
}
Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADER_TYPES; i++)
	{
		glDetachShader(m_Program, m_Shaders[i]);
		glDeleteShader(m_Shaders[i]);
	}
	glDeleteProgram(m_Program);
}
void Shader::Bind()
{
	glUseProgram(m_Program);
}
void Shader::Update(Transform& transform)
{
	mat4 projection = m_Camera->returnperspective();
	mat4 view = m_Camera->GetViewProjection();
	mat4 model = transform.GetModel();
	/*std::cout <<to_string(projection) << std::endl;
	std::cout << to_string(view) << std::endl;*/
	glUniformMatrix4fv(m_Uniforms[MODEL_U], 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m_Uniforms[PROJECTION_U], 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(m_Uniforms[VIEW_U], 1, GL_FALSE, &view[0][0]);

}
GLuint Shader::GetProgram()
{
	return m_Program;
}