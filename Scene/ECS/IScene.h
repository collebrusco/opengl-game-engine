#pragma once
//  IScene.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/2/22.
//
#include "entt.hpp"
#include "Shader.h"
#include "Vertex.h"
#include "Input.h"
#include "Window.h"

class IScene {
public:
    virtual glm::vec3 mouseToWorldSpace(glm::vec2 m) = 0;
    virtual glm::vec2 worldToMouseSpace(glm::vec3 w) = 0;
    virtual glm::vec2 worldToScreenSpace(glm::vec3 w) = 0;
    virtual glm::vec3 screenToWorldSpace(glm::vec2 s) = 0;

    //TODO: need access to entity in here... or some way to ask IScene for entity obj...
    //consider one more level of interface above entity?? is that bad?
    //it would allow nothing more than to have a func in here that returns a pointer to
    //that interface, from which code in actor can cast and use... currently for camera
    //to get hero tform. for now, ID return func... repalce in future with this ^
    virtual entt::entity getEntityID(string name) = 0;
    std::map<std::string, MeshDetails> sMeshes;
    std::map<std::string, int> sTextures;
    std::map<std::string, Shader> sShaders;
    std::map<std::string, float> sDiags;
    entt::registry sRegistry;
    InterfaceDetails *interfacePtr;
    ScrollDataLocations scroll;
    
    
};
