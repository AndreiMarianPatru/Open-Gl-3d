#include "LightBase.h"

LightBase::LightBase()
{
	M_Color=vec3(1,1,1);
}

LightBase::~LightBase()
{
}

void LightBase::Draw(Camera* Cam)
{
	glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const GLfloat*)&Cam->returnperspective());
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 MV= Cam->GetViewProjection()* m_Transform.GetModel();
	glLoadMatrixf((const GLfloat*)&MV[0]);

	glBegin(GL_LINES);
	vec3 p1=this->m_Transform.getpos();
	vec3 p2=p1;

	glColor3f(1,0,0);
	glVertex3fv(&p1.x);
	p2=p1+ vec3(1,0,0)*0.1f;
	glColor3f(1,0,0);
	glVertex3fv(&p2.x);

	glColor3f(0,1,0);
	glVertex3fv(&p1.x);
	p2=p1+vec3 (0,1,0)*0.1f;
	glColor3f(0,1,0);
	glVertex3fv(&p2.x);

	glColor3f(0,0,1);
	glVertex3fv(&p1.x);
	p2=p1+vec3 (0,0,1)*0.1f;
	glColor3f(0,0,1);
	glVertex3fv(&p2.x);
	glEnd();
}

Transform LightBase::GetTransform()
{
	return this->m_Transform;
}
