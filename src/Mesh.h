#include"VAO.h"
#include"Texture.h"
#include<string>
#include"ShaderClass.h"

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
    Texture texture;
    // Shader shader;
    GLsizeiptr indices_count;
    void generateMesh(GLfloat* vertices_data, GLsizeiptr vertices_count, GLuint* indices, GLsizeiptr indices_count);

public:
    Model();
    Model(const std::string& filedir, const std::string& texturedir);
    void setModel(std::string& filedir);
    void setModel(int* vertexArray, int length);
    void loadFile(const std::string& file);

    void setTexture(const std::string& texturedir);
    void setShader(const std::string& vertexdir, const std::string& fragmentdir);
    void render();
    void destroy();
};

typedef enum
{
	PRIM_CUBE,
	PRIM_TETRAHEDRON
} PRIM_MODEL;

class Primitive : public Model {
protected:
    void Cube();
    void Tetrahedron();
public:
    Primitive(PRIM_MODEL model);
};