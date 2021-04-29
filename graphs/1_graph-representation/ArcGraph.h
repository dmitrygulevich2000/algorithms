//
// Created by DmitryGulevich on 28.02.2019.
//

#ifndef PREDSTAVLENIE_GRAFA_ARCGRAPH_H
#define PREDSTAVLENIE_GRAFA_ARCGRAPH_H

#include"IGraph.h"

class ArcGraph : protected IGraph {
public:
    ArcGraph(unsigned int count);
    ArcGraph(const IGraph *Graph);
    ~ArcGraph() override = default;

    void AddEdge(int from, int to) override;
    unsigned int VertCount() const override;
private:
    unsigned int cntVert;
    std::vector<std::pair<int, int>> Edges;

    void GetNextVert(int v, std::vector<int>& vert) const override;
    void GetPrevVert(int v, std::vector<int>& vert) const override;
};


#endif //PREDSTAVLENIE_GRAFA_ARCGRAPH_H
