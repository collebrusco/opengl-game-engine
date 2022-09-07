#pragma once
//  Script.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/5/22.
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
#include "entt.hpp"
#include "IScene.h"

class Script {
public:
    entt::entity host;
//    void Remove();
    virtual void onUpdate(IScene *scene, float dt) = 0;
};
