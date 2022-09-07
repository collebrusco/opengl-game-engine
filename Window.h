#pragma once
//  Window.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/4/22.
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
#include <vector>

struct Button {
    bool down, last, lastTick, pressed, pressedTick, released;
};

struct Mouse {
    Button buttons[GLFW_MOUSE_BUTTON_LAST];
    glm::vec2 position, delta, scrollDelta, scrollDeltaLast;
};

struct Keyboard {
    Button keys[GLFW_KEY_LAST];
};

class Window {
public:
    GLFWwindow *handle;
    glm::ivec2 frame;
    float aspect;
    Mouse mouse;
    Keyboard keyboard;
    
    Window();
    Window(Window const&)            = delete;
    void operator = (Window const&)  = delete;
    
    void Update();
    void buttonArrayUpdate(size_t n, Button *butts);
    void setMouseGrabbed(bool);
    bool getMouseGrabbed();
    void setMousePos(glm::vec2);
};

extern Window window;
