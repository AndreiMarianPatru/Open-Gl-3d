#define GLEW_STATIC
#include <glew.h>
#include <SDL_opengl.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<SDL.h>
#include <iostream>
#include <vector>

#include "mesh.h"
#include "Camera.h"
#include "Vertex.h"
#include "mesh_ind.h"
#include "Shader.h"
#include "LightBase.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



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




GLuint LoadTexture(string TextureLocation) 
{
	GLuint textureID;
	int width, height, numComponents;
	unsigned char* ImageData = stbi_load(TextureLocation.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);
	if (ImageData == NULL)
	{
		cerr << "texture loading failed for texture: " << TextureLocation << endl;
	}
	GLenum format;
	if (numComponents == 1)
		format = GL_RED;
	if (numComponents == 3)
		format = GL_RGB;
	if (numComponents == 4)
		format = GL_RGBA;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(ImageData);
	return textureID;
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
	LightBase* light= new LightBase();

	

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

	SquareVertices.push_back(Vertex(vec3(-2.0f, 0.5f, 1.0f),vec2(-2,0.5f))); // top right
	SquareVertices.push_back(Vertex(vec3(0.5f, 0.5f, 1.0f),vec2(0.5f,0.5f))); // top left
	SquareVertices.push_back(Vertex(vec3(0.5f, -0.5f, 1.0f),vec2(0.5f,-0.5f))); // bottom left
	SquareVertices.push_back(Vertex(vec3(-0.5f, -0.5f, 1.0f),vec2(-0.5f,-0.5f)));// bottom right
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

	
	Shader* basicShader = new Shader("resources/Basic", camera);
	GLuint DiffuseTextureID= LoadTexture("brickwall.jpg");
	GLuint NormalTextureID= LoadTexture("brickwall_normal.jpg");

	Mesh Tri1(Vertices1, 3);
	/*Mesh_ind Tri2(Vertices2, 3);
	Mesh_ind Tri3(Vertices3,3);*/
	

	
	glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
	glViewport(0, 0, 1000, 1000);


	Tri1.transform.setpos(vec3(1, 0.4f, -1.0f));
	Tri1.transform.setscale(vec3(2));
	/*Tri3.transform.setscale(vec3(2));
	Tri2.transform.setscale(vec3(1));
	Tri2.transform.setpos(vec3(0.1, 0.3, 0));
	Tri3.transform.setscale(vec3(1));
	Tri3.transform.setpos(vec3(0.1, 0.3, 0));
*/

	Mesh_ind square(&SquareVertices[0],SquareVertices.size(),&SquareIndecies[0],6);
	square.transform.setscale(vec3(1));

	vec3 viewvec;
	viewvec = vec3(0, 0, 0);

	LoadTexture("brickwall.jpg");

	
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
					//camera movement
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
					//camera movement(zoom)
				case SDLK_q:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(0, 0, 1));
					viewvec.z += 1;
					break;
				case SDLK_e:
					camera.cameraTransform.setpos(camera.cameraTransform.getpos() + vec3(0, 0, -1));
					viewvec.z -= 1;
					break;
				//camera rotation
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

		glBindVertexArray(VertexArrayObject);
		
		light->Draw(&camera);
		
		camera.setviewvec(viewvec);
		mat4 camera_view = camera.GetViewProjection();

		basicShader->Bind();		

		glActiveTexture(GL_TEXTURE0);
		GLuint TextureLoc = glGetUniformLocation(basicShader->GetProgram(), "texture_diffuse");
		glUniform1i(TextureLoc, 0);
		glBindTexture(GL_TEXTURE_2D, DiffuseTextureID);
		
		glActiveTexture(GL_TEXTURE1);
		TextureLoc = glGetUniformLocation(basicShader->GetProgram(), "texture_normal");
		glUniform1i(TextureLoc, 1);
		glBindTexture(GL_TEXTURE_2D, NormalTextureID);


		basicShader->Update(square.transform,*light);

		
		
		
		//Tri1.Draw();
		square.Draw();

		
		/*Tri2.Draw();
		Tri3.Draw();*/

	
		



		glUseProgram(0);


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
