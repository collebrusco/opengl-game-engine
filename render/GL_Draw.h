#pragma once
//#include <iostream>
//  GLDraw.h
//  workspace 2.0
//
//  Created by Frank Collebrusco on 5/21/22.
//
//#include "Vertex.h"
//#include "Object.h"

#include "../Vertex.h"
#include <vector>
using namespace std;

void DrawObjects(const vector<MeshDetails*> &objs);

void DrawMesh(MeshDetails* mesh);
