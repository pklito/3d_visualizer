#include"VAO.h"
#include"Texture.h"
#include<string>

class VertexData{
    public:
    GLfloat* data; GLsizeiptr data_length; GLint data_amount; GLuint* indices; GLsizeiptr indices_length;
    VertexData() : data(nullptr), data_length(0), data_amount(0), indices(nullptr), indices_length(0){}
    VertexData(GLfloat* vertices, GLsizeiptr vertexcount, GLint data_amount, GLuint* vertex_indices, GLsizeiptr indices_count)
    : data(vertices), data_length(vertexcount), data_amount(data_amount), indices(vertex_indices), indices_length(indices_count) {}
};

class Model {
protected:
    VAO vao;
    VBO vbo;
    EBO ebo;
    Texture* texture;
    void generateMesh(VertexData& location, VertexData& normals, VertexData& textures);

public:
    Model();
    Model(std::string filedir, std::string texturedir);
    void setModel(std::string filedir);
    void setModel(int* vertexArray, int length);
    void loadFile(std::string file);

    void setTexture(std::string texturedir);
    void setShader(std::string vertexdir, std::string fragmentdir);
    void render();
    void destroy();
};

typedef enum
{
	PRIM_CUBE,
	PRIM_TETRAHEDRON
} PRIM_MODEL;

class Primitive : public Model {
public:
    Primitive(PRIM_MODEL model);
};