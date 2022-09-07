#pragma once
//  FMath.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/7/22.
//
//  some commonly used mathematical processes

//#define GLFW_INCLUDE_GLCOREARB
//#define GL_SILENCE_DEPRECATION
//#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
namespace fmath{

static float vectorToAngle(glm::vec3 vector){
    //calc angle
    float angle = glm::atan(-vector.x / vector.y) + (glm::radians(180.0) * (vector.y < 0));
    //keep range from 0 to 360
    if (angle < 0){
        angle += glm::radians(360.0);
    }
    if (vector.x == 0 && vector.y == 0){
        return glm::radians(-1.0);
    }
    return (angle);
    //RETURNS RADIANS
}

static float vectorToAngle(glm::vec2 vector){
    //calc angle
    float angle = glm::atan(-vector.x / vector.y) + (glm::radians(180.0) * (vector.y < 0));
    //keep range from 0 to 360
    if (angle < 0){
        angle += glm::radians(360.0);
    }
    if (vector.x == 0 && vector.y == 0){
        return glm::radians(-1.0);
    }
    return angle;
}


static glm::vec3 angleToVector(float angleDeg){
    angleDeg = glm::radians(angleDeg);
    return glm::vec3(-1 * glm::sin(angleDeg), glm::cos(angleDeg), 0.0);
}

static float smallestAngleFrom(float a, float b){
float diff;
    diff = b - a;
    if (glm::abs(diff) > 180.0){
        int sign = -1;
        if (diff < 0){
            sign = 1;
        }
        diff = (360 - glm::abs(diff)) * sign;
    }
    return diff;
}

static glm::vec3 rotateVector(glm::vec3 input, float deg){
    float len = glm::length(input);
    
    return angleToVector(glm::degrees(vectorToAngle(input)) + deg) * len;

}

static glm::vec2 rotateVector(glm::vec2 input, float deg){
    float len = glm::length(input);
    glm::vec3 a = angleToVector(glm::degrees(vectorToAngle(input)) + deg) * len;
    return glm::vec2(a.x, a.y);

}

static glm::vec2 project(glm::vec2 u, glm::vec2 v){
    float udotv = (u.x * v.x) + (u.y * v.y);
    float magvsq = glm::length(v);
    magvsq *= magvsq;
    return (udotv / magvsq) * v;
}

static glm::vec3 project(glm::vec3 u, glm::vec3 v){
    float udotv = (u.x * v.x) + (u.y * v.y);
    float magvsq = glm::length(glm::vec2(v.x, v.y));
    magvsq *= magvsq;
    return (udotv / magvsq) * v;
    
}





}
