//
//  Actors.cpp
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//

#include "Actors.h"

void Actor::Init(){}//{ID = home->sRegistry.create();}
IScene * Actor::getHome(){return home;}
entt::entity Actor::getID(){return ID;}
Actor::Actor(IScene *s, string n){home = s;name = n;}


//====================HERO====================
Hero::Hero(IScene *s, string n) : Actor(s,n) {home = s;}
Hero::~Hero(){}
std::string Hero::getEType(){return "hero";}
void Hero::Destroy(IScene* i){}
void Hero::Init(){}
vector<Action*> Hero::takeTurn(){
    vector<Action*> acts;
    //==========MOVEMENT==========
    float wdir, herowalkspeed, herosprintspeed;
    herowalkspeed = 3;
    herosprintspeed = 6;
    float heroturnspeed = 8;
    if (window.keyboard.keys[GLFW_KEY_W].down){
        wdir = 0 + (45.0 * window.keyboard.keys[GLFW_KEY_A].down);
        wdir -= (45.0f * window.keyboard.keys[GLFW_KEY_D].down);
        acts.push_back(new WalkAction(ID, wdir, window.keyboard.keys[GLFW_KEY_LEFT_SHIFT].down ? herosprintspeed : herowalkspeed));
        //DEBUG: these emplace or replaces will ensure that no more than 1 script can be on a hero at a time :(
        acts.push_back(new AddScriptAction(ID, new PointAtScript(heroturnspeed, ID, wdir)));
    } else if (window.keyboard.keys[GLFW_KEY_A].down){
        wdir = 90.0 - (45.0 * window.keyboard.keys[GLFW_KEY_W].down);
        wdir += (45.0f * window.keyboard.keys[GLFW_KEY_S].down);
        acts.push_back(new WalkAction(ID, wdir, window.keyboard.keys[GLFW_KEY_LEFT_SHIFT].down ? herosprintspeed : herowalkspeed));
        acts.push_back(new AddScriptAction(ID, new PointAtScript(heroturnspeed, ID, wdir)));
    } else if (window.keyboard.keys[GLFW_KEY_S].down){
        wdir = 180 - (45.0 * window.keyboard.keys[GLFW_KEY_A].down);
        wdir += (45.0f * window.keyboard.keys[GLFW_KEY_D].down);
        acts.push_back(new WalkAction(ID, wdir, window.keyboard.keys[GLFW_KEY_LEFT_SHIFT].down ? herosprintspeed : herowalkspeed));
        acts.push_back(new AddScriptAction(ID, new PointAtScript(heroturnspeed, ID, wdir)));
    } else if (window.keyboard.keys[GLFW_KEY_D].down){
        wdir = 270 - (45.0 * window.keyboard.keys[GLFW_KEY_S].down);
        wdir += (45.0f * window.keyboard.keys[GLFW_KEY_W].down);
        acts.push_back(new WalkAction(ID, wdir, window.keyboard.keys[GLFW_KEY_LEFT_SHIFT].down ? herosprintspeed : herowalkspeed));
        acts.push_back(new AddScriptAction(ID, new PointAtScript(heroturnspeed, ID, wdir)));
    }
    if (!window.keyboard.keys[GLFW_KEY_W].down && !window.keyboard.keys[GLFW_KEY_A].down &&
        !window.keyboard.keys[GLFW_KEY_S].down && !window.keyboard.keys[GLFW_KEY_D].down){
        home->sRegistry.remove<ScriptComponent>(ID); //TODO: garbage take out the
    }
    
    //==========AIM==========
    if (window.mouse.buttons[GLFW_MOUSE_BUTTON_2].down){
        window.setMouseGrabbed(true);
        acts.push_back(new AddScriptAction(ID, new PointAtScript(heroturnspeed, ID)));
        //TODO: scene should gather list of all comps that include dynamic pointers and delete them all on destruct, or, use entt func to listen for on destruct of any pointer comps & kill them then (better)
    }
    if (window.mouse.buttons[GLFW_MOUSE_BUTTON_2].released){
        //DEBUG: this currently deletes all scripts...
        window.setMouseGrabbed(false);
        window.setMousePos(window.mouse.position);
        home->sRegistry.remove<ScriptComponent>(ID); //TODO: garbage take out the (scrip removal action)
    }
    
    
    
    
    
    
    
    
    //MORE HERO ACTIONS.................
    vector<Action*> AB;
    AB.reserve( acts.size() + q.size() ); // preallocate memory
    AB.insert( AB.end(), acts.begin(), acts.end() );
    AB.insert( AB.end(), q.begin(), q.end() );
    q.clear();
    return AB;
}

