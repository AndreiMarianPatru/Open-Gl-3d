#include "mesh_ind.h"

#include <iostream>
#include <vector>
#include <glm/gtx/normal.hpp>

Mesh_ind::Mesh_ind(Vertex* verts, unsigned int vertCount, unsigned int* indices, unsigned int numIndices)
{
	_indices=indices;
	m_drawCount = numIndices;
	std::vector<vec3> positions;
	std::vector<vec2> texCoords;

	for(unsigned int i=0;i<vertCount;i++)
	{
		positions.push_back(verts[i].Position);
		texCoords.push_back(verts[i].TextureCoord);
	}

	std::vector<vec3> Normals;
	Normals.resize(vertCount);

	for(int i=0; i<numIndices;i+=3)
	{
		vec3 Vert1= positions[indices[i]];
		vec3 Vert2= positions[indices[i+1]];
		vec3 Vert3= positions[indices[i+2]];

		vec3 normal= triangleNormal(Vert1,Vert2,Vert3);
		Normals[indices[i]]+= normal;
		Normals[indices[i+1]]+= normal;
		Normals[indices[i+2]]+= normal;
	}

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	glGenBuffers(NUM_BUFFERS, m_vertexBufferObjects); //
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(POSITION_VB, 3, GL_FLOAT, GL_FALSE,0,0);
	glEnableVertexAttribArray(POSITION_VB);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(TEXCOORD_VB, 2, GL_FLOAT, GL_FALSE,0,0);
	glEnableVertexAttribArray(TEXCOORD_VB);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Normals[0]), &Normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(NORMAL_VB, 3, GL_FLOAT, GL_FALSE,0,0);
	glEnableVertexAttribArray(NORMAL_VB);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferObjects[INDEX_VB]);
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

