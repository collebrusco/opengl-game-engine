//
//  Actions.cpp
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//

#include "Actions.h"
#include <iostream>
#include <math.h>

//=================================O CAM ZOOM=================================
OCamZoomAction::OCamZoomAction(entt::entity p, float r){
    performer = p;
    rate = r;
}

void OCamZoomAction::perform(IScene * scene, float dt){
    
    auto& cComp = scene->sRegistry.get<OrthoCameraComponent>(performer);
    cout << "zooming: dt = " << dt << ", rate = " << rate << " for vw of " << cComp.viewWidth << "\n";
//    cComp.viewWidth += (dt * rate);
    float& v = cComp.viewWidth;
    
    float sens = 8;
    if (rate < 0){
        v -= 0.8 * (log(v) * (sens / 10));
    } else if (rate > 0){
        v += 0.8 * (log(v) * (sens / 10));
    }
//    v += (v * rate * (sens / 100));
    v = glm::clamp(v, 5.0f, 110.0f);
}


//=================================WALK=================================
WalkAction::WalkAction(entt::entity p, float d, float s){
    performer = p;
    dir = d;
    speed = s;
}
void WalkAction::perform(IScene *scene, float dt){
    auto d = scene->sRegistry.try_get<DisplaceComponent>(performer);
    glm::vec3 step = vec3(-glm::sin(glm::radians(dir)), glm::cos(glm::radians(dir)), 0.0);
    step *= (speed * dt);
    if (d){
        d->displace += step;
    } else {
        scene->sRegistry.emplace<DisplaceComponent>(performer, step);
    }
}

//==============================ADD SCRIPT==============================
AddScriptAction::AddScriptAction(entt::entity p, Script* s){
    scrip = s;
    performer = p;
}
void AddScriptAction::perform(IScene *s, float dt){
    s->sRegistry.emplace_or_replace<ScriptComponent>(performer, scrip);
}

//==============================CAM TRACK==============================
CamTrackAction::CamTrackAction(entt::entity p, string par){
    parent = par;
    performer = p;
}
void CamTrackAction::perform(IScene *s, float dt){
    auto &TF = s->sRegistry.get<TransformComponent>(performer);
    auto &HTF = s->sRegistry.get<TransformComponent>(s->getEntityID(parent));
    glm::vec3 camToHeroV = HTF.trans.pos - TF.trans.pos;
    float camSnapSpeed = 10.0;
    TF.trans.pos.x += (camToHeroV.x * dt * 60) / camSnapSpeed;
    TF.trans.pos.y += (camToHeroV.y * dt * 60) / camSnapSpeed;
}

























//=================================ACCEL=================================
AccelAction::AccelAction(entt::entity p, float mp, float dir){
    performer = p;
    mpsps = mp;
    direction = dir;
}
//template these? that way actor can call thm w an actor to spawn if needed
void AccelAction::perform(IScene *scene, float dt){
    
}

