#pragma once
#include"graphics/VAO.h"
#include"graphics/Texture.h"

#include<string>
#include"graphics/shaderClass.h"
#include<glm/glm.hpp>
#include"graphics/Renderer.h"
#include<vector>
#include"graphics/GUIfuncs.h"

class VertexData{
    public:
    GLfloat* data; GLsizeiptr data_length; GLint data_amount; GLuint* indices; GLsizeiptr indices_length;
    VertexData() : data(nullptr), data_length(0), data_amount(0), indices(nullptr), indices_length(0){}
    VertexData(GLfloat* vertices, GLsizeiptr vertexcount, GLint data_amount, GLuint* vertex_indices, GLsizeiptr indices_count)
    : data(vertices), data_length(vertexcount), data_amount(data_amount), indices(vertex_indices), indices_length(indices_count) {}
};
#define RenderFunc(name) void (Renderer::*name)(GLuint render_mode, GLsizeiptr indices_count, const VAO* const vao, const Texture* const texture, const glm::vec4& color, const glm::mat4& model_transform, const glm::mat4& normal_transform)

//I've decided to put a wrapper over GL_[RENDER] types because i want to pass relevant choices to my renderer without adding infinitely many and conflicting parameters
typedef enum {
    //pass to models to have them use their own pipeline.
    PL_OVERRIDDEN = -2,
    PL_NONE = -1,
    PL_MESH = GL_TRIANGLES,
    PL_LINES = GL_LINES,
    PL_LINE_STRIP = GL_LINE_STRIP,
    PL_LINE_LOOP = GL_LINE_LOOP,
    PL_MESH_SHADELESS = GL_TRIANGLES | 1 << 4,
    PL_MESH_HIGHLIGHT = GL_TRIANGLES | 2 << 4,
    PL_MESH_OVERLAY = GL_TRIANGLES | 3 << 4
} RenderPipeline;

inline GLuint PL_TO_GL(RenderPipeline pl){
    return pl & 0b111;
}

class Model {
protected:
    Texture texture;
    // Shader shader;

	glm::mat4 _world_transform;
	glm::mat4 _model_transform;
	glm::mat4 _world_normal_transform;
	glm::mat4 _model_normal_transform;

    glm::vec3 position = glm::vec3(0, 0, 0);
    // pitch: -pi/2 to pi/2, yaw: -pi to pi, roll: -pi to pi
    // yaw = 0, facing +z
    glm::vec3 yaw_pitch_roll = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 size= glm::vec3(1,1,1);

    glm::vec4 color = glm::vec4(1.,1.,1.,1.);

    RenderPipeline render_pipeline = PL_MESH;

    std::string name;

    void applyWorldTransformation(const glm::mat4 &transformation);
	void applyModelTransformation(const glm::mat4 &transformation);
	void applyWorldNormalTransformation(const glm::mat4 &transformation_inv);
	void applyModelNormalTransformation(const glm::mat4 &transformation_inv);

public:
    void setWorldTransformation(const glm::mat4 &transformation);
	void setModelTransformation(const glm::mat4 &transformation);
	void setWorldNormalTransformation(const glm::mat4 &transformation_inv);
	void setModelNormalTransformation(const glm::mat4 &transformation_inv);

protected:
    void translate(GLfloat x_trans, GLfloat y_trans, GLfloat z_trans, bool isWorld = false);
	void rotate(GLfloat theta_angle, int mode, bool isWorld = false);
	void scale(GLfloat x_scale, GLfloat y_scale, GLfloat z_scale, bool isWorld = false);

    void updateTransform();

    LockFloat3State _gui_scale_state;
    bool _gui_use_degrees = false;

public:
    Model();

    Model& setRenderPipeline(RenderPipeline pipeline) {render_pipeline = pipeline; return *this;};
    Model& setTexture(const std::string& texturedir);
    Model& setTexture(const Texture& texture) {this->texture = texture;};
    bool hasTexture() const {return texture.exists();};
    
    // virtual void render(Renderer& renderer, RenderFunc(renderer_func) = Renderer::renderModel) = 0;
    //applies transforms on the render, changes the set render_mode
    virtual void doRenderPipeline(Renderer& renderer, RenderPipeline renderformat = PL_OVERRIDDEN ,const glm::mat4& model_transform = glm::mat4(1), const glm::mat4& normal_transform = glm::mat4(1)) const;
    virtual void _render(Renderer& renderer, RenderFunc(render_func) = &Renderer::renderModel,
                        const glm::mat4& model_transform = glm::mat4(1), const glm::mat4& normal_transform = glm::mat4(1), GLuint render_mode = -1) const = 0;

