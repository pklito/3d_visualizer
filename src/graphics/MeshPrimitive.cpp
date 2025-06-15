
//		--------------------	//
// 			PRIMITIVES			//
//		--------------------	//
#include "Mesh.h"
using namespace glm;

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
	this->setRenderPipeline(PL_LINES);
    this->generateMesh(vertices, sizeof(vertices)/sizeof(GLfloat), indices, sizeof(indices)/sizeof(GLuint));
}

void Primitive::Circle(){
	const int nodes = 32;	//must be constant sadly, i dont wanna malloc
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
	this->setRenderPipeline(PL_LINE_LOOP);
	this->generateMesh(vertices, sizeof(vertices)/sizeof(GLfloat), indices, sizeof(indices)/sizeof(GLuint));
};


void Primitive::Cone(){
	const int nodes = 32;
	GLfloat vertices[8 * (nodes + 2)];
	int top_node = nodes + 1;
	int bottom_node = nodes;
	for(int i = 0; i < nodes; i++){
		float angle = 2 * glm::pi<float>() * i / nodes;
		vertices[8*i + 0] = 0.5 * glm::cos(angle);	//pos
		vertices[8*i + 1] = -0.5f;					
		vertices[8*i + 2] = 0.5 * glm::sin(angle);
		vertices[8*i + 3] = 0.8 * glm::cos(angle);  //normal
		vertices[8*i + 4] = -0.6f;
		vertices[8*i + 5] = 0.8 * glm::sin(angle);
		vertices[8*i + 6] = 0.5f * glm::cos(angle) + 0.5f;	//texture
		vertices[8*i + 7] = 0.5f * glm::sin(angle) + 0.5f;
	}
	//top node
	vertices[8*top_node + 0] = 0.f;	//pos
	vertices[8*top_node + 1] = 0.5f;					
	vertices[8*top_node + 2] = 0.f;
	vertices[8*top_node + 3] = 0.f;  //normal
	vertices[8*top_node + 4] = 1;
	vertices[8*top_node + 5] = 0;
	vertices[8*top_node + 6] = 0.5f;	//texture
	vertices[8*top_node + 7] = 0.5f;

	//bottom node
	vertices[8*bottom_node + 0] = 0.f;	//pos
	vertices[8*bottom_node + 1] = -0.5f;					
	vertices[8*bottom_node + 2] = 0.f;
	vertices[8*bottom_node + 3] = 0.f;  //normal
	vertices[8*bottom_node + 4] = -1;
	vertices[8*bottom_node + 5] = 0;
	vertices[8*bottom_node + 6] = 0.5f;	//texture
	vertices[8*bottom_node + 7] = 0.5f;
	GLuint indices[6 * nodes];
	for(int i = 0; i < nodes; i++){
		indices[6 * i] = i;
		indices[6 * i + 1] = (i + 1) % nodes;
		indices[6 * i + 2] = bottom_node;

		indices[6 * i + 3] = (i + 1) % nodes;
		indices[6 * i + 4] = i;
		indices[6 * i + 5] = top_node;
	}
	this->setRenderPipeline(PL_TRIANGLES);
	this->generateMesh(vertices, sizeof(vertices)/sizeof(GLfloat), indices, sizeof(indices)/sizeof(GLuint));
};

