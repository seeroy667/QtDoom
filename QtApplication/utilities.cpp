/*
Author: Donavan Sirois
Date: April 7, 2026
File name: utilities.cpp
Description: Code for certain methods used throughout the project.
Modifications:
*/

#include"utilities.h"

// Description: Computes the cross product between a segment (first two verteces) and a vertex. Returns the cross product
float crossProduct(Vertex& segmentV1, Vertex& segmentV2, Vertex& vertex)
{
    float dxSeg = segmentV1.x - segmentV2.x;
    float dySeg = segmentV1.y - segmentV2.y;
    float dxVertex = segmentV1.x - vertex.x;
    float dyVertex = segmentV1.y - vertex.y;

    return (dxSeg * dyVertex) - (dySeg * dxVertex);
}
