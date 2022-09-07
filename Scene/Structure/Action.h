#pragma once
//  Action.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//
#include "IScene.h"
#include "entt.hpp"
//#include "Entity.h"

class Action {
public:
    virtual void perform(IScene *scene, float dt) = 0;
//    virtual ~Action() = 0; ///TODO: destructor
//    virtual entt::entity getPerformer() = 0;
    entt::entity performer;
};
