//
//  GL_Draw.cpp
//  gaspunk
//
//  Created by Frank Collebrusco on 5/29/22.
//

#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include "GL_Draw.h"

void DrawObjects(const vector<MeshDetails*> &objs){
    for (const auto& d : objs){
        glBindVertexArray(d->vao);
//        cout << d->vao << endl;
        //TODO ERROR CHECK
        glDrawElements((d->type == "triangles") ? GL_TRIANGLES : GL_LINES,
                       d->numElements,
                       GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}

void DrawMesh(MeshDetails* mesh){
    
    glBindVertexArray(mesh->vao);
//    cout << mesh->vao << endl;
    //TODO ERROR CHECK
    glDrawElements((mesh->type == "triangles") ? GL_TRIANGLES : GL_LINES,
                    mesh->numElements,
                    GL_UNSIGNED_INT, nullptr);
//    std::cout << glGetError() << std::endl;
    

    
    glBindVertexArray(0);
}
