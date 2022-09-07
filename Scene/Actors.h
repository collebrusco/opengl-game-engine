#pragma once
//  Actors.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//
#include "Actions.h"
#include "Entity.h"
#include "Scripts.h"
#include <iostream>

class Actor : public Entity {
public:
    string name;
    std::vector<Action*> q;
    IScene * home;
    entt::entity ID;
    void Init() override;
    Actor(IScene *, string n);
    IScene * getHome() override;
    entt::entity getID() override;
};

class Hero : public Actor {
public:
    Hero(IScene*, string);
    ~Hero();
    void Init() override;
    std::string getEType() override;
    void Destroy(IScene*) override; //TODO remove entity from scene
    vector<Action*> takeTurn() override;
    void Spawn(Transform t) override;
};

class Gauge : public Actor {
private:
    entt::entity needleID;
public:
    Gauge(IScene*, string);
    ~Gauge();
    void Init() override;
    std::string getEType() override;
    void Destroy(IScene*) override; //TODO remove entity from scene
    void Destroy(); //TODO remove entity from scene
    vector<Action*> takeTurn() override;
    void Spawn(Transform t) override;
};

//.. blank actor that can be constructed on the fly by adding components, similar to 0.1

class OCamera : public Actor {
public:
    OCamera(IScene *, string);
    ~OCamera();
    void Init() override;
    std::string getEType() override;
    void Destroy(IScene*) override; //TODO remove entity from scene
    void Destroy();
    vector<Action*> takeTurn() override;
    void Spawn(Transform t) override;

};

class Vehicle : public Actor {
public:
    Vehicle(IScene*, string);
    ~Vehicle();
    void Init() override;
    std::string getEType() override;
    void Destroy(IScene*) override; //TODO remove entity from scene
    void Destroy(); //TODO remove entity from scene
    vector<Action*> takeTurn() override;
    void Spawn(Transform t) override;
};

class Cursor : public Actor {
public:
    Cursor(IScene*, string);
    ~Cursor();
    void Init() override;
    std::string getEType() override;
    void Destroy(IScene*) override; //TODO remove entity from scene
    void Destroy(); //TODO remove entity from scene
    vector<Action*> takeTurn() override;
    void Spawn(Transform t) override;
};

class Blank : public Actor {
public:
    Blank(IScene*, string);
    ~Blank();
    void Init() override;
    std::string getEType() override;
    void Destroy(IScene*) override; //TODO remove entity from scene
    void Destroy(); //TODO remove entity from scene
    vector<Action*> takeTurn() override;
    void Spawn(Transform t) override;
    
    template<typename T>
    void add(T &);
};
