#pragma once
#include<glm/glm.hpp>
#include<GLFW/glfw3.h>

inline void toFloatArray(GLfloat* array, const glm::mat4& mat){
    const int size = 4;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            array[j+size*i] = mat[j][i];
        }
    }
}

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
inline
glm::vec3 vec3fFromStream(std::istream &aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}
inline
glm::vec2 vec2fFromStream(std::istream &aStream)
{
	float x, y;
	aStream >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

//
// Matrices
//


//----------------------------------------------------------------------------
//
//  Rotation matrix generators
//
inline
float Radians(float degrees) 
{
	return glm::radians(degrees);
}

inline
glm::mat4 RotateX(const GLfloat theta)
{
    //GLfloat angle = (M_PI / 180.0) * theta;
    GLfloat angle = Radians(theta);

    glm::mat4 c(1.0f);
    c[2][2] = c[1][1] = cos(angle);
    c[2][1] = sin(angle);
    c[1][2] = -c[2][1];
    return glm::transpose(c);
}

inline
glm::mat4 RotateY(GLfloat theta_angle) {
	glm::mat4 rotation_matrix(1.0f);
    theta_angle = Radians(theta_angle);
	rotation_matrix[0].x = cos(theta_angle);
	rotation_matrix[0].z = sin(theta_angle);
	rotation_matrix[2].x = -sin(theta_angle);
	rotation_matrix[2].z = cos(theta_angle);
	/*((cos(theta_angle), 0, sin(theta_angle), 0),
		(0, 1, 0, 0),
		(-sin(theta_angle), 0, cos(theta_angle), 0),
		(0, 0, 0, 1));*/
	return glm::transpose(rotation_matrix);
}

inline
glm::mat4 RotateZ(GLfloat theta_angle) {
	glm::mat4 rotation_matrix(1.0f);
    theta_angle = Radians(theta_angle);
	rotation_matrix[0].x = cos(theta_angle);
	rotation_matrix[0].y = -sin(theta_angle);
	rotation_matrix[1].x = sin(theta_angle);
	rotation_matrix[1].y = cos(theta_angle);
	/*((cos(theta_angle), -sin(theta_angle), 0, 0),
		(sin(theta_angle), cos(theta_angle), 0, 0),
		(0, 0, 1, 0),
		(0, 0, 0, 1));*/
	return glm::transpose(rotation_matrix);
}

inline glm::mat4 RotateAxis(GLfloat theta_angle, int axis){
	return glm::transpose((axis == 0 ? RotateX(theta_angle) : (axis == 1 ? RotateY(theta_angle) : RotateZ(theta_angle))));
}
//----------------------------------------------------------------------------
//
//  Translation matrix generators
//

inline
glm::mat4 Translate(const GLfloat x, const GLfloat y, const GLfloat z)
{
    glm::mat4 c(1.0f);
    c[0][3] += x;
    c[1][3] += y;  /*there was a BUG here*/
    c[2][3] += z;
    return glm::transpose(c);
}

inline
glm::mat4 Translate(const glm::vec3& v)
{
    return glm::transpose(Translate(v.x, v.y, v.z));
}

inline
glm::mat4 Translate(const glm::vec4& v)
{
    return glm::transpose(Translate(v.x, v.y, v.z));
}

//----------------------------------------------------------------------------
//
//  Scale matrix generators
//

inline
glm::mat4 Scale(const GLfloat x, const GLfloat y, const GLfloat z)
{
    glm::mat4 c(1.0f);
    c[0] *= x;
    c[1] *= y; /*there was a BUG*/
    c[2] *= z;
    return glm::transpose(c);
}

inline
glm::mat4 Scale(const glm::vec3& v)
{
    return glm::transpose(Scale(v.x, v.y, v.z));
}