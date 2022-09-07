#pragma once
//  Vertex.h
//  workspace 2.0
//
//  Created by Frank Collebrusco on 5/21/22.
//
#define GLM_SWIZZLE
#include <glm/vec2.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp>
#include <string>

//#include "Shader.h"



struct Vertex {
    Vertex(){
        Pos = glm::vec3(0.0f);
        UV = glm::vec3(0.0f);
    }
    Vertex(glm::vec3 p){
        Pos = p;
        UV = glm::vec3(0.0f);
    }
    Vertex(glm::vec3 p, glm::vec2 uv){
        Pos = p;
        UV = uv;
    }

    glm::vec3 Pos;
    glm::vec2 UV;
};

struct MeshDetails {
    MeshDetails(){
        
    }
    MeshDetails(uint32_t v, uint32_t e){
        vao = v;
        numElements = e;
        type = "triangles";
    }
    MeshDetails(uint32_t v, uint32_t e, std::string t){
        vao = v;
        numElements = e;
        type = t;
    }
    
    uint32_t vao = 0;
    uint32_t numElements = 0;
    std::string type = "triangles";
};

//struct ObjectDetails {
//    ObjectDetails(VAODetails dd, Transform trans, Shader* s){
//        vaoDetails = dd;
//        shad = s;
//        genModelMat(trans, &modelMat);
//    }
//    VAODetails vaoDetails = VAODetails(0, 0);
//    mat4 modelMat;
//    Shader *shad;
//};

enum OBJ_TYPE {
    CUBE,
    SQUARE
};
enum AXIS {
    X, Y, Z
};

