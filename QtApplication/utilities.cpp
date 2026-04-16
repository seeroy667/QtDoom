/*
Author: Donavan Sirois
Date: April 7, 2026
File name: utilities.cpp
Description: Code for certain methods used throughout the project.
Modifications:
*/

#include"utilities.h"

// Description: Computes the cross product between a segment (first two verteces) and a vertex. Returns the cross product
float crossProduct(const Vertex& segmentV1, const Vertex& segmentV2, const Vertex& vertex)
{
    float dxSeg = segmentV1.x - segmentV2.x;
    float dySeg = segmentV1.y - segmentV2.y;
    float dxVertex = segmentV1.x - vertex.x;
    float dyVertex = segmentV1.y - vertex.y;

    return (dxSeg * dyVertex) - (dySeg * dxVertex);
}

float crossProduct(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4)
{
    float dxSeg1 = v1.x - v2.x;
    float dySeg1 = v1.y - v2.y;
    float dxSeg2 = v3.x - v4.x;
    float dySeg2 = v3.y - v4.y;

    return (dxSeg1 * dySeg2) - (dySeg1 * dxSeg2);
}

float vectorMagnitude(const Vertex& v1, const Vertex& v2)
{
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;

    return sqrt(dx*dx + dy*dy);
}
