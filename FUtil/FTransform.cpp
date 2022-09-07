//
//  FTransform.cpp
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/2/22.
//

#include "FTransform.h"


    //rotation about x,y,z in order? DEGREES
Transform::Transform(){
    pos = glm::vec3(0.0);
    scale = glm::vec3(1.0);
    rotation = pos;
    anchor = glm::vec3(0.0);
}
Transform::Transform(glm::vec3 p){
    pos = p;
    scale = glm::vec3(1.0);
    rotation = glm::vec3(0.0);
    anchor = glm::vec3(0.0);
}
Transform::Transform(glm::vec3 p, glm::vec3 s){
    pos = p;
    scale = s;
    rotation = glm::vec3(0.0);
    anchor = glm::vec3(0.0);
}
Transform::Transform(glm::vec3 p, glm::vec3 s, glm::vec3 r){
    pos = p;
    scale = s;
    rotation = r;
    anchor = glm::vec3(0.0);
}
Transform::Transform(glm::vec3 p, glm::vec3 s, glm::vec3 r, glm::vec3 a){
    pos = p;
    scale = s;
    rotation = r;
    anchor = a;
}

