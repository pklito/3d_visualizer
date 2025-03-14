#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<glm/glm.hpp>
#include"util.h"

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activates the Shader Program
	void activate();
	// Deletes the Shader Program
	void destroy();

	void setFloat(const std::string& name, float time);
	void setMat4(const std::string& name, const glm::mat4& mat);
	void setVec4(const std::string& name, const glm::vec4& vec);
	
	// Deactivates shaders
	static void deactivate();
private:
	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char* type);
};


#endif