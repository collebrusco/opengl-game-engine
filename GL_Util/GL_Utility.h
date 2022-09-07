#pragma once
//  Utility.h
//  workspace 2.0
//
//  Created by Frank Collebrusco on 5/21/22.
//
//#include <glfw/glfw3.h>
//#include "Object.h"
//#include "Callbacks.h"
#include <vector>
//class Object;
bool InitHelper(GLFWwindow **window){
    if (!glfwInit()){
        cout << "Failed to initialize GLFW!" << endl;
        return false;

    }
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    *(window) = glfwCreateWindow(1024, 768, "title", NULL, NULL);
    if (!window){ //redundant
        glfwTerminate();
        cout << "Failed to create window!" << endl;
        return false;
    }
    
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, glfw_framebuffer_size_callback);
    glfwSetWindowCloseCallback(*window, glfw_window_close_callback);
    glfwSetKeyCallback(*window, keyCallback);
    glfwSetScrollCallback(*window, glfw_scroll_callback);
    
    return true;
}


// void UpdateAllObjects(vector<Object*> *ol){
//     for (int i = 0; i < ol->size(); i++){
//         ol->at(i)->Update();
//     }
//    //TODO maybe add a trans parenting system in OBJ class AE style
//    //that may have to be in here as some objs may need to get parent trans
//    //data from parents before they call update
//}

void outputMat4(mat4 m){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            cout << m[i][j] << " ";
        }
        cout << endl << endl;
    }
    
}

void outputVec3(vec3 v){
    cout << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
}
