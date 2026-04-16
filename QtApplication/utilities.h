/*
Author: Donavan Sirois
Date: April 7, 2026
File name: utilities.cpp
Description: Code for certain methods used throughout the project.
Modifications:
*/

#ifndef UTILITIES_H
#define UTILITIES_H

#include"geoStructs.h"

// Used to compute a cross product between a point and a line.
float crossProduct(const Vertex& v1, const Vertex& v2, const Vertex& v3);

// Used to compute a cross product between two already defined lines.
float crossProduct(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4);

#endif // UTILITIES_H
