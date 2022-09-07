//
//  Components.cpp
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//

#include "Components.h"

//struct BaseComponent{
//    int ownerID;
//    virtual std::string getCType() = 0;
//};

//=========================BOX=========================
std::string BoxComponent::getCType(){return "box";}
BoxComponent::BoxComponent(float r, glm::vec2 p){
    pos = p;
    rad = r;
    type = 'c';
}
BoxComponent::BoxComponent(glm::vec2 s, glm::vec2 p){
    size = s;
    pos = p;
    type = 's';
}

//=======================ENAME=======================
std::string ENameComponent::getCType(){return "ename";}
ENameComponent::ENameComponent(string n){
    ename = n;
}


//=========================SCRIPT=========================
std::string ScriptComponent::getCType(){return "script";}
ScriptComponent::ScriptComponent(Script* s){scrip = s;}


//=========================FOCUSPOINT=========================
std::string FocusPointComponent::getCType(){return "focuspoint";}
FocusPointComponent::FocusPointComponent(glm::vec3 f){
    focalpoint = f;
}

//=========================TRANSFORM=========================
std::string TransformComponent::getCType()  {return "transform";}
TransformComponent::TransformComponent(const Transform t){trans = t;}

//=======================DISPLACE========================
DisplaceComponent::DisplaceComponent(const glm::vec3 d){displace = d;adisplace = 0;}
DisplaceComponent::DisplaceComponent(float a){displace = glm::vec3(0.0);adisplace = a;}
DisplaceComponent::DisplaceComponent(const glm::vec3 d, float a){displace = d;adisplace = a;}

std::string DisplaceComponent::getCType() {return "displacement";}

//=========================VELO=========================
VelocityComponent::VelocityComponent(const Transform v){velocity = v;}
std::string VelocityComponent::getCType() {return "velocity";}

//=========================MESH=========================
std::string MeshComponent::getCType() {return "mesh";}
MeshComponent::MeshComponent(const MeshDetails m) {mesh = m;}

//=========================TEXTURE=========================
std::string TextureComponent::getCType() {return "texture";}
TextureComponent::TextureComponent(const int sl, glm::vec2 SSpos, int spw, int sph, std::string n){
    spritePixH = sph;
    spritePixW = spw;
    slot = sl;
    sheetPos = SSpos;
    name = n;
}

//=========================SHADER=========================
std::string ShaderComponent::getCType()  {return "shader";}
ShaderComponent::ShaderComponent(const std::string s){name = s;}

//=========================DRAW ORDER=========================
std::string DrawOrderComponent::getCType()  {return "draworder";}
DrawOrderComponent::DrawOrderComponent(const int p){priority = p;}

//=========================ORTHOCAMERA=========================
std::string OrthoCameraComponent::getCType()  {return "ocam";}
OrthoCameraComponent::OrthoCameraComponent(glm::vec3 p, glm::vec3 l, glm::vec3 u, float n, float f, float w){
    pos = p;
    look = l;
    up = u;
    near = n;
    far = f;
    viewWidth = w;
}


