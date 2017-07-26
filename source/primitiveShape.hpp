#ifndef PRIMITIVESHAPE_HPP
#define PRIMITIVESHAPE_HPP


#include "vertexManager.hpp"
#include "elementManager.hpp"

extern std::vector<vertex> cubeShapeVertices;
extern std::vector<vertex> planeShapeVertices;

elementManager& getCubeshape();
elementManager& getPlaneshape();


#endif
