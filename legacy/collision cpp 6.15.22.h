//
//  Collision.cpp
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/15/22.
//

#include "Collision.h"


//IScene *sc;
//entt::entity etobDisplaced;

Collision::Collision(IScene *s, entt::entity e){sc = s;etobDisplaced = e;}
void Collision::Check(){
    //TB MEANS TO BE DISPLACED
    //E MEANS ENVIRONMENTAL (ENTS GATHERED FROM SCENE IN SURROUNDINGS)
    auto [transTB, dispTB, boxTB] = sc->sRegistry.try_get<TransformComponent, DisplaceComponent, BoxComponent>(etobDisplaced);
    //TODO: check for collisions, bounding entswCollision, noclip, etc
    if (transTB && boxTB){
        free = true;
        afree = true;
        bool deflect = false;
        glm::vec2 callertoObj;
        glm::vec3 deflection;
        auto entswCollision = sc->sRegistry.view<BoxComponent>();

        for (auto entwCollision : entswCollision){
            if (entwCollision != etobDisplaced){
                auto [transE, boxE] = sc->sRegistry.try_get<TransformComponent, BoxComponent>(entwCollision);
                glm::vec3 callerBoxCenter = glm::vec3(transTB->trans.pos.x + boxTB->pos.x,
                                                      transTB->trans.pos.y + boxTB->pos.y,
                                                      transTB->trans.pos.z);
                glm::vec3 objBoxCenter = glm::vec3(transE->trans.pos.x + boxE->pos.x,
                                                   transE->trans.pos.y + boxE->pos.y,
                                                   transE->trans.pos.z);
                callertoObj = glm::vec2(objBoxCenter.x - callerBoxCenter.x, objBoxCenter.y - callerBoxCenter.y);
                float collangle = glm::degrees(fmath::vectorToAngle(glm::vec3(callertoObj, 0.0)));
                float dispangle = glm::degrees(fmath::vectorToAngle(dispTB->displace));
                float colltoDisp = fmath::smallestAngleFrom(collangle,
                                                            dispangle);
                    //TODO: all of these need to filter entities to collide check to within specified range which will be max boxcomp size to avoid looping over entities across the world from each other
                if (boxTB->type == 'c' && boxE->type == 'c'){
//BOX TO BE AND BOX ENVIRONMENT ARE BOTH RADIAL, CHECK ACCORDINGLY
                    bool cc = (glm::length((callerBoxCenter + dispTB->displace) - objBoxCenter) - boxE->rad - boxTB->rad) < 0;
                    if (cc){
                        free = false;
                        
                        int sign = 1;
                        if (colltoDisp < 0) {sign = -1;}
                        glm::vec3 V = fmath::angleToVector(collangle + (90 * sign));
                        float udotv = (dispTB->displace.x * V.x) + (dispTB->displace.y * V.y);
                        float magvsq = glm::length(V) * glm::length(V);
                        deflection = V * (udotv / magvsq);
                                                    
                            //TODO: check deflect against all other ents once more
                        deflect = true;
                            
                    }
                        
                } else if (boxTB->type == 'c' && boxE->type == 's') {
//BOX TO BE IS RADIAL, BOX E IS RECTANGULAR, CHECK ACCORDINGLY
                    auto objToCaller = (callertoObj * -1.0f) + glm::vec2(dispTB->displace.x, dispTB->displace.y);
                    glm::vec3 objToCaller_r = glm::vec3(fmath::rotateVector(objToCaller, -transE->trans.rotation.z), 0.0);
                    bool cs = ((glm::abs(objToCaller_r.x) < ((boxE->size.x / 2.0) + boxTB->rad)) &&
                                (glm::abs(objToCaller_r.y) < ((boxE->size.y / 2.0) + boxTB->rad)));
                    if (cs){
                        free = false;
                        //deflect...
                        glm::vec3 V;
                            
                        bool hittingWSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) <
                        (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
                        if (hittingWSide){
                            V = glm::vec3(0.0, (-2.0 * (objToCaller_r.x < 0)) + 1.0, 0.0);
                        } else {
                            V = glm::vec3((-2.0 * (objToCaller_r.y < 0)) + 1.0, 0.0, 0.0);
                        }
                        V = fmath::rotateVector(V, transE->trans.rotation.z);
                        cout << "V: " << V.x << ", " << V.y << endl;
                        float udotv = (dispTB->displace.x * V.x) + (dispTB->displace.y * V.y);
                        deflection = V * udotv;
                        cout << "deflection: " << deflection.x << ", " << deflection.y << "\n";
                        sc->sDiags["deflect"] = glm::degrees(fmath::vectorToAngle(deflection));
                        cout << "def angle: " << sc->sDiags["deflect"] << endl;
                        deflect = true;
                        
                            

                    }
                    
                } else if (boxTB->type == 's' && boxE->type == 's') {
//BOX TO BE AND BOX E ARE BOTH RECTANGULAR, CHECK ACCORDINGLY
                        
                } else if (boxTB->type == 's' && boxE->type == 'c') {
//BOX TO BE IS RECTANGULAR, BOX E IS RADIAL, CHECK ACCORDINGLY
//                    auto objToCaller = (callertoObj * 1.0f) - glm::vec2(dispTB->displace.x, dispTB->displace.y);
//                    glm::vec3 objToCaller_r = glm::vec3(fmath::rotateVector(objToCaller, -transTB->trans.rotation.z), 0.0);
//                    bool cs = ((glm::abs(objToCaller_r.x) < ((boxTB->size.x / 2.0) + boxE->rad)) &&
//                               (glm::abs(objToCaller_r.y) < ((boxTB->size.y / 2.0) + boxE->rad)));
//                    if (cs){
//                        cout << "Carbang!\n";
//                        free = false;
//                        //deflect...
//                        glm::vec3 V;
//
//                        bool hittingWSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) <
//                        (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
//                        if (hittingWSide){
//                            V = glm::vec3(0.0, (-2.0 * (objToCaller_r.x < 0)) + 1.0, 0.0);
//                        } else {
//                            V = glm::vec3((-2.0 * (objToCaller_r.y < 0)) + 1.0, 0.0, 0.0);
//                        }
//                        V = fmath::rotateVector(V, transE->trans.rotation.z);
//                        cout << "V: " << V.x << ", " << V.y << endl;
//                        float udotv = (dispTB->displace.x * V.x) + (dispTB->displace.y * V.y);
//                        deflection = V * udotv;
//                        cout << "deflection: " << deflection.x << ", " << deflection.y << "\n";
//                        sc->sDiags["deflect"] = glm::degrees(fmath::vectorToAngle(deflection));
//                        cout << "def angle: " << sc->sDiags["deflect"] << endl;
//                        deflect = true;
//                    }
//                    if (dispTB->adisplace != 0) {
//                        objToCaller_r = fmath::rotateVector(objToCaller_r, dispTB->adisplace);
//                        bool acs = ((glm::abs(objToCaller_r.x) < ((boxTB->size.x / 2.0) + boxE->rad)) &&
//                                   (glm::abs(objToCaller_r.y) < ((boxTB->size.y / 2.0) + boxE->rad)));
//                        cout << "acs: " << acs << "\n";
//                        if (acs){
//                            afree = false;
//                        }
//                        cout << "afree: " << afree << "\n";
//
//                    }
//










                }
            
            }
            if (!afree){cout << "BANG! angular\n";}
            if (!free){cout << "BANG! positional\n";}
            if (afree ){//}&& dispTB->adisplace != 0){
//                transTB->trans.rotation.z += dispTB->adisplace;
            }
            if (free){
                transTB->trans.pos += (dispTB->displace);
                sc->sRegistry.remove<DisplaceComponent>(etobDisplaced);
//                cout << dispTB->displace.x << " , " << dispTB->displace.y << endl;
            } else if (deflect){
                sc->sRegistry.remove<DisplaceComponent>(etobDisplaced);
                sc->sRegistry.emplace<DisplaceComponent>(etobDisplaced, deflection);
            } else {
                sc->sRegistry.remove<DisplaceComponent>(etobDisplaced);
            }
            
        }
    
    }
}
bool Collision::willCollide(){
    return free;
}
bool Collision::awillCollide(){
    return afree;
}
