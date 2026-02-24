/*
Author: Donavan Sirois
Date: Febuary 13, 2026
File name: geoStructs.h
Goal: Header file for defining geomtry structures used for the map and the renderer.
Modifications:
*/

#ifndef GEOSTRUCTS_H
#define GEOSTRUCTS_H

#include<vector>

struct Vertex // Think of it as a 2D point
{
    float x, y;
};

struct Linedef // Think of it as a segment or a line
{
    Vertex start;
    Vertex end;
    float floorHeight;
    float ceilingHeight;
}; // TO BE CHANGED IF EVER CHANGING TO DOOM FORMAL STRUCTURES

struct Sidedef // Think of it as one side of the segment or the line
{
    int sectorRef;
}; // NOT USED YET, TO BE USED IF EVER CHANGING TO DOOM FORMAL STRUCTURES

struct Sector // Think of it as a polygon
{
    std::vector<int> linedefs;
    float floorHeight;
    float ceilingHeight;
};

#endif // NOT USED YET, TO BE USED IF EVER CHANGING TO DOOM FORMAL STRUCTURES
