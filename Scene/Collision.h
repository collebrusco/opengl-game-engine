#pragma once
//  Collision.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/15/22.
//
#define GLM_SWIZZLE
#include "entt.hpp"
#include "fmath.h"
#include "Components.h"
#include "IScene.h"



class Collision {
private:
    IScene *sc;
    entt::entity etobDisplaced;
    entt::entity entwCollision;

public:
    Collision(IScene *s, entt::entity e);
    ~Collision() = default;
    bool Check(entt::entity e);
    bool displaceOverlap();
    bool displaceOverlapPos();
    bool displaceOverlapRot();
    bool staticOverlap();
    bool isStaticOverlap;
    bool isOverlap;
    
    
    bool free();
    bool isFree;
    glm::vec3 deflect();
    glm::vec3 deflection;
    glm::vec3 callerBoxCenter;
    glm::vec3 objBoxCenter;
    glm::vec2 callertoObj;
    
    glm::vec2 normal;
    glm::vec2 tangent;
    bool pointingIn;
    
    bool checkPointingIn(glm::vec3);
    
    string type;

    //todo return datat obj
    
};
