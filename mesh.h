#ifndef MESH_H
#define MESH_H

#include <glm\glm.hpp>
#include <glew.h>
#include "Transform.h"
#include "Vertex.h"
class Mesh
{
public:
	Mesh(float* verts, unsigned int vertCount);
	void Draw();
	~Mesh();
	Transform transform;
	void PrintPosition();
	void CalculateTangentsBitangent(Vertex* verticies,unsigned int vertCount,unsigned int *indices,unsigned int numIndices);
private:
	GLuint m_vertexBufferObject = 0;
	GLuint m_vertexArrayObject = 0;
};

#endif