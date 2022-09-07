//
//  Collision.cpp
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/15/22.
//

#include "Collision.h"

using namespace glm;
//IScene *sc;
//entt::entity etobDisplaced;

/*
 COLLISION CHECKER
 establish a collision obj for an ent that needs to be checked & has a box.
 register another entity w/ a box using Check(ent)
 to check if it is statically overlapping the checked ent, call staticOverlap()
 to check if it will overlap the checked ent after displacement, call displaceOverlap()
 to check overlap with only positional or rotational displacements, call displaceOverlapPos() or displaceOverlapRot()
 each overlap call sets up member data for use:
 
 bool isOverlap to see if they'll touch,
 normal and tangent vectors,
 for non static checks only:
    disp vector, deflect vector, (disp proj'd onto tan),
    bool pointingIn checks if disp is going toward the obj,
    if not pointintIn then movement shouldnt be stopped (this was corner grabbing error)
 */

Collision::Collision(IScene *s, entt::entity e){sc = s;etobDisplaced = e;}
//CHECK LOCKS ON TO GIVEN ENT AND RETURNS FALSE IF IT IS OVER 50 IN GAME UNITS AWAY
bool Collision::Check(entt::entity e){  //, float checkradius){
    entwCollision = e;

    auto [transTB, dispTB, boxTB] = sc->sRegistry.try_get<TransformComponent, DisplaceComponent, BoxComponent>(etobDisplaced);
    auto [transE, boxE] = sc->sRegistry.try_get<TransformComponent, BoxComponent>(entwCollision);

    callerBoxCenter = glm::vec3(transTB->trans.pos.x + boxTB->pos.x,
                                          transTB->trans.pos.y + boxTB->pos.y,
                                          transTB->trans.pos.z);
    objBoxCenter = glm::vec3(transE->trans.pos.x + boxE->pos.x,
                                       transE->trans.pos.y + boxE->pos.y,
                                       transE->trans.pos.z);
    callertoObj = glm::vec2(objBoxCenter.x - callerBoxCenter.x, objBoxCenter.y - callerBoxCenter.y);
    
    
    
    
    float dist = glm::length(glm::vec2(transTB->trans.pos.x - transE->trans.pos.x, transTB->trans.pos.y - transE->trans.pos.y));
    return (dist < 50.0);
    
}


