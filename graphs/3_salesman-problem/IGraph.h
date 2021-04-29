//
// Created by DmitryGulevich on 28.04.2019.
//

#ifndef SALESMAN_IGRAPH_H
#define SALESMAN_IGRAPH_H

#include <vector>
#include <iostream>

class IGraph {
public:
    virtual ~IGraph() = default;
    virtual void AddEdge(int from, int to, double weight) = 0;
    virtual double FindWeight(int from, int to) const = 0;
    virtual void PrintEdges() const = 0;
    virtual unsigned int VertCount() const = 0;
    virtual void GetNextVert(int v, std::vector<std::pair<int, double>>& vert) const = 0;
};

#endif //SALESMAN_IGRAPH_H
