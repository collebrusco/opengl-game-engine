//
//  main.cpp
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION

//#include <opengl/gl.h>
//#include <glad/glad.h>
//#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <vector>
#include "entt.hpp"

#include "Window.h"
#include "FUtil/FTime.h"
#include "Input.h"
#include "GL_Util/GL_Callbacks.h"
#include "render/Shader.h"
#include "render/GL_Loader.h"
#include "Vertex.h"
#include "GL_Util/GL_Utility.h"
#include "render/GL_Draw.h"
#include "Actors.h"
#include "Actions.h"
#include "Scene.h"


using namespace std;


int main(int argc, const char * argv[]) {
//    glfwSetErrorCallback(glfw_error_callback);
//    GLFWwindow *window;
//    if (!InitHelper(&window)){
//        return -1;
//    }

//    InterfaceDetails interface(window.handle);
    glClearColor(200.0 / 255.0, 140.0 / 255.0, 80.0 / 255.0, 0.f);
//    glfwSetScrollCallback(window, glfw_scroll_callback);
    
    
//    static Camera cam(&interface,
//                      vec3(0.0, 0.0, 10.0),
//                      vec3(0.0, 0.0, 0.0),
//                      vec3(0.0, 1.0, 0.0),
//                      10.0,
//                      0.0001,
//                      10000.0);
    
//    vector<MeshDetails> tempVAOList;
//    tempVAOList.push_back(UploadTileMesh());
//
//    int terrainTexSlot = UploadTexture("terrain");
//
//    vector<MeshDetails*> tempDrawList;
//    tempDrawList.push_back(&tempVAOList.at(0));
    ScrollDataLocations scrolldatalocs;
    if (window.handle == nullptr){cout << "null window at scene init!";}

    Scene mainScene(window.handle, scrolldatalocs);
    mainScene.AddMesh("tileMesh", UploadTileMesh());
    mainScene.AddTexture("terrain", UploadTexture("terrain"));
    mainScene.Init();
    
    
    
    
    
    //TODO: vsync causes mysterious fucking left mouse bug...
    glfwSwapInterval(0);
    //LOOP
    double beginTime = nanos();
    double endTime = nanos();
    double dt = seconds();
    double dtn = nanos();
    while(!glfwWindowShouldClose(window.handle)){ //LLLLLLLLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOPPPPPPP
        cout << (1 / dt) << " FPS\n";
        
        //UPDATES
//        cout << *(scrolldatalocs.scrollyLoc) << "\n";
//        glfwPollEvents();
        glfwWaitEventsTimeout(1.0 / 120.0);
        mainScene.Update(dt);
        
        
        
        glClear(GL_COLOR_BUFFER_BIT);
        //RENDER
        //DRAW
        mainScene.DrawSystemRender();
//        glfwSwapBuffers(window.handle);
//        glfwPollEvents();
        window.Update();
//        UnloadMesh(tempVAOList);
        endTime = nanos();
        dtn = endTime - beginTime;
        dt = dtn / 1000000000;
        beginTime = endTime;
        cout << "---------\n";
    }
    
//UNLOAD
//    mainScene.Destroy();
//    UnloadMesh(tempVAOList);
    cout << "window closed!" << endl;
    glfwTerminate();
    return 0;
}




