//
// Created by DmitryGulevich on 28.02.2019.
//

#ifndef PREDSTAVLENIE_GRAFA_SETGRAPH_H
#define PREDSTAVLENIE_GRAFA_SETGRAPH_H

#include"IGraph.h"
#include<unordered_set>

class SetGraph : protected IGraph {
public:
    SetGraph(unsigned int count);
    SetGraph(const IGraph *Graph);
    ~SetGraph() override = default;

    void AddEdge(int from, int to) override;
    unsigned int VertCount() const override;
private:
    unsigned int cntVert;
    std::vector<std::unordered_set<int>> InEdges, OutEdges;

    void GetNextVert(int v, std::vector<int>& vert) const override;
    void GetPrevVert(int v, std::vector<int>& vert) const override;
};

#endif //PREDSTAVLENIE_GRAFA_SETGRAPH_H
