//
//  ActionResult.cpp
//  gaspunk 2.0
//
//  Created by Frank Collebrusco on 6/1/22.
//

#include "ActionResult.h"

ActionResult::ActionResult(){//std::string ename){
//    entityName = ename;
}

ActionResult::~ActionResult(){
//    for (int i = 0; i < updates.size(); i++){
//        delete updates.at(i);
//    }
//    updates.clear();
}

void ActionResult::EmplaceUpdate(BaseComponent* cUpdate){
    updates.push_back(cUpdate);
}
