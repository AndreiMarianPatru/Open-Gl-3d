#ifndef LIGHTBASE_H
#define LIGHTBASE_H
#include "Transform.h"
#include "Camera.h"
#include <glew.h>
#include <SDL_opengl.h>
#include<glm/glm.hpp>
class LightBase
{
public:
	vec3 M_Color;
	
	LightBase();
	~LightBase();
	void Draw(Camera* Cam);
	Transform GetTransform();

private:
	Transform m_Transform;
};



#endif
