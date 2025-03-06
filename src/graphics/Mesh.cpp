#include "graphics/Mesh.h"
#include<glm/glm.hpp>
#include"util.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

using namespace glm;
Model::Model() : _world_transform(1.0f), _model_transform(1.0f), _world_normal_transform(1.0f), _model_normal_transform(1.0f), size(1.0f,1.0f,1.0f), name("model"){
}

std::string extractFileName(const std::string& filedir) {
	int start = filedir.find_last_of("/\\") + 1;
	int end = filedir.substr(start).find_first_of(".");
	if (end == 0) {
		end = filedir.length();
	}
	return filedir.substr(start, end);
}

ObjModel::ObjModel() : Model() {name = "objModel";};
ObjModel::ObjModel(const std::string& filedir) : Model() {
	setModel(filedir);
	name = extractFileName(filedir);

};
ObjModel::ObjModel(const std::string& filedir, const std::string& texturedir) : Model() {
	loadFile(filedir);
	setTexture(texturedir);
	name = extractFileName(filedir);
};


void ObjModel::generateMesh(GLfloat* vertices_data, GLsizeiptr vertices_count, GLuint* indices, GLsizeiptr indices_c){
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
void ObjModel::setModel(const std::string& modeldir){
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

void ObjModel::render(Renderer& renderer, RenderFunc(renderer_func), const glm::mat4& model_transform, const glm::mat4& normal_transform, GLuint render_mode){
	if(render_mode == -1){
		render_mode = this->render_type;
	}
	//Call renderer with the chosen render call (usually renderer.renderModel)
	//Overrides the transformations.
	(renderer.*renderer_func)(render_mode, this->indices_count, &this->vao, &this->texture, this->color, model_transform * getFullTransformation(), normal_transform * getFullNormalTransformation());
}

void Model::destroy(){
	texture.destroy();
	//shader.destroy();
}

void ObjModel::destroy(){
	vao.destroy();
	vbo.destroy();
	ebo.destroy();
	texture.destroy();
	//shader.destroy();
}

void GroupModel::destroy(){
	for(Model* model : models){
		model->destroy();
	}
}

/// -----------------
//
//
//  FILE MESH GEN
//
//
/// ----------------

void ObjModel::loadFile(const std::string& file){
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
				data_array[face_size*8*face_index + 8*i + 3] = vertex_normals[it->vn[i] - 1][0];
				data_array[face_size*8*face_index + 8*i + 4] = vertex_normals[it->vn[i] - 1][1];
				data_array[face_size*8*face_index + 8*i + 5] = vertex_normals[it->vn[i] - 1][2];
			}
			if(vertex_textures.size() > 0){
				data_array[face_size*8*face_index + 8*i + 6] = vertex_textures[it->vt[i] - 1][0];
				data_array[face_size*8*face_index + 8*i + 7] = vertex_textures[it->vt[i] - 1][1];
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

void Primitive::FromFile(const std::string file){
	this->loadFile("resources/primitives/" + file);
}

void Primitive::Tetrahedron(){
        GLfloat vertices[] =
    { //     COORDINATES     /        NORMALS      /   TexCoord  //
        -0.5f, -0.5f,  0.5f,     -1.f, 0.0f,  0.5f,	0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,     -1.f, 0.0f, -0.5f,	5.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     0.5f, 0.0f, -1.f,	0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,     0.5f, 0.0f, 1.f,	5.0f, 0.0f,
        0.0f, 0.5f,  0.0f,     0.f, 1.f, 0.f,	2.5f, 5.0f
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

void Primitive::Circle(){
const int nodes = 100;
GLfloat vertices[8*nodes];
	for(int i = 0; i < nodes; i++){
		float angle = 2 * glm::pi<float>() * i / nodes;
		vertices[8*i + 0] = 0.5 * glm::cos(angle);
		vertices[8*i + 1] = 0.0f;
		vertices[8*i + 2] = 0.5 * glm::sin(angle);
		vertices[8*i + 3] = 0.0f;
		vertices[8*i + 4] = 1.0f;
		vertices[8*i + 5] = 0.0f;
		vertices[8*i + 6] = 0.5f * glm::cos(angle) + 0.5f;
		vertices[8*i + 7] = 0.5f * glm::sin(angle) + 0.5f;
	}
	GLuint indices[nodes];
	for(int i = 0; i < nodes; i++){
		indices[i] = i;
	}
	this->setRenderType(GL_LINE_LOOP);
	this->generateMesh(vertices, sizeof(vertices)/sizeof(GLfloat), indices, sizeof(indices)/sizeof(GLuint));
};

Primitive::Primitive(PRIM_MODEL model){
	name = PRIM_MODEL_NAMES.at(model);
    switch(model){
        case PRIM_TETRAHEDRON:
            Tetrahedron();
			break;
		case PRIM_GRID:
			Grid();
			break;
		case PRIM_CIRCLE:
			Circle();
			break;
		case PRIM_CONE:
		case PRIM_SPHERE:
		case PRIM_CYLINDER:
		case PRIM_CUBE:
            FromFile(PRIM_MODEL_NAMES.at(model) + ".obj");
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

/**
 * @brief Sets the rotation angles of the model.
 * 
 * This function sets the yaw, pitch, and roll angles of the model and updates the model's transformation matrix accordingly.
 * 
 * @param yaw_pitch_roll glm::vec3 in radians, contains yaw, pitch, roll.
 */
void Model::setAngles(const glm::vec3& yaw_pitch_roll){
	this->yaw_pitch_roll = yaw_pitch_roll;
	updateTransform();
}

void Model::setAnglesDegrees(const glm::vec3& yaw_pitch_roll){
	setAngles(glm::vec3(glm::pi<float>()/180.) * yaw_pitch_roll);
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

// --------------------
//  GroupModel
// --------------------

GroupModel::GroupModel() : models(){

}

GroupModel::GroupModel(const std::vector<Model*>& models){
	this->models = models; //? works?
}

GroupModel::GroupModel(const GroupModel& _that){
	for(Model* model : _that.models){
		addCopy(model);
	}
}
void GroupModel::addModel(Model* model){
	models.push_back(model);
}

void GroupModel::addCopy(const Model* const model){
	Model* new_model = model->copy();
	models.push_back(new_model);
}

void GroupModel::render(Renderer& renderer, RenderFunc(renderer_func), const glm::mat4& model_transform, const glm::mat4& normal_transform, GLuint render_mode){
	if(render_mode == -1){
		render_mode = this->render_type;
	}
	for(Model* model : models){
		model->render(renderer, renderer_func, model_transform * getFullTransformation(), normal_transform * getFullNormalTransformation(), render_mode);
	}
}

GroupModel* demoFoxHat(){
	Model* mesh = new ObjModel("resources\\fox.obj", "resources\\UVMap.png");
    mesh->setRenderType(GL_TRIANGLES);
    mesh->setScale(glm::vec3(1.,0.5,1.));
    mesh->setPosition(glm::vec3(0.0,0,0));

    Model* mesh_2 = new Primitive(PRIM_CONE);
    mesh_2->setPosition(glm::vec3(0,1.2,0));
    mesh_2->setScale(glm::vec3(0.3));
    mesh_2->setAnglesDegrees(glm::vec3(0,-30,0));

    GroupModel* group = new GroupModel();
    group->addModel(mesh_2);
    group->addModel(mesh);
	return group;
}



// - - - -
//  GUI
// - - - - 

void Model::buildGUI(){
	//Update position without friend class

	if(ImGui::DragFloat3("Position", glm::value_ptr(position), 0.02f, -100.0f, 100.0f)){
		updateTransform();
	}
	if(_gui_use_degrees){
		glm::vec3 yaw_pitch_roll_deg = yaw_pitch_roll * glm::vec3(180.0f/glm::pi<float>());
		if(ImGui::DragFloat3("Yaw Pitch Roll", glm::value_ptr(yaw_pitch_roll_deg), 0.1f, -180.0f, 180.0f)){
			yaw_pitch_roll = yaw_pitch_roll_deg * glm::vec3(glm::pi<float>()/180.0f);
			updateTransform();
		}
	}
	else{
	if(ImGui::DragFloat3("Yaw Pitch Roll", glm::value_ptr(yaw_pitch_roll), 0.01f, -glm::pi<float>(), glm::pi<float>())){
		updateTransform();
	}
	}
	// Pop into next line if no room
	if(ImGui::GetContentRegionAvail().x > 330)
		ImGui::SameLine();
	ImGui::PushID("use_degrees");
	ImGui::SetNextItemWidth(50);
	ImGui::SliderInt("", (int*)&_gui_use_degrees, 0, 1, _gui_use_degrees ? "   Deg" : "Rad   ");
	ImGui::PopID();
	glm::vec3 scale = getScale();
	bool a = false;
	if (DragFloat3Lock("Scale", scale, _gui_scale_state)){
		setScale(scale);
	}
}

void ObjModel::buildGUI(){
	Model::buildGUI();
	ImGui::Separator();

	if (ImGui::Button("Set mesh")){
		setModel(popupExplorer(".obj"));
	}

	ImGui::SameLine();
	if (ImGui::Button("Set texture")){
		setTexture(popupExplorer(".jpg"));
	}
	//ImGui::SameLine();
	if (ImGui::ColorEdit4("Color", glm::value_ptr(color))){
	}
	ImGui::Separator();

	int _type = render_type;
	ImGui::RadioButton("TRIANGLES", &_type, GL_TRIANGLES); ImGui::SameLine();
	ImGui::RadioButton("LINES", &_type, GL_LINES); ImGui::SameLine();
	ImGui::RadioButton("LINE_STRIP", &_type, GL_LINE_STRIP); ImGui::SameLine();
	ImGui::RadioButton("LINES_LOOP", &_type, GL_LINE_LOOP);
	render_type = _type;
}