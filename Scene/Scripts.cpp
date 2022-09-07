//
//  Scripts.cpp
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/4/22.
//
#include "Scripts.h"

//===============POINT AT===============
PointAtScript::PointAtScript(float s, entt::entity h){
    spd = s;
    type = 'm'; //TYPE MOUSE (SCRIPT WILL POINT ENT AT MOUSE)
    host = h;
}
PointAtScript::PointAtScript(float s, entt::entity h, glm::vec3 tar){
    spd = s;
    type = 'p'; //TYPE POINT (SCRIPT WILL POINT ENT AT POINT IN GAME)
    host = h;
    target = tar;
}
PointAtScript::PointAtScript(float s, entt::entity h, entt::entity tar){
    spd = s;
    type = 'e'; //TYPE ENTITY (SCRIPT WILL POINT AT TRANSFORM OF IN GAME ENTITY)
    host = h;
    etarget = tar;
}
PointAtScript::PointAtScript(float s, entt::entity h, float a){
    spd = s;
    type = 'a'; //TYPE ANGLE (SCRIPT WILL POINT IN GIVEN DIRECTION)
    host = h;
    angle = glm::radians(a);
}
void PointAtScript::onUpdate(IScene *s, float dt){
    glm::vec3 posToLookAt;
    float diffCoeff;
    auto& hostTF = s->sRegistry.get<TransformComponent>(host);
    //switch target based on scrip type
    if (type == 'e'){
        auto& etargetTF = s->sRegistry.get<TransformComponent>(etarget);
        posToLookAt = etargetTF.trans.pos;
        glm::vec3 lookVec = posToLookAt - hostTF.trans.pos;
        angle = fmath::vectorToAngle(lookVec);
    } else if (type == 'p'){
        posToLookAt = target;
        glm::vec3 lookVec = posToLookAt - hostTF.trans.pos;
        angle = fmath::vectorToAngle(lookVec);
    } else if (type == 'm'){
        posToLookAt = s->mouseToWorldSpace(window.mouse.position);
        glm::vec3 lookVec = posToLookAt - hostTF.trans.pos;
        angle = fmath::vectorToAngle(lookVec);
    }
    
    diffCoeff = fmath::smallestAngleFrom(hostTF.trans.rotation.z, glm::degrees(angle));

    hostTF.trans.rotation.z += (diffCoeff * dt * 60) / spd;
    hostTF.trans.rotation.z -= (360 * (hostTF.trans.rotation.z > 360));
    hostTF.trans.rotation.z += (360 * (hostTF.trans.rotation.z < 0));

}
