#define GLEW_STATIC
#include <glew.h>
#include <SDL_opengl.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<SDL.h>
#include <iostream>
#include "mesh.h"
#include "Camera.h"
#include "Vertex.h"
#include <vector>
#include "mesh_ind.h"


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

	SDL_Window* window = SDL_CreateWindow("myapp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext GLContext = SDL_GL_CreateContext(window);

	Camera camera( 70.0f, 1000.0f / 1000.0f, 0.01f, 1000.0f);

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
		-1.5f,-0.5f,0.0f
	};
	float Vertices2[]
	{
		1.0f,0.5f,2.5f,
		1.5f,-0.5f,2.5f,
		-0.5f,-0.5f,2.5f
	};
	float Vertices3[]
	{
		2.0f,0.5f,5.0f,
		2.5f,-0.5f,5.0f,
		0.5f,-0.5f,5.0f
	};



	vector<Vertex> SquareVertices;

	SquareVertices.push_back(Vertex(-0.5f, 1.5f, 0.0f));
	SquareVertices.push_back(Vertex(0.5f, 1.5f, 0.0f));
	SquareVertices.push_back(Vertex(0.5f, 0.5f, 0.0f));
	SquareVertices.push_back(Vertex(-0.5f, 0.5f, 0.0f));
	unsigned int SquareIndecies[]
	{
		0,1,2,0,2,3
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
		"uniform vec3 Color;"
		"out vec4 frag_colour;"
		"void main(){"
		//"frag_colour=vec4(0.6,0.0,0.5,1.0);"//colour of triangle
		"frag_colour=vec4(Color,1.0);"//colour of triangle
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
	Mesh Tri3(Vertices3,3);
	

	
	glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
	glViewport(0, 0, 1000, 1000);


	Tri1.transform.setscale(vec3(1));
	Tri1.transform.setpos(vec3(0.1, 0.3, 0));
	Tri2.transform.setscale(vec3(1));
	Tri2.transform.setpos(vec3(0.1, 0.3, 0));
	Tri3.transform.setscale(vec3(1));
	Tri3.transform.setpos(vec3(0.1, 0.3, 0));


	Mesh_ind square(&SquareVertices[0],SquareVertices.size(),&SquareIndecies[0],6);
	square.transform.setscale(vec3(1));

	vec3 viewvec;
	viewvec = vec3(0, 0, 0);
	while (true)
	{
		//std::cout<<camera.retrunUP().x<<" "<<camera.retrunUP().y<<" "<<camera.retrunUP().y<<std::endl;
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
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(0,-1,0));
					viewvec.y -= 1;
					break;
				case SDLK_s:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(0, 1, 0));
					viewvec.y += 1;
					break;
				case SDLK_a:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(-1, 0, 0));
					viewvec.x -= 1;
					break;
				case SDLK_d:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(1, 0, 0));
					viewvec.x += 1;

					break;
				case SDLK_q:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(0, 0, 1));
					viewvec.z += 1;
					break;
				case SDLK_e:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(0, 0, -1));
					viewvec.z -= 1;
					break;
				case SDLK_LEFT:
					viewvec.x+=2;
					
					break;
				case SDLK_RIGHT:
					viewvec.x-=2;									
					break;
				case SDLK_UP:
					viewvec.y-=2;
					break;
				case SDLK_DOWN:
					viewvec.x-=2;
				}

			}
			
		}
		

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


		GLint ColorLoc = glGetUniformLocation(ShaderPrograme, "Color");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &Tri1.transform.GetModel()[0][0]);
		vec3 color = vec3(1.0f,1.0f,0.0f);
		glUniform3f(ColorLoc, color.x, color.y, color.z);
		

		

		
		
		
		
	

		Tri1.Draw();
		Tri2.Draw();
		Tri3.Draw();
		square.Draw();

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
