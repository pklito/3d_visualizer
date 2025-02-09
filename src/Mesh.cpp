#include "Mesh.h"
#include<glm/glm.hpp>
#include"util.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include<glm/gtc/matrix_transform.hpp>
using namespace glm;
Model::Model() : _world_transform(1.0f), _model_transform(1.0f), _world_normal_transform(1.0f), _model_normal_transform(1.0f), size(1.0f,1.0f,1.0f){
}

void Model::generateMesh(GLfloat* vertices_data, GLsizeiptr vertices_count, GLuint* indices, GLsizeiptr indices_c){
    vao = VAO().generate();
    vao.bind();

    vbo = VBO().generate(vertices_data, vertices_count * sizeof(GLfloat));
	ebo = EBO().generate(indices, indices_c * sizeof(GLuint));
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
	int i = 0;
	if(modeldir.length() < 1){
		std::cerr << "Invalid model file: " + modeldir << std::endl;
		return;
	}
	else {
		i = 2;
	}
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
		glDrawElements(this->render_type, this->indices_count, GL_UNSIGNED_INT, 0);
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
		// Generate normals if not existing in obj file.
		if(vertex_normals.size() == 0){
			vec3 p1 = vec3(data_array[face_size*8*face_index + 0], data_array[face_size*8*face_index + 1], data_array[face_size*8*face_index + 2]);
			vec3 p2 = vec3(data_array[face_size*8*face_index + 8], data_array[face_size*8*face_index + 9], data_array[face_size*8*face_index + 10]);
			vec3 p3 = vec3(data_array[face_size*8*face_index + 16], data_array[face_size*8*face_index + 17], data_array[face_size*8*face_index + 18]);
			vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
			for (int i = 0; i < face_size; i++)
			{
				data_array[face_size*8*face_index + 8*i + 3] = normal[0];
				data_array[face_size*8*face_index + 8*i + 4] = normal[1];
				data_array[face_size*8*face_index + 8*i + 5] = normal[2];
			}
		}
		// Generate texture coordinates if not existing in obj file.
		if(vertex_textures.size() == 0){
			for (int i = 0; i < face_size; i++)
			{
				vec3 coord = vec3(data_array[face_size*8*face_index + 8*i + 0], data_array[face_size*8*face_index + 8*i + 1], data_array[face_size*8*face_index + 8*i + 2]);
				vec2 tex_coord = vec2((0.5*(atan2(coord.z, coord.x))/glm::pi<float>()), coord.y);
				data_array[face_size*8*face_index + 8*i + 6] = tex_coord[0];
				data_array[face_size*8*face_index + 8*i + 7] = tex_coord[1];
			}
		}
		// Increment to next (3 or 4) * 8 data floats
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

//		--------------------	//
// 			PRIMITIVES			//
//		--------------------	//

void Primitive::Cube(){

}

void Primitive::Tetrahedron(){
        GLfloat vertices[] =
    { //     COORDINATES     /        COLORS      /   TexCoord  //
        -0.5f, 0.0f,  0.5f,     -1.f, 0.0f,  0.5f,	0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,     -1.f, 0.0f, -0.5f,	5.0f, 0.0f,
        0.5f, 0.0f, -0.5f,     0.5f, 0.0f, -1.f,	0.0f, 0.0f,
        0.5f, 0.0f,  0.5f,     0.5f, 0.0f, 1.f,	5.0f, 0.0f,
        0.0f, 1.f,  0.0f,     0.f, 1.f, 0.f,	2.5f, 5.0f
    };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 1,
        1, 4, 2,
        2, 4, 3,
        3, 4, 0
    };

    this->generateMesh(vertices, sizeof(vertices)/sizeof(GLfloat), indices, sizeof(indices)/sizeof(GLuint));
}

void Primitive::Grid(){
	GLfloat vertices[] = {
-0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,                -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
-0.4f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.1f, 0.5f,                -0.4f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.1f, 0.5f,
-0.3f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.2f, 0.5f,                -0.3f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.2f, 0.5f,
-0.2f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.3f, 0.5f,                -0.2f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.3f, 0.5f,
-0.1f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.4f, 0.5f,                -0.1f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.4f, 0.5f,
0.0f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f,                 0.0f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f,
0.1f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.6f, 0.5f,                 0.1f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.6f, 0.5f,
0.2f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.7f, 0.5f,                 0.2f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.7f, 0.5f,
0.3f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.8f, 0.5f,                 0.3f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.8f, 0.5f,
0.4f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.9f, 0.5f,                 0.4f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.9f, 0.5f,
0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,                 0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,
0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,                -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
0.5f, 0.0f, -0.4f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,                -0.5f, 0.0f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
0.5f, 0.0f, -0.3f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,                -0.5f, 0.0f, -0.3f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
0.5f, 0.0f, -0.2f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,                -0.5f, 0.0f, -0.2f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
0.5f, 0.0f, -0.1f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,                -0.5f, 0.0f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,                 -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
0.5f, 0.0f, 0.1f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,                 -0.5f, 0.0f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
0.5f, 0.0f, 0.2f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,                 -0.5f, 0.0f, 0.2f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
0.5f, 0.0f, 0.3f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,                 -0.5f, 0.0f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
0.5f, 0.0f, 0.4f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,                 -0.5f, 0.0f, 0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f,                 -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f
};
GLuint indices[] = {
0, 1, 3, 2,
4, 5, 7, 6,
8, 9, 11, 10,
12, 13, 15, 14,
16, 17, 19, 18,
20, 21, 23, 22,
24, 25, 27, 26,
28, 29, 31, 30,
32, 33, 35, 34,
36, 37, 39, 38,
40, 41, 43, 42
};
	this->setRenderType(GL_LINES);
    this->generateMesh(vertices, sizeof(vertices)/sizeof(GLfloat), indices, sizeof(indices)/sizeof(GLuint));
}

