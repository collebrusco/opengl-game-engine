//
//  Components.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//
#pragma once
#include <string>
#include "FTransform.h"
#include "Vertex.h"
#include "Script.h"
//#include "Action.h"

struct BaseComponent{
    int ownerID;
    virtual std::string getCType() = 0;
};

struct ENameComponent : public BaseComponent {
    std::string getCType() override;
    string ename;
    ENameComponent(const ENameComponent&) = default;
    ENameComponent(string n);
    ENameComponent(float, glm::vec2);
};

struct BoxComponent : public BaseComponent {
    char type;
    std::string getCType() override;
    glm::vec2 size, pos;
    float rad;
    BoxComponent(const BoxComponent&) = default;
    BoxComponent(glm::vec2, glm::vec2);
    BoxComponent(float, glm::vec2);
};

struct ScriptComponent : public BaseComponent { //uhm... this may be just replaced by aimaction
    std::string getCType() override;
    Script *scrip;
    ScriptComponent(const ScriptComponent&) = default;
    ScriptComponent(Script*s);
    operator Script*& () {return scrip;}
};

struct FocusPointComponent : public BaseComponent { //uhm... this may be just replaced by aimaction
    std::string getCType() override;
    glm::vec3 focalpoint;
    FocusPointComponent(const FocusPointComponent&) = default;
    FocusPointComponent(const glm::vec3 f);
    operator glm::vec3& () {return focalpoint;}
};

struct TransformComponent : public BaseComponent {
    std::string getCType() override;
    Transform trans;
    glm::mat4 modelMat;
    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const Transform t);
    operator Transform& () {return trans;}
};

struct DisplaceComponent : public BaseComponent{
    std::string getCType() override;
    glm::vec3 displace;
    float adisplace;
    DisplaceComponent(const DisplaceComponent&) = default;
    DisplaceComponent(const glm::vec3 d, float a);
    DisplaceComponent(float a);
    DisplaceComponent(const glm::vec3 d);
    operator glm::vec3& () {return displace;}
};

struct VelocityComponent : public BaseComponent{
    std::string getCType() override;
    Transform velocity;
//    VelocityComponent() = default;
    VelocityComponent(const VelocityComponent&) = default;
    VelocityComponent(const Transform v);
    //TODO: contructor that takes velo in polar or spherical
    operator Transform& () {return velocity;}
};

struct MeshComponent : public BaseComponent {
    std::string getCType() override;
    MeshDetails mesh;
    MeshComponent() = default;
    MeshComponent(const MeshComponent&) = default;
    MeshComponent(const MeshDetails m);
    operator MeshDetails& () { return mesh;}
};

struct TextureComponent : public BaseComponent {
    std::string getCType() override;
    int slot;
    glm::vec2 sheetPos;
    int spritePixW;
    int spritePixH;
    std::string name;
    TextureComponent() = default;
    TextureComponent(const TextureComponent&) = default;
    TextureComponent(const int sl, glm::vec2 SSpos, int spw, int sph, std::string n);
};

struct ShaderComponent : public BaseComponent {
    std::string getCType() override;
    std::string name;
    ShaderComponent() = default;
    ShaderComponent(const ShaderComponent&) = default;
    ShaderComponent(const std::string s);
    operator std::string& () { return name;}
};

struct DrawOrderComponent : public BaseComponent {
    std::string getCType() override;
    //ZEROS ALWAYS DRAWN FIRST, 1'S NEXT, SO ON
    int priority;
    DrawOrderComponent() = default;
    DrawOrderComponent(const DrawOrderComponent&) = default;
    DrawOrderComponent(const int p);
    operator int& () { return priority;}
};

struct OrthoCameraComponent : public BaseComponent {
    std::string getCType() override;
    bool enable;
    glm::vec3 pos, look, up;
    glm::mat4 view;
    float near, far;
    float viewWidth; //in game coords
    glm::mat4 proj;
    OrthoCameraComponent() = default;
    OrthoCameraComponent(const OrthoCameraComponent&) = default;
    OrthoCameraComponent(glm::vec3 p, glm::vec3 l, glm::vec3 u, float n, float f, float w);
};
