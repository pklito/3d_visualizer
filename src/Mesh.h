#pragma once
#include"VAO.h"
#include"Texture.h"
#include<string>
#include"ShaderClass.h"
#include<glm/glm.hpp>
#include"Renderer.h"
#include<vector>

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

	glm::mat4 _world_transform;
	glm::mat4 _model_transform;
	glm::mat4 _world_normal_transform;
	glm::mat4 _model_normal_transform;

    glm::vec3 position = glm::vec3(0, 0, 0);
    // pitch: -pi/2 to pi/2, yaw: -pi to pi, roll: -pi to pi
    // yaw = 0, facing +z
    glm::vec3 yaw_pitch_roll = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 size= glm::vec3(1,1,1);

    GLuint render_type = GL_TRIANGLES;

    void applyWorldTransformation(const glm::mat4 &transformation);
	void applyModelTransformation(const glm::mat4 &transformation);
	void applyWorldNormalTransformation(const glm::mat4 &transformation_inv);
	void applyModelNormalTransformation(const glm::mat4 &transformation_inv);

    void setWorldTransformation(const glm::mat4 &transformation);
	void setModelTransformation(const glm::mat4 &transformation);
	void setWorldNormalTransformation(const glm::mat4 &transformation_inv);
	void setModelNormalTransformation(const glm::mat4 &transformation_inv);

    void translate(GLfloat x_trans, GLfloat y_trans, GLfloat z_trans, bool isWorld = false);
	void rotate(GLfloat theta_angle, int mode, bool isWorld = false);
	void scale(GLfloat x_scale, GLfloat y_scale, GLfloat z_scale, bool isWorld = false);

    void updateTransform();

public:
    Model();
    Model(const std::string& filedir, const std::string& texturedir) : Model() {
        loadFile(filedir);
        setTexture(texturedir);
    }
    void setModel(const std::string& modeldir);
    void setModel(int* vertexArray, int length);
    void loadFile(const std::string& file);
    void setRenderType(GLuint new_type) {render_type = new_type;};
    void setTexture(const std::string& texturedir);
    bool hasTexture() {return texture.exists();};
    
    virtual void render(Renderer& renderer);
    virtual void render(Renderer& renderer, const glm::mat4& model_transform, const glm::mat4& normal_transform);

    void destroy();

    glm::mat4 getFullTransformation();
    glm::mat4 getFullNormalTransformation();

    void setPosition(const glm::vec3& position);
    void setAngles(const glm::vec3& orientation);
    void setAnglesDegrees(const glm::vec3& yaw_pitch_roll);
    void setScale(const glm::vec3& scale);
    glm::vec3 getPosition() {return position;};
    glm::vec3 getAngles() {return yaw_pitch_roll;};
    glm::vec3 getScale() {return size;};

    friend class GUI;
};

// --------------------
//      PRIMITIVE
// --------------------

typedef enum
{
	PRIM_CUBE,
	PRIM_TETRAHEDRON,
    PRIM_GRID,
    PRIM_CONE,
    PRIM_CYLINDER,
    PRIM_SPHERE
} PRIM_MODEL;

#include <map>

const std::map<PRIM_MODEL, std::string> PRIM_MODEL_NAMES = {
    {PRIM_CUBE, "cube"},
    {PRIM_TETRAHEDRON, "tetrahedron"},
    {PRIM_GRID, "grid"},
    {PRIM_CONE, "cone"},
    {PRIM_CYLINDER, "cylinder"},
    {PRIM_SPHERE, "sphere"}
};

class Primitive : public Model {
protected:
    void FromFile(const std::string file);
    void Tetrahedron();
    void Grid();
public:
    Primitive(PRIM_MODEL model);
};

// --------------------
//      MODEL GROUP
// --------------------

class GroupModel : public Model {
protected:
std::vector<Model*> models;

public:
    GroupModel();
    GroupModel(const std::vector<Model*>& models);

    void addModel(Model* model);
    void addCopy(const Model* const model);
    void render(Renderer& renderer) override;
};