void Hero::Spawn(Transform t){
    ID = home->sRegistry.create();
    home->sRegistry.emplace<ENameComponent>(ID, name);
    //emplace nec comps...
    home->sRegistry.emplace<TransformComponent>(ID, t);
    home->sRegistry.emplace<MeshComponent>(ID, home->sMeshes["tileMesh"]);
    //future tex emplaces read from hero class type object
    home->sRegistry.emplace<TextureComponent>(ID, home->sTextures["terrain"], glm::vec2(3.25,2), 16, 16, "playerwGun");
    home->sRegistry.emplace<DrawOrderComponent>(ID, 1);
    home->sRegistry.emplace<ShaderComponent>(ID, "default");
    home->sRegistry.emplace<BoxComponent>(ID, 0.15, glm::vec2(0.0));
    
//    home->sRegistry.emplace<TextureComponent>(ID, home->sTextures["terrain"], glm::vec2(1, 15), 32, 64, "hotRod");
//    home->sRegistry.emplace<BoxComponent>(ID, glm::vec2(1.5, 3.5), glm::vec2(0.0));
    
}

//====================GAUGE====================
Gauge::Gauge(IScene *s, string n) : Actor(s,n) {home = s;}
Gauge::~Gauge(){}
std::string Gauge::getEType(){return "gauge";}
void Gauge::Destroy(IScene* I){}
void Gauge::Init(){}
vector<Action*> Gauge::takeTurn(){
    vector<Action*> acts;
//    auto &heroTF = home->sRegistry.get<TransformComponent>(home->getEntityID("hero"));
//    float dir = heroTF.trans.rotation.z;
    float dir = home->sDiags["0"];
    acts.push_back(new AddScriptAction(needleID, new PointAtScript(2, needleID, dir)));
    
//    auto &TF = home->sRegistry.get<TransformComponent>(ID);
//    auto &nTF = home->sRegistry.get<TransformComponent>(needleID);
    
//    TF.trans.pos = home->screenToWorldSpace(glm::vec2(-0.75, -0.5));
//    nTF.trans.pos = home->screenToWorldSpace(glm::vec2(-0.75, -0.5));


    
    
    //MORE GAUGE TURNS...
    vector<Action*> AB;
    AB.reserve( acts.size() + q.size() ); // preallocate memory
    AB.insert( AB.end(), acts.begin(), acts.end() );
    AB.insert( AB.end(), q.begin(), q.end() );
    q.clear();
    return AB;
}

void Gauge::Spawn(Transform t){
    //===INIT FACE===
    ID = home->sRegistry.create();
    home->sRegistry.emplace<ENameComponent>(ID, name);

    //emplace nec comps...
    home->sRegistry.emplace<TransformComponent>(ID, t);
    home->sRegistry.emplace<MeshComponent>(ID, home->sMeshes["tileMesh"]);
    //future tex emplaces read from hero class type object
    home->sRegistry.emplace<TextureComponent>(ID, home->sTextures["terrain"], glm::vec2(3, 4), 64, 64, "360gaugeface");
    home->sRegistry.emplace<DrawOrderComponent>(ID, 7);
    home->sRegistry.emplace<ShaderComponent>(ID, "UI");
    
    //===INIT NEEDLE===
    needleID = home->sRegistry.create();
    //emplace nec comps...
    home->sRegistry.emplace<TransformComponent>(needleID, Transform(t.pos, t.scale, t.rotation, glm::vec3(0.0, 0.75, 0.0)));
    home->sRegistry.emplace<MeshComponent>(needleID, home->sMeshes["tileMesh"]);
    //future tex emplaces read from hero class type object
    home->sRegistry.emplace<TextureComponent>(needleID, home->sTextures["terrain"], glm::vec2(4.0, 4), 8, 32, "gaugeneedle");
    home->sRegistry.emplace<DrawOrderComponent>(needleID, 8);
    home->sRegistry.emplace<ShaderComponent>(needleID, "UI");
    
    
}

//====================OCAMERA====================
OCamera::OCamera(IScene *s, string n) : Actor(s,n) {home = s;}
OCamera::~OCamera(){}
std::string OCamera::getEType(){return "ocamera";}
void OCamera::Destroy(IScene* i){}
void OCamera::Init(){}
vector<Action*> OCamera::takeTurn(){
    vector<Action*> acts;
    if (window.mouse.scrollDelta.y == 0){
    } else {
        acts.push_back(new OCamZoomAction(ID, (float)window.mouse.scrollDelta.y));
    }
    acts.push_back(new CamTrackAction(ID, "hero"));

    
    
    
    
    //TODO: abstract this shit
    vector<Action*> AB;
    AB.reserve( acts.size() + q.size() ); // preallocate memory
    AB.insert( AB.end(), acts.begin(), acts.end() );
    AB.insert( AB.end(), q.begin(), q.end() );
    q.clear();
    return AB;
}

