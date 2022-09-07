#pragma once
//  FTransform.h
//  gaspunk
//
//  Created by Frank Collebrusco on 5/28/22.
//

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <stdio.h>


#ifndef FTransform_h
#define FTransform_h

struct Transform{
    //rotation about x,y,z in order? DEGREES
    Transform();
    Transform(glm::vec3 p);
    Transform(glm::vec3 p, glm::vec3 s);
    Transform(glm::vec3 p, glm::vec3 s, glm::vec3 r);
    Transform(glm::vec3, glm::vec3, glm::vec3, glm::vec3);
    glm::vec3 pos;
    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec3 anchor;
};

#endif /* FTransform_h */
