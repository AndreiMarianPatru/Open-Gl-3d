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
	void RotateY(float amount) 
	{
		vec3 forwardCopy = m_forward;
		vec3 rightCopy = m_right;

		amount/= 57.2957795f; //transform it to radians
		
		m_forward.x= (cos(1.5708f + amount) * forwardCopy.x) + (cos(amount) * rightCopy.x);
		m_forward.y = (cos(1.5708f + amount) * forwardCopy.y) + (cos(amount) * rightCopy.y);
		m_forward.z = (cos(1.5708f + amount) * forwardCopy.z) + (cos(amount) * rightCopy.z);
		m_right.x = (cos(amount) * forwardCopy.x) + (cos(1.5708f - amount) * rightCopy.x);
		m_right.y = (cos(amount) * forwardCopy.y) + (cos(1.5708f - amount) * rightCopy.y);
		m_right.z= (cos(amount) * forwardCopy.z) + (cos(1.5708f - amount) * rightCopy.z);
		normalize(m_forward);
		normalize(m_right);
	}
	vec3 retrunUP() const 
	{
		return m_up;
	}
	
};

