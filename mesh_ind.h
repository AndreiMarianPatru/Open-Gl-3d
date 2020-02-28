#ifndef MESH_IND_H
#define MESH_IND_H

#include <glm\glm.hpp>
#include <glew.h>
#include "Transform.h"
#include "Vertex.h"
class Mesh_ind
{
public:
	Transform transform;
	int m_drawCount;


	Mesh_ind(Vertex* verts,unsigned int vertCount,unsigned int* indices,unsigned int numIndices);
	void Draw();
	~Mesh_ind();	
	void PrintPosition();
private:
	
	enum {
		Position_VB,
		Index_VB,
		Num_Buffers
	};
	GLuint m_vertexBufferObjects[Num_Buffers];
	GLuint m_vertexArrayObject = 0;
};

#endif