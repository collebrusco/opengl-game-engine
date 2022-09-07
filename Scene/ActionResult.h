#pragma once
//  ActionResult.h
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//

#include <string>
#include <vector>
#include "Components.h"


//make this abstract? then diff types of results to return... eg one for add component(entity),
//add entity(trans), change other things etc.... templated? 

class ActionResult{
public:
    ActionResult();//string);
    ~ActionResult();
    void EmplaceUpdate(BaseComponent*);
//    std::string entityName;
    std::vector<BaseComponent*> updates;
};
