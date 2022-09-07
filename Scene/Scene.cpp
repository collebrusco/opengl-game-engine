//
//  Scene.cpp
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//

#include "Scene.h"
Scene::~Scene(){
    for (auto& s : sShaders){
        s.second.destroy();
    }
}

void Scene::Destroy(){
    for (auto ent : sEnts){
        ent.second->Destroy(this);
        delete ent.second;
    }
    sRegistry.clear();
    
}
void Scene::AddMesh(string name, MeshDetails mds){
    sMeshes[name] = mds;
}
void Scene::AddTexture(string name, int slot){
    sTextures[name] = slot;
}

void Scene::outputMat4(mat4 m){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            cout << m[i][j] << " ";
        }
        cout << endl << endl;
    }
    
}

glm::mat4 Scene::genModelMat(Transform t){//, glm::mat4 *m){
    glm::mat4 m;
    m = glm::mat4(1.0);
    m = glm::translate(m, t.pos);
    m = glm::rotate(m, glm::radians(t.rotation.x), glm::vec3(1.0, 0.0, 0.0));
    m = glm::rotate(m, glm::radians(t.rotation.y), glm::vec3(0.0, 1.0, 0.0));
    m = glm::rotate(m, glm::radians(t.rotation.z), glm::vec3(0.0, 0.0, 1.0));
    m = glm::scale(m, t.scale);
    m = glm::translate(m, t.anchor);

    //std::cout << t.pos.x << ", " << t.pos.y << std::endl;
    return m;
}

Scene::Scene(GLFWwindow * win, ScrollDataLocations scr) //: interface(win)
{
//    win = window;
    lrbtMat[0] = glm::vec4(-0.5f, 0.5f, 0, 0);
    lrbtMat[1] = glm::vec4(0, 0, -0.5f, 0.5f);
    scroll.scrollnanosLoc = scr.scrollnanosLoc;
    scroll.scrollxLoc = scr.scrollxLoc;
    scroll.scrollyLoc = scr.scrollyLoc;
    //TODO move shader compilation somewhere?
    sShaders["default"] = Shader("vert shader", "frag shader");
    sShaders["UI"] = Shader("vert shader UI", "frag shader UI");

    
}

void Scene::Init(){
    //INIT & SPAWN CAMERA
    sEnts["camera"] = new OCamera(this, "camera"); //TODO: GARBAGE COLLECT THESE
    sEnts["camera"]->Spawn(Transform(glm::vec3(0.0, 0.0, 10.0), glm::vec3(10.0), glm::vec3(0.0)));
    
    //INIT & SPAWN HERO!
    sEnts["hero"] = new Hero(this, "hero");
    sEnts["hero"]->Spawn(Transform(glm::vec3(0.0), glm::vec3(1), glm::vec3(0.0)));
    
    //INIT & SPAWN CAR
    sEnts["car"] = new Vehicle(this, "car");
    sEnts["car"]->Spawn(Transform(glm::vec3(4.0, 2.0, 0.0), glm::vec3(1.0), glm::vec3(0.0, 0.0, 45.0), glm::vec3(0.0, 0.0, 0.0)));
    
    sEnts["hooptie"] = new Vehicle(this, "hooptie");
    sEnts["hooptie"]->Spawn(Transform(glm::vec3(4.0, 0.0, 0.0), glm::vec3(1.0), glm::vec3(0.0, 0.0, 100.0), glm::vec3(0.0, 0.0, 0.0)));
    
    sEnts["distant"] = new Vehicle(this, "distant");
    sEnts["distant"]->Spawn(Transform(glm::vec3(-40.0, 0.0, 0.0), glm::vec3(1.0), glm::vec3(0.0, 0.0, 100.0), glm::vec3(0.0, 0.0, 0.0)));
    
    sEnts["test"] = new Vehicle(this, "test");
    sEnts["test"]->Spawn(Transform(glm::vec3(1.0, -2.0, 0.0), glm::vec3(1.0), glm::vec3(0.0, 0.0, 300.0), glm::vec3(0.0, 0.0, 0.0)));
    
    sEnts["cursor"] = new Cursor(this, "cursor");
    sEnts["cursor"]->Spawn(Transform(glm::vec3(0.0)));
    
    sEnts["gauge"] = new Gauge(this, "gauge");
    sEnts["gauge"]->Spawn(Transform(glm::vec3(-1.5, -1, 0.0), glm::vec3(0.2)));
    
    entt::entity ID = sRegistry.create();
    sRegistry.emplace<ENameComponent>(ID, "test");
    //emplace nec comps...
    sRegistry.emplace<TransformComponent>(ID, Transform(glm::vec3(-6, -6, 0)));
    sRegistry.emplace<MeshComponent>(ID, sMeshes["tileMesh"]);
    //future tex emplaces read from hero class type object
//    home->sRegistry.emplace<TextureComponent>(ID, home->sTextures["terrain"], glm::vec2(3.25,2), 16, 16, "playerwGun");
    sRegistry.emplace<DrawOrderComponent>(ID, 1);
    sRegistry.emplace<ShaderComponent>(ID, "default");
//    home->sRegistry.emplace<BoxComponent>(ID, 0.15, glm::vec2(0.0));
    
    sRegistry.emplace<TextureComponent>(ID, sTextures["terrain"], glm::vec2(3, 4), 64, 64, "test");
    sRegistry.emplace<BoxComponent>(ID, 2, glm::vec2(0.0));
    
}

