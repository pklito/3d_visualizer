#include "Mesh.h"
#include<glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace glm;
Model::Model(){
    
}

void Model::generateMesh(GLfloat* vertices_data, GLsizeiptr vertices_count, GLuint* indices, GLsizeiptr indices_c){
    vao = VAO().generate();
    vao.bind();


    vbo = VBO().generate(vertices_data, vertices_count * sizeof(GLfloat));
	ebo = EBO().generate(indices, indices_c * sizeof(GLsizeiptr));
    this->indices_count = indices_c;
	// Links VBO attributes such as coordinates and colors to VAO
	vao.linkAttributes(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao.linkAttributes(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao.linkAttributes(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	vao.unbind();
    vbo.unbind();
	ebo.unbind();
}
void Model::setModel(const std::string& modeldir){
    this->loadFile(modeldir);
    // Generates Vertex Array Object and binds it
}
void Model::setTexture(const std::string& texture_dir){
    texture.generate(texture_dir, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
}

void Model::render(){
		texture.bind();
		// Bind the VAO so OpenGL knows to use it
		vao.bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, this->indices_count, GL_UNSIGNED_INT, 0);
}

void Model::destroy(){
	vao.destroy();
	vbo.destroy();
	ebo.destroy();
	texture.destroy();
	//shader.destroy();
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
    bool is_quad;

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
        is_quad = false;
		for (int i = 0; i < 4; i++)
		{
            aStream >> std::ws;
            if(aStream.peek() == EOF){
                break;
            }
            if(i == 3){
                is_quad = true;
            }
			aStream >> v[i] >> std::ws;
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
friend std::ostream& operator<<(std::ostream& os, const FaceIdcs& face) {
    for (int i = 0; i < 4; i++) {
        os << "v" << i + 1 << ": " << face.v[i] << ", ";
        os << "vn" << i + 1 << ": " << face.vn[i] << ", ";
        os << "vt" << i + 1 << ": " << face.vt[i] << "; ";
    }
    return os;
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

void Model::loadFile(const std::string& file){
    std::ifstream ifile;

	ifile.open(file.c_str());
    if(!ifile.is_open()){
        std::cerr << "Failed to open file: " + file << std::endl;
        return;
    }
	std::vector<FaceIdcs> faces;
	std::vector<vec3> vertices;
	std::vector<vec3> vertex_normals;
	std::vector<vec2> vertex_textures;
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
		if (lineType == "v")
        {
			vertices.push_back(vec3fFromStream(issLine));
        }
		else if (lineType == "f"){
			faces.push_back(issLine);
        }
		else if (lineType == "vn")
		{
			// normal to vertices
			vertex_normals.push_back(vec3fFromStream(issLine));

		}
		else if (lineType == "vt")
		{
			vertex_textures.push_back(vec2fFromStream(issLine));

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
    // Each face has 3 vertices, and 8 data floats
    bool is_quads = faces.back().is_quad;
    int face_size = is_quads ? 4 : 3;
    GLfloat *data_array = new GLfloat[faces.size() * face_size * 8];
    int face_index = 0;
    for (std::vector<FaceIdcs>::iterator it = faces.begin(); it != faces.end(); ++it)
	{
		for (int i = 0; i < face_size; i++)
		{
            int temp = 0;
            data_array[face_size*8*face_index + 8*i + 0] = vertices[it->v[i] - 1][0];
            data_array[face_size*8*face_index + 8*i + 1] = vertices[it->v[i] - 1][1];
            data_array[face_size*8*face_index + 8*i + 2] = vertices[it->v[i] - 1][2];
            temp = 0;
			if(vertex_normals.size() > 0){
				data_array[face_size*8*face_index + 8*i + 3] = vertex_normals[it->v[i] - 1][0];
				data_array[face_size*8*face_index + 8*i + 4] = vertex_normals[it->v[i] - 1][1];
				data_array[face_size*8*face_index + 8*i + 5] = vertex_normals[it->v[i] - 1][2];
			}
			if(vertex_textures.size() > 0){
				data_array[face_size*8*face_index + 8*i + 6] = vertex_textures[it->v[i] - 1][0];
				data_array[face_size*8*face_index + 8*i + 7] = vertex_textures[it->v[i] - 1][1];
			}
		}
        face_index += 1;
	}

    // create the slices, for now, its just the 0, 1, 2, 3, 4, 5 matrix
    int indices_per_face = is_quads ? 6 : 3;
    GLuint *indices_array = new GLuint[faces.size() * indices_per_face];
    if(is_quads){
        for(int i = 0; i < faces.size(); i++){
            //Tri 1
            indices_array[6*i + 0] = 4*i + 0;
            indices_array[6*i + 1] = 4*i + 1;
            indices_array[6*i + 2] = 4*i + 2;
            //Tri 2
            indices_array[6*i + 3] = 4*i + 2;
            indices_array[6*i + 4] = 4*i + 3;
            indices_array[6*i + 5] = 4*i + 0;

        }
    }
    else{
        for(int i = 0; i < faces.size() * 3; i++){
            indices_array[i] = i;
        }
    }
    generateMesh(data_array, faces.size()*face_size*8, indices_array, faces.size() * indices_per_face);
    delete[] indices_array;
    delete[] data_array;
}

void Primitive::Cube(){

}

void Primitive::Tetrahedron(){
        GLfloat vertices[] =
    { //     COORDINATES     /        COLORS      /   TexCoord  //
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
        0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
        0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
    };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    this->generateMesh(vertices, sizeof(vertices)/sizeof(GLfloat), indices, sizeof(indices)/sizeof(GLuint));
}

Primitive::Primitive(PRIM_MODEL model){
    switch(model){
        case PRIM_TETRAHEDRON:
            Tetrahedron();
        case PRIM_CUBE:
            Cube();
    }
}