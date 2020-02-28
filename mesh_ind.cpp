#include "mesh_ind.h"

#include <iostream>

Mesh_ind::Mesh_ind(Vertex* verts, unsigned int vertCount, unsigned int* indices, unsigned int numIndices)
{
	m_drawCount = numIndices;

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	

	glGenBuffers(Num_Buffers, m_vertexBufferObjects);
	glEnableVertexAttribArray(Position_VB);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[Position_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);
	glVertexAttribPointer(Position_VB, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), NULL);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferObjects[Index_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);



}

void Mesh_ind::Draw()
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh_ind::~Mesh_ind()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh_ind::PrintPosition()
{
	std::cout << "x: " << transform.getpos().x << " " << "y: " << transform.getpos().y << " " << "z: " << transform.getpos().z << std::endl;
}

