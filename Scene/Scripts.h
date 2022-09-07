#pragma once
//  Scripts.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/4/22.
//
#include "FMath.h"
#include "Script.h"
#include "Entity.h"
#include "Actions.h"
#include "Scene.h"

//TODO: change name to turnto
class PointAtScript : public Script {
public:
    float angle;
    float spd;
    char type;
    glm::vec3 target;
    entt::entity etarget;
    PointAtScript(float s, entt::entity h); //default: mouse
    PointAtScript(float s, entt::entity h, glm::vec3 tar); //focus on point
    PointAtScript(float s, entt::entity h, entt::entity tar); //focus on entity transform
    PointAtScript(float s, entt::entity h, float); //focus on angle
    virtual void onUpdate(IScene *scene, float dt) override;
    
};
