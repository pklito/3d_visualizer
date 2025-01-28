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

glm::vec3 vec3fFromStream(std::istream &aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 vec2fFromStream(std::istream &aStream)
{
	float x, y;
	aStream >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

