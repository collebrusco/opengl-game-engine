#pragma once
//
//  Input.h
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
#include<glm/vec2.hpp>
//#include<GLFW/glfw3.h>
using namespace glm;


static double scrollx;
static double scrolly;
static double nanosAtLastScroll;

struct ScrollDataLocations{
    //TODO: constructor inits to hold pointers to all scroll data so that one thing can be passed to scene
    ScrollDataLocations(){
        scrollxLoc = &scrollx;
        scrollyLoc = &scrolly;
        scrollnanosLoc = &nanosAtLastScroll;
    }
    
    double *scrollxLoc;
    double *scrollyLoc;
    double *scrollnanosLoc;
};

static vec2 getMousePosVector(GLFWwindow *window){
    vec2 pos;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    pos = vec2(x, y);
    return pos;
}

//void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//auto callback = (&InterfaceDetails::scroll_callback);
//void enableScroll(GLFWwindow* win){
//    glfwSetScrollCallback(win, glfw_scroll_callback);
//}

struct InterfaceDetails {
    InterfaceDetails(const InterfaceDetails& i){
        win = i.win;
        glfwGetFramebufferSize(win, &frame.x, &frame.y);
        mousePos = getMousePosVector(win);
        time = 0;
        aspect = (float)frame.x / (float)frame.y;
        NMP = vec2((mousePos.x / frame.x),
                                 (mousePos.y / frame.y));
        dmouse = vec2(0.0);
        mouseSensitivity = vec2(1.0);
    }
    InterfaceDetails(GLFWwindow *window){
//        callback = (GLFWscrollfun)(&InterfaceDetails::scroll_callback);
        win = window;
        glfwGetFramebufferSize(win, &frame.x, &frame.y);
        mousePos = getMousePosVector(win);
        time = 0;
        aspect = (float)frame.x / (float)frame.y;
        NMP = vec2((mousePos.x / frame.x),
                                 (mousePos.y / frame.y));
        dmouse = vec2(0.0);
        mouseSensitivity = vec2(1.0);
    }
    
    void update(){
        glfwGetFramebufferSize(win, &frame.x, &frame.y);
        vec2 oldMousePos = mousePos;
        mousePos = getMousePosVector(win);
        dmouse = mousePos - oldMousePos;
        dmouse.y *= -1;
        mousePos = getMousePosVector(win);
        time = (float)glfwGetTime();
        aspect = (float)frame.x / (float)frame.y;
//        std::cout << frame.x << " / " << frame.y << " = " << aspect << std::endl; //RIP INTEGER DIVISION
        NMP = vec2((mousePos.x / frame.x),
                   (mousePos.y / frame.y));
    }
public:
    double scrollx, scrolly;
    vec2 NMP;
    GLFWwindow* win;
    float aspect;
    ivec2 frame;
    vec2 mousePos;
    float time;
    vec2 dmouse;
    vec2 mouseSensitivity;
//    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
//        if (win == window){
//            scrollx = xoffset;
//            scrolly = yoffset;
//        }
//    }
};



//void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
//    
//    
//
//}
//GLFWscrollfun callback = (&glfw_scroll_callback);
//void enableScroll(InterfaceDetails* i){
//    glfwSetScrollCallback(i->win, glfw_scroll_callback);
//}
//















//From code tech tutorial
//static void ProcessInput(GLFWwindow *win){
//    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS){
//        glfwSetWindowShouldClose(win, true);
//    }
//}
