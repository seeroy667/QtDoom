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

struct Point2D
{
    float x, y;
};

struct Wall
{
    Point2D start;
    Point2D end;
    float floorHeight;
    float ceilingHeight;
};

struct Segment {
    Point2D start;
    Point2D end;
};

struct Plane {
    Point2D normal;
    float dist;
};

struct BSPNode {
    Plane splitter;
    BSPNode* front;
    BSPNode* back;
    std::vector<Segment> segments;
    bool isLeaf;
};

#endif // GEOSTRUCTS_H
