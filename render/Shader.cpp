//
//  Shader.cpp
//  workspace
//
//  Created by Frank Collebrusco on 5/20/22.
//
//#ifdef __APPLE__
//#define glGenVertexArrays glGenVertexArraysAPPLE
//#define glBindVertexArray glBindVertexArrayAPPLE
//#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
//#define GL_SILENCE_DEPRECATION
//#endif
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#include "Shader.h"

//#include <string>
//#include <iostream>
//#include <vector>
//#include <glm/vec3.hpp> // glm::vec3
//#include <glm/vec4.hpp> // glm::vec4
//#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/type_ptr.hpp>


Shader::Shader(){

}

Shader::~Shader(){
//    cout << "shads destruct\n";
}

Shader::Shader(const char* vFileName, const char* fFileName){
    compileAndLink(vFileName, fFileName);
}

const char* Shader::getShaderSource(string shad, string type){
    ifstream fin;
    string path = "Shaders/" + shad + ".glsl";
    cout << path << endl;
    fin.open(path);
    if (!fin){
        cout << "error opening " + type + " shader!\n";
    }
    string contents((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    fin.close();
    static string src;
    src = contents;
    return src.c_str();
}

bool Shader::compileAndLink(const char* vFileName, const char* fFileName){
    bool flag = true;
    const char* vSource = getShaderSource(vFileName, "vert");
    
    
//    cout << vSource << endl;
    
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vSource, NULL);
    glCompileShader(vShader);
    GLint status;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE){
        cout << "vert shader compiled sucessfully!" << endl;
    } else {
        cout << "vert shader compilation failed!" << endl;
        flag = false;
    }
    
    const char* fSource = getShaderSource(fFileName, "frag");
//    cout << endl << fSource << endl;
    
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fSource, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE){
        cout << "frag shader compiled sucessfully!" << endl;
    } else {
        cout << "frag shader compilation failed!" << endl;
        flag = false;
    }
    
    //create program object
    GLuint prog = glCreateProgram();
    //attach vert & frags
    glAttachShader(prog, vShader);
    glAttachShader(prog, fShader);
    //link v&f
    glLinkProgram(prog);
    //error check link
    GLint isLinked = GL_FALSE;
    glGetProgramiv(prog, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE){
        flag = false;
        GLint maxLength = 0;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxLength);
        vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(prog, maxLength, &maxLength, &infoLog[0]);
        for (int i = 0; i < maxLength; i++){
            cout << infoLog[i];
        }
        cout << endl;
    } else {
        cout << "Shader link successful!\n";
    }
    
    //destroy individual shader objs
    glDetachShader(prog, vShader);
    glDetachShader(prog, fShader);
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    
    Shader::programId = prog;
    
    return flag;
    
}

void Shader::bind(){
    glUseProgram(programId);
}

void Shader::unBind(){
    glUseProgram(0);
}

void Shader::destroy(){
    glDeleteProgram(programId);
}

void Shader::uVec4(const char* varName, const glm::vec4& vec) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
}
void Shader::uVec3(const char* varName, const glm::vec3& vec) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform3f(loc, vec.x, vec.y, vec.z);
}
void Shader::uVec2(const char* varName, const glm::vec2& vec) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform2f(loc, vec.x, vec.y);
}
void Shader::uIVec4(const char* varName, const glm::ivec4& vec) const{
        GLint loc = glGetUniformLocation(programId, varName);
        glUniform4i(loc, vec.x, vec.y, vec.z, vec.w);
}
void Shader::uIVec3(const char* varName, const glm::ivec3& vec) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform3i(loc, vec.x, vec.y, vec.z);
}
void Shader::uIVec2(const char* varName, const glm::ivec2& vec) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform2i(loc, vec.x, vec.y);
}
void Shader::uFloat(const char* varName, const float val) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1f(loc, val);
}
void Shader::uInt(const char* varName, const int val) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1i(loc, val);
}
void Shader::uIntArr(const char* varName, int size, const int* arr) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1iv(loc, size, arr);
}
void Shader::uIntU(const char* varName, uint32_t val) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1i(loc, val);
}
void Shader::uBool(const char* varName, bool val) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniform1i(loc, val);
}

void Shader::uMat4(const char* varName, const glm::mat4& mat) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::uMat3(const char* varName, const glm::mat3& mat) const{
    GLint loc = glGetUniformLocation(programId, varName);
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}
