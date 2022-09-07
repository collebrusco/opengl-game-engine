#pragma once
//  Callbacks.h
//  workspace 2.0
//
//  Created by Frank Collebrusco on 5/21/22.
//
using namespace std;

static void glfw_error_callback(int error, const char* description){
    cout << "error: " << description << endl;
}

static void glfw_window_close_callback(GLFWwindow *window){
    cout << "window closed" << endl;
}

static void glfw_framebuffer_size_callback(GLFWwindow *window, int w, int h){
    glViewport(0, 0, w, h);
}

static void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    scrollx = xoffset;
    scrolly = yoffset;
    nanosAtLastScroll = nanos();
    cout << "Scroll (x,y) = " << scrollx << ", " << scrolly << endl;
    cout << &scrolly << endl;


}
static void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods){
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
}
