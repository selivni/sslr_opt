#pragma once
#include <list>

#include "Utility.h"
//GLuint - VAO, uint - mesh vertices count(not anymore I guess?)
typedef std::pair<GLuint, unsigned int> MeshInfo;

typedef std::list<MeshInfo> VAOs;
//GLuint - texture index; VAOs - meshes' VAOs
//typedef std::pair<GLuint[2], VAOs> MaterialInfo;
//1 - ambient texture index, 2 - opacity texture index

typedef std::list<std::vector<unsigned int>> TrianglesIndices;
//Basically faces info
