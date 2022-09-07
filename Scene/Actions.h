#pragma once
//  Actions.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//
#include "Action.h"
#include "ActionResult.h"
#include "Components.h"
#include "IScene.h"

//another approach... just have action operate directly as in bob ny... it has Iscene now
//iscene needs more involved interface but that may be the way. Still idk how to have this
//know about diff actors though...

//ALL ACTIONS MUST TAKE PERFORMERS ENTITY ID AS CONTRUCTOR ARG

class OCamZoomAction : public Action {
private:
    float rate;
public:
    OCamZoomAction(entt::entity p, float r);
    void perform(IScene *scene, float dt) override;
};

class WalkAction : public Action {
private:
    float speed; //yps (fps / 3)
    float dir;
public:
    WalkAction(entt::entity p, float, float);
    void perform(IScene *scene, float dt) override;
};

class AddScriptAction : public Action {
private:
    Script* scrip;
public:
    AddScriptAction(entt::entity p, Script* s);
    void perform(IScene *, float) override;
};


//TODO: move to mouse action, gamespace
class CamTrackAction : public Action {
private:
    string parent;
public:
    CamTrackAction(entt::entity p, string par);
    void perform(IScene *, float) override;
};






class AccelAction : public Action {
private:
    float mpsps;
    float direction;
public:
    AccelAction(entt::entity p, float mp, float dir);
    void perform(IScene *scene, float dt) override;
};