bool Collision::displaceOverlap(){
    //get comps of ent to be moved
    auto [transTB, dispTB, boxTB] = sc->sRegistry.try_get<TransformComponent, DisplaceComponent, BoxComponent>(etobDisplaced);
    auto [transE, boxE] = sc->sRegistry.try_get<TransformComponent, BoxComponent>(entwCollision);

    isOverlap = false;
    if (transTB && boxTB){
        glm::vec2 callertoObj;
        if (entwCollision != etobDisplaced){
            glm::vec3 callerBoxCenterS = glm::vec3(transTB->trans.pos.x + boxTB->pos.x,
                                                  transTB->trans.pos.y + boxTB->pos.y,
                                                  transTB->trans.pos.z);
            glm::vec3 objBoxCenterS = glm::vec3(transE->trans.pos.x + boxE->pos.x,
                                               transE->trans.pos.y + boxE->pos.y,
                                               transE->trans.pos.z);
            callertoObj = glm::vec2(objBoxCenterS.x - callerBoxCenterS.x, objBoxCenterS.y - callerBoxCenterS.y);
            float collangle = glm::degrees(fmath::vectorToAngle(glm::vec3(callertoObj, 0.0)));
            float dispangle = glm::degrees(fmath::vectorToAngle(dispTB->displace));
            float colltoDisp = fmath::smallestAngleFrom(collangle,
                                                        dispangle);
            
            if (boxTB->type == 'c' && boxE->type == 'c'){
//BOX TO BE AND BOX ENVIRONMENT ARE BOTH RADIAL, CHECK ACCORDINGLY
                bool cc = (glm::length((callerBoxCenterS + dispTB->displace) - objBoxCenterS) - boxE->rad - boxTB->rad) < 0;
                if (cc){
                    isOverlap = true;
                }
                //SET TAN
                int sign = 1;
                if (colltoDisp < 0) {sign = -1;}
                tangent = fmath::angleToVector(collangle + (90 * sign));

            } else if (boxTB->type == 'c' && boxE->type == 's') {
//BOX TO BE IS RADIAL, BOX E IS RECTANGULAR, CHECK ACCORDINGLY
                auto objToCaller = (callertoObj * -1.0f) + glm::vec2(dispTB->displace.x, dispTB->displace.y);
                glm::vec3 objToCaller_r = glm::vec3(fmath::rotateVector(objToCaller, -(transE->trans.rotation.z + dispTB->adisplace)), 0.0);
//                cout << "In coll::overlap, box bein checked is r'd by " << transE->trans.rotation.z << endl;
                bool cs = ((glm::abs(objToCaller_r.x) <= ((boxE->size.x / 2.0) + (boxTB->rad / 1.41))) &&
                            (glm::abs(objToCaller_r.y) <= ((boxE->size.y / 2.0) + (boxTB->rad / 1.41))));

                if (cs){
                    isOverlap = true;
                } else {return isOverlap;}
                //SET TAN
                bool hittingWSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) <
                (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
                bool hittingHSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) >
                (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
                
                if (hittingWSide){
                    tangent = glm::vec3(0.0, (-2.0 * (objToCaller_r.x < 0)) + 1.0, 0.0);
                    tangent = fmath::rotateVector(tangent, transE->trans.rotation.z);
                    
                } else if (hittingHSide){
                    tangent = glm::vec3((-2.0 * (objToCaller_r.y < 0)) + 1.0, 0.0, 0.0);
                    tangent = fmath::rotateVector(tangent, transE->trans.rotation.z);
                }
                //SET NORM
                if (hittingWSide){
                    if (objToCaller_r.x < 0){
                        normal = glm::vec2(-1.0, 0.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    } else {
                        normal = glm::vec2(1.0, 0.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    }
                } else {
                    if (objToCaller_r.y < 0){
                        normal = glm::vec2(0.0, -1.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    } else {
                        normal = glm::vec2(0.0, 1.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    }
                }
                //SET POINTING IN
                pointingIn = glm::abs(fmath::smallestAngleFrom(glm::degrees(fmath::vectorToAngle(dispTB->displace)), glm::degrees(fmath::vectorToAngle(normal)))) > 90.0;
                //SET DEFLECT
                deflection = fmath::project(dispTB->displace, vec3(tangent.x, tangent.y, 0.0));
            } else if (boxTB->type == 's' && boxE->type == 's') {
//BOX TO BE AND BOX E ARE BOTH RECTANGULAR, CHECK ACCORDINGLY
            } else if (boxTB->type == 's' && boxE->type == 'c') {
//BOX TO BE IS RECTANGULAR, BOX E IS RADIAL, CHECK ACCORDINGLY
                auto objToCaller = callertoObj - glm::vec2(dispTB->displace.x, dispTB->displace.y);
                glm::vec3 objToCaller_r = glm::vec3(fmath::rotateVector(objToCaller, -(transTB->trans.rotation.z + dispTB->adisplace)), 0.0);
//                cout << "In coll::overlap, box bein checked is r'd by " << transE->trans.rotation.z << endl;
                bool cs = ((glm::abs(objToCaller_r.x) <= ((boxTB->size.x / 2.0) + (boxE->rad / 1.41))) &&
                            (glm::abs(objToCaller_r.y) <= ((boxTB->size.y / 2.0) + (boxE->rad / 1.41))));

                if (cs){
                    isOverlap = true;
                    cout << "overlap\n";

                } else {return isOverlap;}
                //SET TAN
                bool hittingWSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) <
                (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
                bool hittingHSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) >
                (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
                
                if (hittingWSide){
                    tangent = glm::vec3(0.0, (-2.0 * (objToCaller_r.x < 0)) + 1.0, 0.0);
                    tangent = fmath::rotateVector(tangent, transE->trans.rotation.z);
                    
                } else if (hittingHSide){
                    tangent = glm::vec3((-2.0 * (objToCaller_r.y < 0)) + 1.0, 0.0, 0.0);
                    tangent = fmath::rotateVector(tangent, transE->trans.rotation.z);
                }
                //SET NORM
                if (hittingWSide){
                    if (objToCaller_r.x < 0){
                        normal = glm::vec2(-1.0, 0.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    } else {
                        normal = glm::vec2(1.0, 0.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    }
                } else {
                    if (objToCaller_r.y < 0){
                        normal = glm::vec2(0.0, -1.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    } else {
                        normal = glm::vec2(0.0, 1.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    }
                }
                normal *= -1.0;
                //SET POINTING IN
                pointingIn = glm::abs(fmath::smallestAngleFrom(glm::degrees(fmath::vectorToAngle(dispTB->displace)), glm::degrees(fmath::vectorToAngle(normal)))) > 90.0;
                //SET DEFLECT
                deflection = fmath::project(dispTB->displace, vec3(tangent.x, tangent.y, 0.0));
            }
        }
    }
    isFree = !isOverlap;
    return isOverlap;
}


bool Collision::displaceOverlapPos(){
    //get comps of ent to be moved
    auto [transTB, dispTB, boxTB] = sc->sRegistry.try_get<TransformComponent, DisplaceComponent, BoxComponent>(etobDisplaced);
    auto [transE, boxE] = sc->sRegistry.try_get<TransformComponent, BoxComponent>(entwCollision);

    isOverlap = false;
    if (transTB && boxTB){
        glm::vec2 callertoObj;
        if (entwCollision != etobDisplaced){
            glm::vec3 callerBoxCenterS = glm::vec3(transTB->trans.pos.x + boxTB->pos.x,
                                                  transTB->trans.pos.y + boxTB->pos.y,
                                                  transTB->trans.pos.z);
            glm::vec3 objBoxCenterS = glm::vec3(transE->trans.pos.x + boxE->pos.x,
                                               transE->trans.pos.y + boxE->pos.y,
                                               transE->trans.pos.z);
            callertoObj = glm::vec2(objBoxCenterS.x - callerBoxCenterS.x, objBoxCenterS.y - callerBoxCenterS.y);
            float collangle = glm::degrees(fmath::vectorToAngle(glm::vec3(callertoObj, 0.0)));
            float dispangle = glm::degrees(fmath::vectorToAngle(dispTB->displace));
            float colltoDisp = fmath::smallestAngleFrom(collangle,
                                                        dispangle);
            
            if (boxTB->type == 'c' && boxE->type == 'c'){
//BOX TO BE AND BOX ENVIRONMENT ARE BOTH RADIAL, CHECK ACCORDINGLY
                bool cc = (glm::length((callerBoxCenterS + dispTB->displace) - objBoxCenterS) - boxE->rad - boxTB->rad) < 0;
                if (cc){
                    isOverlap = true;
                }
                //SET TAN
                int sign = 1;
                if (colltoDisp < 0) {sign = -1;}
                tangent = fmath::angleToVector(collangle + (90 * sign));

            } else if (boxTB->type == 'c' && boxE->type == 's') {
//BOX TO BE IS RADIAL, BOX E IS RECTANGULAR, CHECK ACCORDINGLY
                auto objToCaller = (callertoObj * -1.0f) + glm::vec2(dispTB->displace.x, dispTB->displace.y);
                glm::vec3 objToCaller_r = glm::vec3(fmath::rotateVector(objToCaller, -transE->trans.rotation.z), 0.0);
//                cout << "In coll::overlap, box bein checked is r'd by " << transE->trans.rotation.z << endl;
                bool cs = ((glm::abs(objToCaller_r.x) <= ((boxE->size.x / 2.0) + (boxTB->rad / 1.41))) &&
                            (glm::abs(objToCaller_r.y) <= ((boxE->size.y / 2.0) + (boxTB->rad / 1.41))));

                if (cs){
                    isOverlap = true;
                } else {return isOverlap;}
                //SET TAN
                bool hittingWSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) <
                (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
                bool hittingHSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) >
                (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
                
                if (hittingWSide){
                    tangent = glm::vec3(0.0, (-2.0 * (objToCaller_r.x < 0)) + 1.0, 0.0);
                    tangent = fmath::rotateVector(tangent, transE->trans.rotation.z);
                    
                } else if (hittingHSide){
                    tangent = glm::vec3((-2.0 * (objToCaller_r.y < 0)) + 1.0, 0.0, 0.0);
                    tangent = fmath::rotateVector(tangent, transE->trans.rotation.z);
                }
                //SET NORM
                if (hittingWSide){
                    if (objToCaller_r.x < 0){
                        normal = glm::vec2(-1.0, 0.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    } else {
                        normal = glm::vec2(1.0, 0.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    }
                } else {
                    if (objToCaller_r.y < 0){
                        normal = glm::vec2(0.0, -1.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    } else {
                        normal = glm::vec2(0.0, 1.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    }
                }
                //SET POINTING IN
                pointingIn = glm::abs(fmath::smallestAngleFrom(glm::degrees(fmath::vectorToAngle(dispTB->displace)), glm::degrees(fmath::vectorToAngle(normal)))) < 90.0;
            } else if   (boxTB->type == 's' && boxE->type == 's') {
//BOX TO BE AND BOX E ARE BOTH RECTANGULAR, CHECK ACCORDINGLY
            } else if (boxTB->type == 's' && boxE->type == 'c') {
//BOX TO BE IS RECTANGULAR, BOX E IS RADIAL, CHECK ACCORDINGLY
            }
        }
    }
    isFree = !isOverlap;
    return isOverlap;
}

bool Collision::displaceOverlapRot(){
    //get comps of ent to be moved
    auto [transTB, dispTB, boxTB] = sc->sRegistry.try_get<TransformComponent, DisplaceComponent, BoxComponent>(etobDisplaced);
    auto [transE, boxE] = sc->sRegistry.try_get<TransformComponent, BoxComponent>(entwCollision);
    float collangle = glm::degrees(fmath::vectorToAngle(glm::vec3(callertoObj, 0.0)));
    float dispangle = glm::degrees(fmath::vectorToAngle(dispTB->displace));
    float colltoDisp = fmath::smallestAngleFrom(collangle,
                                                dispangle);
    isOverlap = false;
    if (transTB && boxTB){
        glm::vec2 callertoObj;
        if (entwCollision != etobDisplaced){
            glm::vec3 callerBoxCenterS = glm::vec3(transTB->trans.pos.x + boxTB->pos.x,
                                                  transTB->trans.pos.y + boxTB->pos.y,
                                                  transTB->trans.pos.z);
            glm::vec3 objBoxCenterS = glm::vec3(transE->trans.pos.x + boxE->pos.x,
                                               transE->trans.pos.y + boxE->pos.y,
                                               transE->trans.pos.z);
            callertoObj = glm::vec2(objBoxCenterS.x - callerBoxCenterS.x, objBoxCenterS.y - callerBoxCenterS.y);
            float collangle = glm::degrees(fmath::vectorToAngle(glm::vec3(callertoObj, 0.0)));
            float dispangle = glm::degrees(fmath::vectorToAngle(dispTB->displace));
            float colltoDisp = fmath::smallestAngleFrom(collangle,
                                                        dispangle);
            if (boxTB->type == 'c' && boxE->type == 'c'){
//BOX TO BE AND BOX ENVIRONMENT ARE BOTH RADIAL, CHECK ACCORDINGLY
                bool cc = (glm::length((callerBoxCenterS + dispTB->displace) - objBoxCenterS) - boxE->rad - boxTB->rad) < 0;
                if (cc){
                    isOverlap = true;
                }
                //SET TAN
                int sign = 1;
                if (colltoDisp < 0) {sign = -1;}
                tangent = fmath::angleToVector(collangle + (90 * sign));

            } else if (boxTB->type == 'c' && boxE->type == 's') {
//BOX TO BE IS RADIAL, BOX E IS RECTANGULAR, CHECK ACCORDINGLY
                auto objToCaller = (callertoObj * -1.0f);
                glm::vec3 objToCaller_r = glm::vec3(fmath::rotateVector(objToCaller, -(transE->trans.rotation.z + dispTB->adisplace)), 0.0);
//                cout << "In coll::overlap, box bein checked is r'd by " << transE->trans.rotation.z << endl;
                bool cs = ((glm::abs(objToCaller_r.x) <= ((boxE->size.x / 2.0) + (boxTB->rad / 1.41))) &&
                            (glm::abs(objToCaller_r.y) <= ((boxE->size.y / 2.0) + (boxTB->rad / 1.41))));

                if (cs){
                    isOverlap = true;
                } else {return isOverlap;}
                //SET TAN
                bool hittingWSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) <
                (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
                bool hittingHSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) >
                (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
                
                if (hittingWSide){
                    tangent = glm::vec3(0.0, (-2.0 * (objToCaller_r.x < 0)) + 1.0, 0.0);
                    tangent = fmath::rotateVector(tangent, transE->trans.rotation.z);
                    
                } else if (hittingHSide){
                    tangent = glm::vec3((-2.0 * (objToCaller_r.y < 0)) + 1.0, 0.0, 0.0);
                    tangent = fmath::rotateVector(tangent, transE->trans.rotation.z);
                }
                //SET NORM
                if (hittingWSide){
                    if (objToCaller_r.x < 0){
                        normal = glm::vec2(-1.0, 0.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    } else {
                        normal = glm::vec2(1.0, 0.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    }
                } else {
                    if (objToCaller_r.y < 0){
                        normal = glm::vec2(0.0, -1.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    } else {
                        normal = glm::vec2(0.0, 1.0);
                        normal = fmath::rotateVector(normal, transE->trans.rotation.z);
                    }
                }
                //SET POINTING IN
                pointingIn = glm::abs(fmath::smallestAngleFrom(glm::degrees(fmath::vectorToAngle(dispTB->displace)), glm::degrees(fmath::vectorToAngle(normal)))) < 90.0;
            } else if   (boxTB->type == 's' && boxE->type == 's') {
//BOX TO BE AND BOX E ARE BOTH RECTANGULAR, CHECK ACCORDINGLY
            } else if (boxTB->type == 's' && boxE->type == 'c') {
//BOX TO BE IS RECTANGULAR, BOX E IS RADIAL, CHECK ACCORDINGLY
            }
        }
    }
    isFree = !isOverlap;
    return isOverlap;
}

bool Collision::checkPointingIn(glm::vec3 disp){
    return glm::abs(fmath::smallestAngleFrom(glm::degrees(fmath::vectorToAngle(disp)), glm::degrees(fmath::vectorToAngle(normal)))) < 90.0;
}



bool Collision::staticOverlap(){
    
    //get comps of ent to be moved
    auto [transTB, boxTB] = sc->sRegistry.try_get<TransformComponent, BoxComponent>(etobDisplaced);
    isOverlap = false;
    if (transTB && boxTB){
        glm::vec2 callertoObj;
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
            
            
            if (boxTB->type == 'c' && boxE->type == 'c'){
//BOX TO BE AND BOX ENVIRONMENT ARE BOTH RADIAL, CHECK ACCORDINGLY
                bool cc = (glm::length((callerBoxCenter) - objBoxCenter) - boxE->rad - boxTB->rad) < 0;
                if (cc){
                    isOverlap = true;
                }
                //SET TAN
                
                tangent = fmath::angleToVector(collangle + (90));

            } else if (boxTB->type == 'c' && boxE->type == 's') {
//BOX TO BE IS RADIAL, BOX E IS RECTANGULAR, CHECK ACCORDINGLY
                auto objToCaller = (callertoObj * -1.0f);
                glm::vec3 objToCaller_r = glm::vec3(fmath::rotateVector(objToCaller, -transE->trans.rotation.z), 0.0);
                bool cs = ((glm::abs(objToCaller_r.x) <= ((boxE->size.x / 2.0) + (boxTB->rad / 1.41))) &&
                            (glm::abs(objToCaller_r.y) <= ((boxE->size.y / 2.0) + (boxTB->rad / 1.41))));

                if (cs){
                    isOverlap = true;
                }
            } else if   (boxTB->type == 's' && boxE->type == 's') {
//BOX TO BE AND BOX E ARE BOTH RECTANGULAR, CHECK ACCORDINGLY

            } else if (boxTB->type == 's' && boxE->type == 'c') {
//BOX TO BE IS RECTANGULAR, BOX E IS RADIAL, CHECK ACCORDINGLY
            }
        }
    }
    return isOverlap;
}


bool Collision::free(){
    if (!isOverlap){isFree = true;return isFree;}
    auto [transTB, dispTB, boxTB] = sc->sRegistry.try_get<TransformComponent, DisplaceComponent, BoxComponent>(etobDisplaced);
    auto [transE, boxE] = sc->sRegistry.try_get<TransformComponent, BoxComponent>(entwCollision);

    isFree = false;

    if (boxTB->type == 'c' && boxE->type == 'c'){


    } else if (boxTB->type == 'c' && boxE->type == 's') {
        
        auto objToCaller = (callertoObj * -1.0f) + glm::vec2(dispTB->displace.x, dispTB->displace.y);
        glm::vec3 objToCaller_r = glm::vec3(fmath::rotateVector(objToCaller, -transE->trans.rotation.z), 0.0);
        
        bool hittingWSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) <
        (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
//        bool hittingHSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) >
//        (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
        
        glm::vec2 normal;
        if (hittingWSide){
            if (objToCaller_r.x < 0){
                normal = glm::vec2(-1.0, 0.0);
                normal = fmath::rotateVector(normal, transE->trans.rotation.z);
            } else {
                normal = glm::vec2(1.0, 0.0);
                normal = fmath::rotateVector(normal, transE->trans.rotation.z);
            }
        } else {
            if (objToCaller_r.y < 0){
                normal = glm::vec2(0.0, -1.0);
                normal = fmath::rotateVector(normal, transE->trans.rotation.z);
            } else {
                normal = glm::vec2(0.0, 1.0);
                normal = fmath::rotateVector(normal, transE->trans.rotation.z);
            }
        }
        
        bool pointingAway = glm::abs(fmath::smallestAngleFrom(glm::degrees(fmath::vectorToAngle(dispTB->displace)), glm::degrees(fmath::vectorToAngle(normal)))) < 90.0;
        if (pointingAway){
            isFree = true;
        }
    } else if   (boxTB->type == 's' && boxE->type == 's') {

    } else if (boxTB->type == 's' && boxE->type == 'c') {
        
    }
    return isFree;
}

glm::vec3 Collision::deflect(){
    auto [transTB, dispTB, boxTB] = sc->sRegistry.try_get<TransformComponent, DisplaceComponent, BoxComponent>(etobDisplaced);
    auto [transE, boxE] = sc->sRegistry.try_get<TransformComponent, BoxComponent>(entwCollision);
//    cout << "rot of object being checked: " << transE->trans.rotation.z << endl;
    glm::vec3 V;
    float collangle = glm::degrees(fmath::vectorToAngle(glm::vec3(callertoObj, 0.0)));
    float dispangle = glm::degrees(fmath::vectorToAngle(dispTB->displace));
    float colltoDisp = fmath::smallestAngleFrom(collangle,
                                                dispangle);


    if (boxTB->type == 'c' && boxE->type == 'c'){
        int sign = 1;
        if (colltoDisp < 0) {sign = -1;}
        V = fmath::angleToVector(collangle + (90 * sign));
        float udotv = (dispTB->displace.x * V.x) + (dispTB->displace.y * V.y);
        float magvsq = glm::length(V) * glm::length(V);
        deflection = V * (udotv / magvsq);

    } else if (boxTB->type == 'c' && boxE->type == 's') {
        
        
        auto objToCaller = (callertoObj * -1.0f) + glm::vec2(dispTB->displace.x, dispTB->displace.y);
        glm::vec3 objToCaller_r = glm::vec3(fmath::rotateVector(objToCaller, -transE->trans.rotation.z), 0.0);
        
        bool hittingWSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) <
        (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
        bool hittingHSide = (((boxE->size.x / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.x)) >
        (((boxE->size.y / 2.0) + boxTB->rad)) - (glm::abs(objToCaller_r.y));
        
        if (hittingWSide){
            V = glm::vec3(0.0, (-2.0 * (objToCaller_r.x < 0)) + 1.0, 0.0);
            V = fmath::rotateVector(V, transE->trans.rotation.z);
//            cout << "V: " << V.x << ", " << V.y << endl;
            float udotv = (dispTB->displace.x * V.x) + (dispTB->displace.y * V.y);
//            deflection = V * udotv;
            deflection = fmath::project(dispTB->displace, V);
        } else if (hittingHSide){//sidehit > thresh){
            V = glm::vec3((-2.0 * (objToCaller_r.y < 0)) + 1.0, 0.0, 0.0);
            V = fmath::rotateVector(V, transE->trans.rotation.z);
//            cout << "V: " << V.x << ", " << V.y << endl;
            float udotv = (dispTB->displace.x * V.x) + (dispTB->displace.y * V.y);
            deflection = V * udotv;
        }
        

    } else if   (boxTB->type == 's' && boxE->type == 's') {

    } else if (boxTB->type == 's' && boxE->type == 'c') {
    }
    return deflection;
}





















//bool Collision::free(){
//    auto [transTB, dispTB, boxTB] = sc->sRegistry.try_get<TransformComponent, DisplaceComponent, BoxComponent>(etobDisplaced);
//    auto [transE, boxE] = sc->sRegistry.try_get<TransformComponent, BoxComponent>(entwCollision);
//
//
//    if (boxTB->type == 'c' && boxE->type == 'c'){
//
//
//    } else if (boxTB->type == 'c' && boxE->type == 's') {
//
//    } else if   (boxTB->type == 's' && boxE->type == 's') {
//
//    } else if (boxTB->type == 's' && boxE->type == 'c') {
//    }
//}