void Scene::Update(float dt){
//    interface.update();
    Script(dt);
    Action(dt);
    Physics(dt);
    Camera(dt);
}

void Scene::Script(float dt){
    auto ents = sRegistry.view<ScriptComponent>();
    for (auto ent : ents){
        auto& scrip = sRegistry.get<ScriptComponent>(ent);
        if (scrip){
            scrip.scrip->onUpdate(this, dt);
        }
    }
}

void Scene::Action(float dt){
    for (auto ent : sEnts){
        auto acts = ent.second->takeTurn();
        if (acts.size() != 0){
//            cout << "Performing action...\n";
            for (auto action : acts){
                action->perform(this, dt);
                delete action; //TODO: mandatory destructor
            }
        }
    }
}

void Scene::Physics(float dt){
    //VELOS
    //DEBUG dont know if these up on the fly
    auto velocomps = sRegistry.view<VelocityComponent>();
    auto tobDisplaced = sRegistry.view<DisplaceComponent>();

//    while (tobDisplaced.size() > 0 || velocomps.size() > 0 || i > 5) {
        for (auto  entity : velocomps){
            auto [t, v, d] = sRegistry.try_get<TransformComponent, VelocityComponent, DisplaceComponent>(entity);
            //TODO: check for collisions, bounding entswCollision, noclip, etc
            //maybe kinetic comp instead of velo? velo and acc in one?
            if (d){
                d->displace += v->velocity.pos;
                t->trans.scale += v->velocity.scale;
                d->displace += v->velocity.rotation;
            
            } else {
                sRegistry.emplace<DisplaceComponent>(entity, v->velocity.pos, v->velocity.rotation.z);
            }
        }
        //DISPLACE
        //TB MEANS TO BE DISPLACED
        //E MEANS ENVIRONMENTAL (ENTS GATHERED FROM SCENE IN SURROUNDINGS)
        auto entswCollision = sRegistry.view<BoxComponent>();
        for (auto  etobDisplaced : tobDisplaced){
                    //for each ent w box component:
                    //try get comps so loop knows who has what
                auto [transTB, dispTB, boxTB] = sRegistry.try_get<TransformComponent, DisplaceComponent, BoxComponent>(etobDisplaced);
                    // see if we can check these two (if they have trans & box comps)
                if (transTB && boxTB){
                        //establish collision object centered on ent to be displaced
                    vector<Collision*> colls;
                    int i = 0;
                    vec2 widestDeflect = vec2(0.0);
                    float widestDefAngle = 0;
                    vec2 disp = vec2(dispTB->displace.x, dispTB->displace.y);
                    bool defLeft = false;
                    bool defRight = false;
                    for (auto entwCollision : entswCollision){
                        
                        if (entwCollision != etobDisplaced){
                            Collision* coll = new Collision(this, etobDisplaced);
                            if (coll->Check(entwCollision)){
                                coll->displaceOverlap();
                                coll->free();
                                auto ename = sRegistry.get<ENameComponent>(entwCollision).ename;
                                auto cname = sRegistry.get<ENameComponent>(etobDisplaced).ename;

                                
                                if (coll->isOverlap){//} && coll->pointingIn) {
                                    colls.push_back(coll);
                                    
//                                    sRegistry.replace<DisplaceComponent>(etobDisplaced, coll->deflect());
//                                    sRegistry.replace<DisplaceComponent>(etobDisplaced, coll->deflection);
                                }
                            }
                        }
                        i++;
                    }
                    cout << "# of colls detected: " << colls.size() << "\n";
                    if(colls.size() != 1){
                    for (auto coll : colls){
                        if (coll->pointingIn){
//                        if abs of diff in angle from disp to def is bigger than any others yet, up it (search for largest)
                        if (abs(fmath::smallestAngleFrom(fmath::vectorToAngle(disp), fmath::vectorToAngle(coll->deflection))) > abs(widestDefAngle)
                            || colls.size() == 1){
                            cout << "settin widest def...\n";
                            widestDeflect = coll->deflection;
                            widestDefAngle = fmath::vectorToAngle(widestDeflect);
                        }
                        if (fmath::smallestAngleFrom(fmath::vectorToAngle(disp), fmath::vectorToAngle(coll->deflection)) < 0){
                            defRight = true;
                        } else if (fmath::smallestAngleFrom(fmath::vectorToAngle(disp), fmath::vectorToAngle(coll->deflection)) > 0){
                            defLeft = true;
                        }
                        }
                    }
                    if (!(defLeft && defRight) && colls.size() > 0) {
                        cout << "widest deflection: " << widestDeflect.x << ", " << widestDeflect.y << endl;
                        sDiags["0"] = glm::degrees(fmath::vectorToAngle(widestDeflect));
                        
                        for (auto coll : colls){
    //                        if abs of diff in angle from disp to def is bigger than any others yet, up it (search for largest)
                            
                            if (fmath::smallestAngleFrom(fmath::vectorToAngle(widestDeflect), fmath::vectorToAngle(coll->deflection)) < 0){
                                defRight = true;
                            } else if (fmath::smallestAngleFrom(fmath::vectorToAngle(widestDeflect), fmath::vectorToAngle(coll->deflection)) > 0){
                                defLeft = true;
                            }
                        }
                        if (!(defLeft&& defRight)){
                            cout << "Second check passed...\n";
                            sRegistry.replace<DisplaceComponent>(etobDisplaced, vec3(widestDeflect.x, widestDeflect.y, 0.0));
                        }
                        
                        
//                        sRegistry.replace<DisplaceComponent>(etobDisplaced, vec3(widestDeflect.x, widestDeflect.y, 0.0));
                    } //else
                    if ((defLeft && defRight) && colls.size() > 0) {
                        cout << "BLOCKING...\n";
                        sRegistry.remove<DisplaceComponent>(etobDisplaced);
                    }
                    
                    
                    } else {
                        for (auto coll : colls){
                            if (coll->pointingIn){
                                sRegistry.replace<DisplaceComponent>(etobDisplaced, coll->deflection);
                            }
                        }
                    }
                    
                    for (auto c : colls){delete c;}
                    dispTB = sRegistry.try_get<DisplaceComponent>(etobDisplaced);
                    if (dispTB){
                        cout << "MOVING...\n";
                        transTB->trans.pos += (dispTB->displace);
//                            cout << "in free, " << dispTB->displace.x << " , " << dispTB->displace.y << endl;
                    }
                    
                    
                    
                    
                }
            sRegistry.remove<DisplaceComponent>(etobDisplaced);
            }
    
}

