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