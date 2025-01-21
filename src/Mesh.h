#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include<string>
class Mesh {
protected:
    Mesh();
    VAO vao;
    VBO vbo;
    Texture texture;


public:
    Mesh(std::string filename, std::string );
    void draw();
    void destroy();
};

typedef enum
{
	PRIM_CUBE,
	PRIM_TETRAHEDRON
} PRIM_MODEL;

class Primitive : public Mesh {
public:
    Primitive(PRIM_MODEL model);
};