void Scene::Camera(float dt){
    //TODO: only works w orthos rn
    //look through all cameras w tforms to get an enabled one
    //camera MUST HAVE TFORM to be considered here
    //TODO: enable(cameracomp cam) func somewhere
    auto validCameras = sRegistry.group<OrthoCameraComponent>(entt::get<TransformComponent>);
    for (entt::entity cam : validCameras){
        bool flag = false;
        //check if active
        const auto&[cameraComp, transformComp] = validCameras.get<OrthoCameraComponent, TransformComponent>(cam);
        if (cameraComp.enable){  //TODO: set up entt observer to check this so that currcam changes only up when necc (probly rare)
            currCam = cam;
            flag = true;
        }
        if (!flag){
            cout << "no valid camera enabled in current scene registry!\n";
        }
        auto [currCamT, currCamC] = sRegistry.get<TransformComponent, OrthoCameraComponent>(currCam);
        //link cam and trans
        currCamC.pos = currCamT.trans.pos;
        
        glm::vec3 target = currCamC.pos + currCamC.look;
        currCamC.view = lookAt(currCamC.pos, target, currCamC.up);
        
        glm::vec2 orthoDims = glm::vec2(currCamC.viewWidth, currCamC.viewWidth / window.aspect);

        glm::vec4 lrbt = (lrbtMat * orthoDims);

        orthoDims.y = orthoDims.x / window.aspect;
        lrbt = (lrbtMat * orthoDims);
        currCamC.proj = glm::ortho(lrbt.x, lrbt.y, lrbt.z, lrbt.w, currCamC.near, currCamC.far);

        //UPLOAD VIEWS AND PROJS TO ALL SHADERS
        currCamProj = currCamC.proj;
        currCamView = currCamC.view;
        currCamPos = currCamC.pos;
        currCamWorldFrame = orthoDims;
        //TODO: organize which shaders get what
        
//        for (auto shad : sShaders){
//            shad.second.bind();
//            shad.second.uMat4("uView", currCamC.view);
//            shad.second.uMat4("uProj", currCamC.proj);
//        }
        
       
        sShaders["default"].bind();
        sShaders["default"].uMat4("uView", currCamC.view);
        sShaders["default"].uMat4("uProj", currCamC.proj);
        
        glm::mat4 viewUI = lookAt(glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 projUI = glm::ortho((-2.0),
                                      (2.0),
                                      (-2.0 / window.aspect),
                                      (2.0 / window.aspect),
                                      0.0001,
                                      1000.0);
        sShaders["UI"].bind();
        sShaders["UI"].uMat4("uView", viewUI);
        sShaders["UI"].uMat4("uProj", projUI);


        

    }

}

void Scene::DrawSystemRender(){
    auto validRenders = sRegistry.group<MeshComponent>(entt::get<TransformComponent,
                                                                 DrawOrderComponent,
                                                                 TextureComponent,
                                                                 ShaderComponent>);
    for (int i = 0; i < 9; i++){
        for(auto entity : validRenders){
            //DEBUG: this doesnt check if any valid renders present and will bad access in that case
            auto [mesh, trans, order, tex, shad] = validRenders.get(entity);
            if (order.priority == i){
                sShaders[shad].bind();
                sShaders[shad].uVec2("uRes", window.frame);
//                sShaders[shad].uFloat("uTime", window.time);
                sShaders[shad].uVec2("uMouse", window.mouse.position);
                sShaders[shad].uMat4("uModel", glm::scale(genModelMat(trans.trans),
                                                          glm::vec3(tex.spritePixW / 16.0,
                                                                    tex.spritePixH / 16.0,
                                                                    1.0)));
                sShaders[shad].uInt("uTexture", tex.slot);
                sShaders[shad].uInt("uSpritePixW", tex.spritePixW);
                sShaders[shad].uInt("uSpritePixH", tex.spritePixH);
                sShaders[shad].uVec2("uSpriteSheetCoords", tex.sheetPos);
          
                DrawMesh(&mesh.mesh);
            }
        }
    }
}

glm::vec3 Scene::mouseToWorldSpace(glm::vec2 pm){
    glm::vec3 pworld;
    glm::vec2 pmouse = pm;
    pmouse.x /= window.frame.x;
    pmouse.x *= 2;
    pmouse.x -= 1;
    pmouse.y /= window.frame.y;
    pmouse.y *= 2;
    pmouse.y -= 1;
    pmouse.y *= -1;
    
    pworld.x = currCamPos.x + (pmouse.x * (currCamWorldFrame.x / 2));
    pworld.y = currCamPos.y + (pmouse.y * (currCamWorldFrame.y / 2));
//    cout << pworld.x << ", " << pworld.y << "\n";

    
    return pworld;
}
glm::vec2 Scene::worldToMouseSpace(glm::vec3 pworld){
    glm::vec2 pmouse;
    glm::mat4 model = genModelMat(Transform(pworld));
    pmouse = currCamProj * (currCamView * (model * vec4(pworld, 1.0f)));
    pmouse.x = (pmouse.x + 1) / 2;
    pmouse.y = (pmouse.y + 1) / 2;
    pmouse.x *= window.frame.x;
    pmouse.y *= window.frame.y;
    return pmouse;
}

glm::vec2 Scene::worldToScreenSpace(glm::vec3 pworld){
    glm::vec2 pscreen;
    glm::mat4 model = genModelMat(Transform(pworld));
    pscreen = currCamProj * (currCamView * (model * vec4(pworld, 1.0f)));
    return pscreen;
}

glm::vec3 Scene::screenToWorldSpace(glm::vec2 s){
    glm::vec3 pworld;
    glm::vec2 pscreen = s;
//    pscreen.x /= window.frame.x;
//    pscreen.x *= 2;
//    pscreen.x -= 1;
//    pscreen.y /= window.frame.y;
//    pscreen.y *= 2;
//    pscreen.y -= 1;
//    pscreen.y *= -1;
    
    pworld.x = currCamPos.x + (pscreen.x * (currCamWorldFrame.x / 2));
    pworld.y = currCamPos.y + (pscreen.y * (currCamWorldFrame.y / 2));
//    cout << pworld.x << ", " << pworld.y << "\n";

    
    return pworld;
}

//void removePointerComponent(entt::registry &reg, entt::entity ent){
////    auto pcomp
//}

entt::entity Scene::getEntityID(string name){
    return sEnts[name]->getID();
}
