#include "Mesh.h"
#include "glm.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace glm;
Model::Model(){
    
}

void Model::generateMesh(VertexData& location, VertexData& normals, VertexData& textures){
    vao = VAO().generate();
    vao.bind();

    vbo = VBO().generate(location.data, location.data_length);
	ebo = EBO().generate(location.indices, location.indices_length);

	// Links VBO attributes such as coordinates and colors to VAO
	vao.linkAttributes(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao.linkAttributes(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao.linkAttributes(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	vao.unbind();
    vbo.unbind();
	ebo.unbind();
}
void Model::setModel(std::string modeldir){

    // Generates Vertex Array Object and binds it
}

void Model::render(){

		brickTex.bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
}

void Model::destroy(){
	VAO1.destroy();
	VBO1.destroy();
	EBO1.destroy();
	brickTex.destroy();
	shaderProgram.destroy();
}


/// -----------------
//
//
//  FILE MESH GEN
//
//
/// ----------------

struct FaceIdcs
{
	int v[4];
	int vn[4];
	int vt[4];

	FaceIdcs()
	{
		for (int i = 0; i < 4; i++)
			v[i] = vn[i] = vt[i] = 0;
	}

	FaceIdcs(std::istream &aStream)
	{
		for (int i = 0; i < 4; i++)
			v[i] = vn[i] = vt[i] = 0;

		char c;
		for (int i = 0; i < 3; i++)
		{
			aStream >> std::ws >> v[i] >> std::ws;
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> std::ws;
			if (aStream.peek() == '/')
			{
				aStream >> c >> std::ws >> vn[i];
				continue;
			}
			else
				aStream >> vt[i];
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> vn[i];
		}
	}
};

vec3 vec3fFromStream(std::istream &aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return vec3(x, y, z);
}

vec2 vec2fFromStream(std::istream &aStream)
{
	float x, y;
	aStream >> x >> std::ws >> y;
	return vec2(x, y);
}

void Model::loadFile(std::string file){
    std::ifstream ifile(file.c_str());
	std::vector<FaceIdcs> faces;
	std::vector<vec3> vertices;
	std::vector<vec3> vertex_normals;
	std::vector<vec3> vertex_textures;
	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		getline(ifile, curLine);

		// read type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v") // BUG FIXED
			vertices.push_back(vec3fFromStream(issLine));
		else if (lineType == "f") // BUG FIXED
			faces.push_back(issLine);
		else if (lineType == "vn")
		{
			// normal to vertices
			vertex_normals.push_back(vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			vertex_textures.push_back(vec3fFromStream(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cerr << "Found unknown line Type \"" << lineType << "\"";
		}
	}


}