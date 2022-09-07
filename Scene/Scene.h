#pragma once
//  Scene.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//
#include <iostream>
#include "GL_Draw.h"
#include "Actors.h"
#include "IScene.h"
#include "Entity.h"
#include "Collision.h"


class Scene : public IScene {
public:
    std::map<string, Entity*> sEnts;
    glm::mat2x4 lrbtMat;
//    InterfaceDetails interface;
    entt::entity currCam;
    glm::mat4 currCamView;
    glm::mat4 currCamProj;
    glm::vec3 currCamPos;
    glm::vec2 currCamWorldFrame;
    
    Scene(GLFWwindow*, ScrollDataLocations);
    ~Scene();
    void Init();
    void Update(float dt);
    void Destroy();
    glm::mat4 genModelMat(Transform t);
    
    void AddMesh(string name, MeshDetails mds);
    void AddTexture(string name, int slot);
    
    void Script(float dt);
    void Action(float dt);
    void Camera(float dt);
    void Physics(float dt);
    void DrawSystemRender();
    
    void outputMat4(mat4 m);
    glm::vec3 mouseToWorldSpace(glm::vec2 m) override;
    glm::vec2 worldToMouseSpace(glm::vec3 w) override;
    glm::vec2 worldToScreenSpace(glm::vec3 w) override;
    glm::vec3 screenToWorldSpace(glm::vec2 s) override;
    entt::entity getEntityID(string) override;
    
//    void removeComponent(entt::registry &, entt::entity);
    
};
