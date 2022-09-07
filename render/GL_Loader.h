#pragma once
//  OpenGLLoader.h
//  GL Open
//
//  Created by Frank Collebrusco on 5/19/22.
//

#include "../Vertex.h"
#include <vector>

int UploadTexture(std::string name);

MeshDetails UploadMesh(const std::vector<Vertex>& verts, const std::vector<uint32_t> elem,
                              std::string type);

MeshDetails UploadTileMesh();

MeshDetails UploadTileOutlineVAO();



 void UnloadMesh(std::vector<MeshDetails>& details);
