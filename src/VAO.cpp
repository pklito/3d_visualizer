#include"VAO.h"

// Constructor that generates a VAO ID
VAO& VAO::generate()
{
	glGenVertexArrays(1, &ID);
	return *this;
}

// Links a VBO Attribute such as a position or color to the VAO
void VAO::linkAttributes(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.unbind();
}

// Binds the VAO
void VAO::bind()
{
	glBindVertexArray(ID);
}

// Unbinds the VAO
void VAO::unbind()
{
	glBindVertexArray(0);
}

// Deletes the VAO
void VAO::destroy()
{
	glDeleteVertexArrays(1, &ID);
}

//
// 	VBO
//

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO& VBO::generate(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	return *this;
}

// Binds the VBO
void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::destroy()
{
	glDeleteBuffers(1, &ID);
}

//
//-----EBO------
//

// Constructor that generates a Elements Buffer Object and links it to indices
EBO& EBO::generate(GLuint* indices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	return *this;
}

// Binds the EBO
void EBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbinds the EBO
void EBO::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the EBO
void EBO::destroy()
{
	glDeleteBuffers(1, &ID);
} 