#include "VertexArray.h"
#include <glew.h>

VertexArray::VertexArray(const float* verticesP, unsigned int nbVerticesP, const unsigned int* indicesP, unsigned int nbIndicesP) :
	nbVertices(nbVerticesP), nbIndices(nbIndicesP), vertexBuffer(0), indexBuffer(0), vertexArray(0)
{
	
	// Create a Vertex Array
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// Create a Vertex buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, nbVertices * 3.0f * sizeof(float), verticesP, GL_STATIC_DRAW);

	// Create a index buffer
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, nbIndices * 3.0f * sizeof(unsigned int), indicesP, GL_STATIC_DRAW);

	// Specify vertex Attributes
	// For now we assume the folling vertex format: Position is 3 floats starting at offset 0.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vertexArray);
}

void VertexArray::setActive()
{
	glBindVertexArray(vertexArray);
}