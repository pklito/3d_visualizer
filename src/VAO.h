#pragma once

#include<glad/glad.h>

//
//		VBO
//

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO() {};
	VBO& generate(GLfloat* vertices, GLsizeiptr size);

	// Binds the VBO
	void bind();
	// Unbinds the VBO
	void unbind();
	// Deletes the VBO
	void destroy();
};

//
//     EBO
//

class EBO
{
public:
	// ID reference of Elements Buffer Object
	GLuint ID;
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO() {};
	EBO& generate(GLuint* indices, GLsizeiptr size);
	// Binds the EBO
	void bind();
	// Unbinds the EBO
	void unbind();
	// Deletes the EBO
	void destroy();
};

class VAO
{
public:
	// ID reference for the Vertex Array Object
	GLuint ID = 0;
	// Constructor that generates a VAO ID
	VAO() {};

	VAO& generate();
	// Links a VBO Attribute such as a position or color to the VAO
	void linkAttributes(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Binds the VAO
	void bind();
	// Unbinds the VAO
	static void unbind();
	// Deletes the VAO
	void destroy();
};