void Primitive::Cylinder(){
	const int nodes = 32;
	//32 nodes in a circle, two circles, plus top and bottom center
	GLfloat vertices[8 * (2 * nodes + 2)];
	int top_node = 2 * nodes + 1;
	int bottom_node = 2 * nodes;
	for(int i = 0; i < nodes; i++){
		//2i	bottom
		float angle = 2 * glm::pi<float>() * i / nodes;
		vertices[8*2*i + 0] = 0.5 * glm::cos(angle);	//pos
		vertices[8*2*i + 1] = -0.5f;					
		vertices[8*2*i + 2] = 0.5 * glm::sin(angle);
		vertices[8*2*i + 3] = 0.8 * glm::cos(angle);  //normal
		vertices[8*2*i + 4] = -0.6f;
		vertices[8*2*i + 5] = 0.8 * glm::sin(angle);
		vertices[8*2*i + 6] = 0.5f * glm::cos(angle) + 0.5f;	//texture
		vertices[8*2*i + 7] = 0.5f * glm::sin(angle) + 0.5f;
		// 2i + 1	top
		vertices[8*2*i + 8 + 0] = 0.5 * glm::cos(angle);	//pos
		vertices[8*2*i + 8 + 1] = 0.5f;					
		vertices[8*2*i + 8 + 2] = 0.5 * glm::sin(angle);
		vertices[8*2*i + 8 + 3] = 0.8 * glm::cos(angle);  //normal
		vertices[8*2*i + 8 + 4] = -0.6f;
		vertices[8*2*i + 8 + 5] = 0.8 * glm::sin(angle);
		vertices[8*2*i + 8 + 6] = 0.5f * glm::cos(angle) + 0.5f;	//texture
		vertices[8*2*i + 8 + 7] = 0.5f * glm::sin(angle) + 0.5f;
	}
	//top node
	vertices[8*top_node + 0] = 0.f;	//pos
	vertices[8*top_node + 1] = 0.5f;					
	vertices[8*top_node + 2] = 0.f;
	vertices[8*top_node + 3] = 0.f;  //normal
	vertices[8*top_node + 4] = 1;
	vertices[8*top_node + 5] = 0;
	vertices[8*top_node + 6] = 0.5f;	//texture
	vertices[8*top_node + 7] = 0.5f;

	//bottom node
	vertices[8*bottom_node + 0] = 0.f;	//pos
	vertices[8*bottom_node + 1] = -0.5f;					
	vertices[8*bottom_node + 2] = 0.f;
	vertices[8*bottom_node + 3] = 0.f;  //normal
	vertices[8*bottom_node + 4] = -1;
	vertices[8*bottom_node + 5] = 0;
	vertices[8*bottom_node + 6] = 0.5f;	//texture
	vertices[8*bottom_node + 7] = 0.5f;
	GLuint indices[12 * nodes];
	for(int i = 0; i < nodes; i++){
		int bl = 2 * i;
		int ul = 2 * i + 1;
		int br = (2 * i + 2) % (2 * nodes);
		int ur = (2 * i + 3) % (2 * nodes);
		indices[12 * i] = bl;
		indices[12 * i + 1] = ul;
		indices[12 * i + 2] = br;

		indices[12 * i + 3] = ul;
		indices[12 * i + 4] = ur;
		indices[12 * i + 5] = br;

		indices[12 * i + 6] = ur;
		indices[12 * i + 7] = ul;
		indices[12 * i + 8] = top_node;

		indices[12 * i + 9] = bl;
		indices[12 * i + 10] = br;
		indices[12 * i + 11] = bottom_node;
	}
	this->setRenderPipeline(PL_TRIANGLES);
	this->generateMesh(vertices, sizeof(vertices)/sizeof(GLfloat), indices, sizeof(indices)/sizeof(GLuint));
};

void Primitive::Cube(){
	GLfloat vertices[8 * 24];
	vec3 faces[] = {vec3(1,0,0), vec3(-1,0,0), vec3(0,1,0), vec3(0,-1,0), vec3(0,0,1), vec3(0,0,-1)};
	vec3 forward[] = {vec3(0,-1,0), vec3(0,1,0), vec3(1,0,0), vec3(-1,0,0), vec3(-1,0,0), vec3(1,0,0)};
	vec3 side[] = {vec3(0,0,1), vec3(0,0,1), vec3(0,0,1), vec3(0,0,1), vec3(0,1,0), vec3(0,1,0)};
	for(int i = 0; i < 6; i ++ ){
		//pos
		for(int j = 0; j < 4; j ++ ){
			float lr = j % 2 ? 1 : -1;
			float ud = j / 2 ? 1 : -1;
			vertices[8 * (4 * i + j) + 0] = 0.5f * (faces[i] + lr * forward[i] + ud * side[i])[0];
			vertices[8 * (4 * i + j) + 1] = 0.5f * (faces[i] + lr * forward[i] + ud * side[i])[1];
			vertices[8 * (4 * i + j) + 2] = 0.5f * (faces[i] + lr * forward[i] + ud * side[i])[2];

			vertices[8 * (4 * i + j) + 3] = faces[i][0];
			vertices[8 * (4 * i + j) + 4] = faces[i][1];
			vertices[8 * (4 * i + j) + 5] = faces[i][2];
			//todo improve
			vertices[8 * (4 * i + j) + 6] = i/8;
			vertices[8 * (4 * i + j) + 7] = i%2;
		}
	}
	GLuint indices[3*12];
	for(int i = 0; i < 6; i ++){
		indices[6*i] = 4*i;
		indices[6*i + 1] = 4*i + 2;
		indices[6*i + 2] = 4*i + 1;
		indices[6*i + 3] = 4*i + 2;
		indices[6*i + 4] = 4*i + 3;
		indices[6*i + 5] = 4*i + 1;
	}

	this->setRenderPipeline(PL_TRIANGLES);
	this->generateMesh(vertices, sizeof(vertices)/sizeof(GLfloat), indices, sizeof(indices)/sizeof(GLuint));

}

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
			Cone();
			break;
		case PRIM_CYLINDER:
			Cylinder();
			break;
		case PRIM_CUBE:
			Cube();
			break;
		case PRIM_SPHERE:
		default:
            FromFile(PRIM_MODEL_NAMES.at(model) + ".obj");
			break;
    }
}
