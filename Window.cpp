//
//  Window.cpp
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/4/22.
//  Some code adapted from jdh MC clone https://github.com/jdah/minecraft-weekend/blob/master/src/gfx/window.c
#include "Window.h"

 Window window;

static void _size_callback(GLFWwindow *handle, int width, int height){
    glViewport(0, 0, width, height);
    window.frame = glm::ivec2(width, height);
    window.aspect = (float)width / (float)height;
}

static void _error_callback(int error, const char* description){
    std::cout << "error: " << description << std::endl;
}

static void _window_close_callback(GLFWwindow *handle){
    std::cout << "window closed" << std::endl;
}

static void _cursor_callback(GLFWwindow *handle, double xp, double yp){
    glm::vec2 p = glm::vec2(xp,yp);
    window.mouse.delta = p - window.mouse.position;
    window.mouse.delta.x = glm::clamp(window.mouse.delta.x, -100.0f, 100.0f);
    window.mouse.delta.y = glm::clamp(window.mouse.delta.y, -100.0f, 100.0f);

    window.mouse.position = p;
}

static void _key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods){
    if (key < 0) {
        return;
    }
    
    switch (action){
        case GLFW_PRESS:
            window.keyboard.keys[key].down = true;
            break;
        case GLFW_RELEASE:
            window.keyboard.keys[key].down = false;
            break;
        default:
            break;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(handle, GLFW_TRUE);
    }
}

static void _mouse_callback(GLFWwindow *handle, int button, int action, int mods){
    if (button < 0){
        return;
    }
    switch (action) {
        case GLFW_PRESS:
            window.mouse.buttons[button].down = true;
            break;
        case GLFW_RELEASE:
            window.mouse.buttons[button].down = false;
            break;
        default:
            break;
    }
}

static void _scroll_callback(GLFWwindow* handle, double xoffset, double yoffset){
    window.mouse.scrollDelta.x = xoffset;
    window.mouse.scrollDelta.y = yoffset;
}

Window::Window(){
    glfwSetErrorCallback(_error_callback);
    if (!glfwInit()){
        std::cout << "Failed to initialize GLFW! :(\n";
        throw ("failed GLFW init");
    }
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
//    window.frame.x = 1440;
//    window.frame.y = 1080;
    window.frame.x = 960;
    window.frame.y = 720;
    window.aspect = (float)window.frame.x / (float)window.frame.y;
    window.handle = glfwCreateWindow(window.frame.x, window.frame.y, "GasPunk", NULL, NULL);
    if (!window.handle){ //redundant
        glfwTerminate();
        std::cout << "Failed to create window!" << std::endl;
    }
    
    //CALLBACKS
    glfwMakeContextCurrent(window.handle);
    glfwSetFramebufferSizeCallback(window.handle, _size_callback);
    glfwSetWindowCloseCallback(window.handle, _window_close_callback);
    glfwSetKeyCallback(window.handle, _key_callback);
    glfwSetScrollCallback(window.handle, _scroll_callback);
    glfwSetCursorPosCallback(window.handle, _cursor_callback);
    glfwSetMouseButtonCallback(window.handle, _mouse_callback);

    //V-SYNC
    glfwSwapInterval(1);
}

void Window::Update(){
    buttonArrayUpdate(GLFW_MOUSE_BUTTON_LAST, window.mouse.buttons);
    buttonArrayUpdate(GLFW_KEY_LAST, window.keyboard.keys);
    //TODO: improve non-scroll detection
    if (window.mouse.scrollDelta == window.mouse.scrollDeltaLast){
        window.mouse.scrollDelta = glm::vec2(0.0);
    }
    window.mouse.scrollDeltaLast = window.mouse.scrollDelta;
    
    
    
    glfwSwapBuffers(window.handle);
    glfwPollEvents(); //TODO: add dt to this obj
}

void Window::buttonArrayUpdate(size_t n, Button *butts){
    for (size_t i = 0; i < n; i++){
        butts[i].pressed = butts[i].down && !butts[i].last;
        butts[i].released = !butts[i].down && butts[i].last;
        butts[i].last = butts[i].down;
    }
}

void Window::setMouseGrabbed(bool grabbed){
    glfwSetInputMode(handle, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Window::setMousePos(glm::vec2 p){
    
    
    
    glfwSetCursorPos(handle, p.x, p.y);
}
