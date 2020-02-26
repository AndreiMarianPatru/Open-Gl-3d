#define GLEW_STATIC
#include <glew.h>
#include <SDL_opengl.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<SDL.h>
#include <iostream>
#include "mesh.h"
#include "Camera.h"


using namespace std;

void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const string& errormessage)
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

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	SDL_Window* window = SDL_CreateWindow("myapp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext GLContext = SDL_GL_CreateContext(window);

	Camera camera( 70.0f, 800.0f / 600.0f, 0.01f, 100.0f);

	mat4 camera_perspective = camera.returnperspective();

	
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	if (status != GLEW_OK)
	{
		cout << "glew failed to init" << endl;
	}


	float Vertices1[]
	{
		0.0f,0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		-0.5f,-0.5f,0.0f
	};
	float Vertices2[]
	{
		1.0f,0.5f,0.0f,
		1.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f
	};

	GLuint VertexBufferObject = 0;
	glGenBuffers(1, &VertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), Vertices1, GL_STATIC_DRAW);

	GLuint VertexArrayObject = 0;
	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);

	const char* VertexShaderCode =
		"#version 450\n"
		"in vec3 vp;"
		"uniform mat4 view;"
		"uniform mat4 perspective;"
		"uniform mat4 model;"
		"void main(){"
		"  gl_Position = perspective*view*model*vec4(vp,1.0);"
		//"  gl_Position = view*model*vec4(vp,1.0);"
		"}";
	const char* FragmentShaderCode =
		"#version 450\n"
		"out vec4 frag_colour;"
		"void main(){"
		"frag_colour=vec4(0.6,0.0,0.5,1.0);"//colour of triangle
		"}";

	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderCode, NULL);
	glCompileShader(VertexShader);

	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderCode, NULL);
	glCompileShader(FragmentShader);

	GLuint ShaderPrograme = glCreateProgram();
	glAttachShader(ShaderPrograme, VertexShader);
	glAttachShader(ShaderPrograme, FragmentShader);

	glLinkProgram(ShaderPrograme);

	CheckShaderError(ShaderPrograme, GL_LINK_STATUS, true, "err program linking failed: ");
	glValidateProgram(ShaderPrograme);
	CheckShaderError(ShaderPrograme, GL_VALIDATE_STATUS, true, "err program is invalid ");

	Mesh Tri1(Vertices1, 3);
	Mesh Tri2(Vertices2, 3);

	
	glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
	glViewport(0, 0, 800, 600);


	Tri1.transform.setscale(vec3(1));
	Tri1.transform.setpos(vec3(0.1, 0.3, 0));
	Tri2.transform.setscale(vec3(1));
	Tri2.transform.setpos(vec3(0.1, 0.3, 0));

	vec3 viewvec;
	viewvec = vec3(0, 0, 0);
	while (true)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			
			//User presses a key
			if (event.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(0,1,0));
					viewvec.y += 1;
					break;
				case SDLK_s:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(0, -1, 0));
					viewvec.y -= 1;
					break;
				case SDLK_a:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(1, 0, 0));
					viewvec.x += 1;
					break;
				case SDLK_d:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(-1, 0, 0));
					viewvec.x -= 1;

					break;
				case SDLK_q:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(0, 0, 1));
					break;
				case SDLK_e:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(0, 0, -1));
					break;
				}

			}
			
		}
		
		//glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(ShaderPrograme);
		glBindVertexArray(VertexArrayObject);

		
		
		mat4 camera_view = camera.GetViewProjection(viewvec);


		GLint perspectiveLoc = glGetUniformLocation(ShaderPrograme, "perspective");
		glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, &camera_perspective[0][0]);
		
		GLint viewLoc = glGetUniformLocation(ShaderPrograme, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera_view[0][0]);

		
		GLint modelLoc = glGetUniformLocation(ShaderPrograme, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &Tri1.transform.GetModel()[0][0]);

		
		
		
		
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		Tri1.Draw();
		Tri2.Draw();
		//glViewport(0, 0, 800, 600);
		SDL_Delay(16);
		SDL_GL_SwapWindow(window);

	}

	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();




	std::cout << "Hello World!\n";
	return 0;
}