    virtual void destroy();

    glm::mat4 getFullTransformation() const;
    glm::mat4 getFullNormalTransformation() const;

    glm::vec4 getColor() const {return color;};
    Model& setColor(const glm::vec4& color) {this->color = color; return *this;};

    Model& setPosition(const glm::vec3& position);
    Model& setAngles(const glm::vec3& orientation);
    Model& setAnglesDegrees(const glm::vec3& yaw_pitch_roll);
    Model& setScale(const glm::vec3& scale);
    glm::vec3 getPosition() const {return position;};
    glm::vec3 getAngles() const {return yaw_pitch_roll;};
    glm::vec3 getScale() const {return size;};
    Model& setName(const std::string& name) {this->name = name; return *this;};
    std::string getName() const {return name;};

    virtual Model* copy() const = 0;

    virtual void buildGUI();
};

class ObjModel : public Model{
    protected:
        VAO vao;
        VBO vbo;
        EBO ebo;
        // Shader shader;
        GLsizeiptr indices_count;
        void generateMesh(GLfloat* vertices_data, GLsizeiptr vertices_count, GLuint* indices, GLsizeiptr indices_count);
        
    public:
    ObjModel& setModel(const std::string& modeldir);
    ObjModel& loadFile(const std::string& file);

    ObjModel();
    ObjModel(const std::string& filedir);
    ObjModel(const std::string& filedir, const std::string& texturedir);

    // virtual void render(Renderer& renderer, RenderFunc(renderer_func) = Renderer::renderModel) override;
    //applies transforms on the render, changes the set render_mode
    //virtual void doRenderPipeline(Renderer& renderer, RenderPipeline renderformat = PL_TRIANGLES ,const glm::mat4& model_transform = glm::mat4(1), const glm::mat4& normal_transform = glm::mat4(1)) override;
    virtual void _render(Renderer& renderer, RenderFunc(renderer_func) = &Renderer::renderModel, const glm::mat4& model_transform = glm::mat4(1), const glm::mat4& normal_transform = glm::mat4(1), GLuint render_mode = -1) const override;

    
    virtual void buildGUI() override;

    virtual void destroy() override;

    virtual Model* copy() const override {return new ObjModel(*this);};


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
    PRIM_SPHERE,
    PRIM_CIRCLE
} PRIM_MODEL;

#include <map>

const std::map<PRIM_MODEL, std::string> PRIM_MODEL_NAMES = {
    {PRIM_CUBE, "cube"},
    {PRIM_TETRAHEDRON, "tetrahedron"},
    {PRIM_GRID, "grid"},
    {PRIM_CONE, "cone"},
    {PRIM_CYLINDER, "cylinder"},
    {PRIM_SPHERE, "sphere"},
    {PRIM_CIRCLE, "circle"}
};

class Primitive : public ObjModel {
protected:
    void FromFile(const std::string file);
    void Tetrahedron();
    void Grid();
    void Circle();
    void Cone();
	void Sphere();
	void Cylinder();
	void Cube();
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
    GroupModel(const GroupModel& _that);

    virtual GroupModel& addModel(Model* model);
    virtual GroupModel& addCopy(const Model* const model);
    // virtual void render(Renderer& renderer, RenderFunc(render_func) = Renderer::renderModel) override;
    virtual void doRenderPipeline(Renderer& renderer, RenderPipeline renderformat = PL_OVERRIDDEN ,const glm::mat4& model_transform = glm::mat4(1), const glm::mat4& normal_transform = glm::mat4(1)) const override;
    virtual void _render(Renderer& renderer, RenderFunc(renderer_func) = &Renderer::renderModel, const glm::mat4& model_transform = glm::mat4(1), const glm::mat4& normal_transform = glm::mat4(1), GLuint render_mode = -1) const override;
    //virtual void buildGUI() override;

    virtual void destroy() override;

    virtual Model* copy() const override {return new GroupModel(*this);};

    virtual void buildChildrenDropdownGUI();

    virtual std::vector<Model*>::iterator begin() {return models.begin();};
    virtual std::vector<Model*>::iterator end() {return models.end();};
};

GroupModel* demoFoxHat();

class EmptyModel : public Model {
public:
    EmptyModel() {};
    void _render(Renderer& renderer, RenderFunc(render_func) = &Renderer::renderModel,
                        const glm::mat4& model_transform = glm::mat4(1), const glm::mat4& normal_transform = glm::mat4(1), GLuint render_mode = -1) const override {};
    virtual Model* copy() const override {return new EmptyModel();};
};