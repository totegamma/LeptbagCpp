#ifndef PRIMITIVESHAPE_HPP
#define PRIMITIVESHAPE_HPP

#include <memory>

#include "vertexManager.hpp"
#include "elementManager.hpp"

extern std::vector<vertex> cubeShapeVertices;
extern std::vector<vertex> planeShapeVertices;

void initPrimitives();

elementManager* getCubeshape();
elementManager* getPlaneshape();


#endif
