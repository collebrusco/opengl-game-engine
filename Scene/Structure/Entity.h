#pragma once
//  Entity.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//
#include <string>
#include <vector>
#include "Components.h"
#include "Action.h"
#include "IScene.h"
#include "entt.hpp"
//#include "Scene.h"

/*
 Entity to live in Scene obj entirely if possible. action diag on blue star
 */

class Entity{
public:
//    virtual ~Entity() = 0;
    virtual void Init() = 0;
    virtual void Spawn(Transform) = 0;
    virtual std::string getEType() = 0;
    virtual void Destroy(IScene*) = 0;
    virtual vector<Action*> takeTurn() = 0;
    virtual IScene* getHome() = 0;
    virtual entt::entity getID() = 0;
};