Primitive::Primitive(PRIM_MODEL model){
    switch(model){
        case PRIM_TETRAHEDRON:
            Tetrahedron();
			break;
        case PRIM_CUBE:
            Cube();
			break;
		case PRIM_GRID:
			Grid();
			break;
    }
}

//
// Transforms
//

mat4 Model::getFullTransformation()
{
	return _world_transform * _model_transform;
}

mat4 Model::getFullNormalTransformation()
{
	return _world_normal_transform * _model_normal_transform;
}

void Model::translate(GLfloat x_trans, GLfloat y_trans, GLfloat z_trans, bool isWorld)
{
	if (isWorld)
	{
		applyWorldTransformation(Translate(x_trans, y_trans, z_trans));
		applyWorldNormalTransformation(Translate(-x_trans, -y_trans, -z_trans)); // Normal Matrix = (M^-1)^T, we give it the inverse and the function will transform it
	}
	else
	{
		applyModelTransformation(Translate(x_trans, y_trans, z_trans));
		applyModelNormalTransformation(Translate(-x_trans, -y_trans, -z_trans));
	}
}

// TODO: Implement this function
void Model::rotate(GLfloat theta_angle, int mode, bool isWorld)
{
	if (isWorld)
	{
		applyWorldTransformation(RotateAxis(theta_angle, mode));
		applyWorldNormalTransformation(RotateAxis(-theta_angle, mode));
	}
	else
	{
		applyModelTransformation(RotateAxis(theta_angle, mode));
		applyModelNormalTransformation(RotateAxis(-theta_angle, mode));
	}
}

// TODO: Implement this function
void Model::scale(GLfloat x_scale, GLfloat y_scale, GLfloat z_scale, bool isWorld)
{
	if (isWorld)
	{
		applyWorldTransformation(Scale(x_scale, y_scale, z_scale));
		applyWorldNormalTransformation(Scale(1 / x_scale, 1 / y_scale, 1 / z_scale));
	}
	else
	{
		applyModelTransformation(Scale(x_scale, y_scale, z_scale));
		applyModelNormalTransformation(Scale(1 / x_scale, 1 / y_scale, 1 / z_scale));
	}
}

void Model::applyWorldTransformation(const mat4 &transformation)
{
	_world_transform = transformation * _world_transform;
}

void Model::applyModelTransformation(const mat4 &transformation)
{
	_model_transform = transformation * _model_transform;
}

void Model::applyWorldNormalTransformation(const mat4 &transformation_inv)
{
	_world_normal_transform = transpose(transformation_inv) * _world_normal_transform;
}
void Model::applyModelNormalTransformation(const mat4 &transformation_inv)
{
	_model_normal_transform = transpose(transformation_inv) * _model_normal_transform;
}
// Setters
void Model::setWorldTransformation(const mat4 &transformation)
{
	_world_transform = transformation;
}

void Model::setModelTransformation(const mat4 &transformation)
{
	_model_transform = transformation;
}

void Model::setWorldNormalTransformation(const mat4 &transformation_inv)
{
	_world_normal_transform = transpose(transformation_inv);
}
void Model::setModelNormalTransformation(const mat4 &transformation_inv)
{
	_model_normal_transform = transpose(transformation_inv);
}
//
//
//

void Model::setPosition(const glm::vec3& position){
	this->position = position;
	updateTransform();
}
void Model::setAngles(const glm::vec3& yaw_pitch_roll){
	this->yaw_pitch_roll = yaw_pitch_roll;
	updateTransform();
}
void Model::setScale(const glm::vec3& scale){
	this->size = scale;
	updateTransform();
}

void Model::updateTransform(){
	setWorldTransformation(Translate(position));
	setWorldNormalTransformation(glm::inverse(glm::lookAt(position,position + glm::vec3(1,0,0), glm::vec3(0,1,0))));


	setModelTransformation(Angles(yaw_pitch_roll) * Scale(size));
	setModelNormalTransformation(Scale(1.0f / size));
}