#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Texture
{
public:
	GLuint ID = 0;
	GLenum type = 0;
	Texture() {};
	Texture(const std::string& path, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {generate(path, texType, slot, format, pixelType);};
	void generate(const std::string& path, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void bind();
	// Unbinds a texture
	void unbind();
	// Deletes a texture
	void destroy();

	// Returns if this texture has been initialized.
	bool exists();
};
#endif