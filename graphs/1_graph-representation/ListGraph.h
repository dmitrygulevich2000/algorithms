//
// Created by DmitryGulevich on 08.02.2019.
//

#ifndef PREDSTAVLENIE_GRAFA_LISTGRAPH_H
#define PREDSTAVLENIE_GRAFA_LISTGRAPH_H

#include"IGraph.h"
#include<list>

class ListGraph : protected IGraph {
public:
    ListGraph(unsigned int count);
    ListGraph(const IGraph *Graph);
    ~ListGraph() override = default;

    void AddEdge(int from, int to) override;
    unsigned int VertCount() const override;
private:
    unsigned int cntVert;
    std::vector<std::list<int>> InEdges, OutEdges;

    void GetNextVert(int v, std::vector<int>& vert) const override;
    void GetPrevVert(int v, std::vector<int>& vert) const override;
};


#endif //PREDSTAVLENIE_GRAFA_LISTGRAPH_H
