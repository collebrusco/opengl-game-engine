
//  GL_Loader.cpp
//  gaspunk
//
//  Created by Frank Collebrusco on 5/30/22.
//


//  OpenGLLoader.h
//  GL Open
//
//  Created by Frank Collebrusco on 5/19/22.
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
//#include "Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "GL_Loader.h"
using namespace std;
using namespace glm;

static vector<int> textureSlotsInUse;
int UploadTexture(string name){
    bool pixelated = true;
    int w, h, c;
    uint32_t textureId;
    string path = "assets/" + name + ".png";
    //GET PIXELS
    uint8_t* pixels = stbi_load(path.c_str(), &w, &h, &c, 0);
    //PARAMS
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
    //UPLOAD
    uint32_t mipLevel = 0;
    uint32_t internalFormat;
    uint32_t format;
    switch(c){
        case 1:
            internalFormat = GL_R32F;
            format = GL_RED;
            break;
        case 2:
            internalFormat = GL_RG32F;
            format = GL_RG;
            break;
        case 3:
            internalFormat = GL_RGB32F;
            format = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA32F;
            format = GL_RGBA;
            break;
        default:
            internalFormat = GL_RGBA32F;
            format = GL_RGBA;
            break;
    }
    
    glTexImage2D(GL_TEXTURE_2D, mipLevel, internalFormat,
                 w, h, 0, format, GL_UNSIGNED_BYTE, pixels);
    
    stbi_image_free(pixels);
    int texSlot = textureSlotsInUse.size();
    textureSlotsInUse.push_back(texSlot);
    glActiveTexture(GL_TEXTURE0 + texSlot);
    glBindTexture(GL_TEXTURE_2D, textureId);
    return texSlot;
}


 MeshDetails UploadMesh(const vector<Vertex>& verts, const vector<uint32_t> elem,
                             string type){
    if (verts.empty() | elem.empty()){
        throw("empty vectors!");
    }
    
    uint32_t VAO, VBO, EBO;
    
    glGenVertexArrays(1, &VAO);
    //cout << VAO << endl;
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (const void*)offsetof(Vertex, Pos));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (const void*)offsetof(Vertex, UV));
    glEnableVertexAttribArray(1);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elem.size() * sizeof(uint32_t), elem.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    return MeshDetails(VAO, elem.size(), type);
}

MeshDetails UploadTileMesh(){
    vector<Vertex> sqVertices(8);
                sqVertices.at(0).Pos = vec3(-0.5f, 0.5f, 0.f);
                sqVertices.at(0).UV = vec2(0.0f, 1.0f);
                sqVertices.at(1).Pos = vec3(0.5f, 0.5f, 0.0f);
                sqVertices.at(1).UV = vec2(1.0f, 1.0f);
                sqVertices.at(2).Pos = vec3(-0.5f, -0.5f, 0.0f);
                sqVertices.at(2).UV = vec2(0.0f, 0.0f);
                sqVertices.at(3).Pos = vec3(0.5f, -0.5f, 0.0f);
                sqVertices.at(3).UV = vec2(1.0f, 0.0f);

    vector<uint32_t> elems(6);
    elems = {
                0, 3, 2,
                0, 1, 3
            };
    return UploadMesh(sqVertices, elems, "triangles");
}

 MeshDetails UploadTileOutlineVAO(){
    vector<Vertex> sqVertices(8);
                sqVertices.at(0).Pos = vec3(-0.5f, 0.5f, 0.f);
                sqVertices.at(1).Pos = vec3(0.5f, 0.5f, 0.0f);
                sqVertices.at(2).Pos = vec3(-0.5f, -0.5f, 0.0f);
                sqVertices.at(3).Pos = vec3(0.5f, -0.5f, 0.0f);

    vector<uint32_t> elems(8);
    elems = {
                0, 1,
                1, 3,
                3, 2,
                2, 0
            };
    return UploadMesh(sqVertices, elems, "lines");
}



 void UnloadMesh(vector<MeshDetails>& details){
    for (auto& d : details){
        glDeleteBuffers(1, &d.vao);
    }
    details.clear();
}
