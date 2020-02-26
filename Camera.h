#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include "Transform.h"

class Camera
{
	mat4 m_perspective;
	vec3 m_position;
	vec3 m_forward;
	vec3 m_right;
	vec3 m_up;
public:
	Transform cameraTransform;
	
	Camera(float fov,float aspect, float zNear,float zFar)
{
		cameraTransform.setpos(vec3(0,0,-3));
		m_perspective = glm::perspective(fov, aspect, zNear, zFar);
		
		
		
		
}
	~Camera(){};
	mat4 returnperspective()
	{
		
		return m_perspective;
	}

	inline mat4 GetViewProjection(vec3 myvec)
	{
		m_position = cameraTransform.getpos();
		m_forward = normalize(vec3(0) - m_position);
		m_right = normalize(cross(vec3(0, 1, 0), m_forward));
		m_up = normalize(cross(m_forward, m_right));
		return  glm::lookAt(m_position, myvec, m_up);
	}
	
	
};