void OCamera::Spawn(Transform t){
    ID = home->sRegistry.create();
    home->sRegistry.emplace<ENameComponent>(ID, name);

    home->sRegistry.emplace<OrthoCameraComponent>(ID,
                                            t.pos,
                                            glm::vec3(0.0, 0.0, -1.0),
                                            glm::vec3(0.0, 1.0, 0.0),
                                            0.0001,
                                            10000.0,
                                            t.scale.x);
    home->sRegistry.emplace<TransformComponent>(ID, t);
    //spawning auto enables at this point
    auto &cComp = home->sRegistry.get<OrthoCameraComponent>(ID);
    cComp.enable = true;
}

//====================VEHICLE====================
Vehicle::Vehicle(IScene *s, string n) : Actor(s, n) {home = s;}
Vehicle::~Vehicle(){}
std::string Vehicle::getEType(){return "vehicle";}
void Vehicle::Destroy(IScene* I){}
void Vehicle::Init(){}
vector<Action*> Vehicle::takeTurn(){
    vector<Action*> acts;
    
//    home->sRegistry.emplace<DisplaceComponent>(ID, 0.1);
    
    
    //MORE VEHICLE TURNS...
    vector<Action*> AB;
    AB.reserve( acts.size() + q.size() ); // preallocate memory
    AB.insert( AB.end(), acts.begin(), acts.end() );
    AB.insert( AB.end(), q.begin(), q.end() );
    q.clear();
    return AB;
}

void Vehicle::Spawn(Transform t){
    ID = home->sRegistry.create();
    home->sRegistry.emplace<ENameComponent>(ID, name);

    //emplace nec comps...
    home->sRegistry.emplace<TransformComponent>(ID, t);
    home->sRegistry.emplace<MeshComponent>(ID, home->sMeshes["tileMesh"]);
    //future tex emplaces read from hero class type object
    home->sRegistry.emplace<TextureComponent>(ID, home->sTextures["terrain"], glm::vec2(1, 15), 32, 64, "hotRod");
    home->sRegistry.emplace<DrawOrderComponent>(ID, 1);
    home->sRegistry.emplace<ShaderComponent>(ID, "default");
    home->sRegistry.emplace<BoxComponent>(ID, glm::vec2(1.5, 3.5), glm::vec2(0.0));
}

//====================CURSOR====================
Cursor::Cursor(IScene *s,string n) : Actor(s, n) {home = s;}
Cursor::~Cursor(){}
std::string Cursor::getEType(){return "cursor";}
void Cursor::Destroy(IScene* I){}
void Cursor::Init(){}
vector<Action*> Cursor::takeTurn(){
    vector<Action*> acts;
    
    if (window.mouse.buttons[GLFW_MOUSE_BUTTON_2].down){
        auto &TF = home->sRegistry.get<TransformComponent>(ID);
        TF.trans.pos = home->mouseToWorldSpace(window.mouse.position);
    }
    if (window.mouse.buttons[GLFW_MOUSE_BUTTON_2].pressed){
        auto &DO = home->sRegistry.get<DrawOrderComponent>(ID);
        DO.priority = 2;
    }
    if (window.mouse.buttons[GLFW_MOUSE_BUTTON_2].released){
        auto &DO = home->sRegistry.get<DrawOrderComponent>(ID);
        DO.priority = -1;
    }
//    cout << window.mouse.position.x << " , " << window.mouse.position.y << endl;
    
    return acts;
}

void Cursor::Spawn(Transform t){
    ID = home->sRegistry.create();
    home->sRegistry.emplace<ENameComponent>(ID, name);

    //emplace nec comps...
    home->sRegistry.emplace<TransformComponent>(ID, Transform(home->mouseToWorldSpace(window.mouse.position), glm::vec3(0.5)));
    home->sRegistry.emplace<MeshComponent>(ID, home->sMeshes["tileMesh"]);
    home->sRegistry.emplace<TextureComponent>(ID, home->sTextures["terrain"], glm::vec2(2, 3), 8, 8, "aim");
    home->sRegistry.emplace<DrawOrderComponent>(ID, -1);
    home->sRegistry.emplace<ShaderComponent>(ID, "default");
}

//====================BLANK====================
Blank::Blank(IScene* s, string n) : Actor(s, n) {home = s;}
Blank::~Blank(){}
std::string Blank::getEType(){return "blank";}
void Blank::Destroy(IScene *s){}
void Blank::Init(){}
vector<Action*> Blank::takeTurn(){
    vector<Action*> acts;
    
    
    
    return acts;
}

void Blank::Spawn(Transform t){
    ID = home->sRegistry.create();
    home->sRegistry.emplace<ENameComponent>(ID, name);
    home->sRegistry.emplace<ShaderComponent>(ID, "default");
}

template<typename T>
void Blank::add(T & comp){
    home->sRegistry.emplace<T>(ID, comp);
}
