/*
Author name: Donavan Sirois
Date: Febuary 1, 2026
File name: BSP.h
Description: Header file for the Binary Space Partitioning tree implementation.
Modifications:
*/

#ifndef BSP_H
#define BSP_H

#include<QDebug>

#include"geostructs.h"

struct Node
{
    Linedef partition;
    int frontSector;
    int backSector;
    Node* front;
    Node* back;
};

class BSP
{
public:
    BSP();
    Node* Builder(std::vector<Linedef> segments);
    void traverse(const Vertex& playerPosition, std::vector<Linedef>& renderedWalls);
    void traverseNode(Node* node, const Vertex& playerPos, std::vector<Linedef>& walls);
    void build(const std::vector<Linedef>& segments);
private:
    Node* root;
};

#endif
