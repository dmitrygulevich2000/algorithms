//
// Created by DmitryGulevich on 08.02.2019.
//

#ifndef PREDSTAVLENIE_GRAFA_IGRAPH_H
#define PREDSTAVLENIE_GRAFA_IGRAPH_H

#include<vector>

class IGraph {
public:
    virtual ~IGraph() {}
    virtual void AddEdge(int from, int to) = 0;
    virtual unsigned int VertCount() const = 0;
    virtual void GetNextVert(int v, std::vector<int>& vert) const = 0;
    virtual void GetPrevVert(int v, std::vector<int>& vert) const = 0;
};

#endif //PREDSTAVLENIE_GRAFA_IGRAPH_H
