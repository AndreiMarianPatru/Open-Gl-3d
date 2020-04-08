#ifndef MESH_IND_H
#define MESH_IND_H

#include <glm\glm.hpp>
#include <glew.h>
#include "Transform.h"
#include "Vertex.h"
class Mesh_ind
{
public:
	unsigned int* _indices;
	enum VertexBuffers
	{
		POSITION_VB,
		TEXCOORD_VB,
		TANGENT_VB,
		BITANGENT_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};
	
	Transform transform;
	int m_drawCount;


	Mesh_ind(Vertex* verts,unsigned int vertCount,unsigned int* indices,unsigned int numIndices);
	void Draw();
	~Mesh_ind();	
	void PrintPosition();
	void CalculateTangentsBitangent(Vertex* verticies,unsigned int vertCount,unsigned int *indices,unsigned int numIndices);
private:
	
	/*enum {
		Position_VB,
		TextCoords_VB,
		Index_VB,
		Num_Buffers
	};*/
	GLuint m_vertexBufferObjects[NUM_BUFFERS];
	GLuint m_vertexArrayObject = 0;
};

#endif