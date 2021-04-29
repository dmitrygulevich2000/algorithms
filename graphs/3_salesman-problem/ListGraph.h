//
// Created by DmitryGulevich on 28.04.2019.
//

#ifndef SALESMAN_LISTGRAPH_H
#define SALESMAN_LISTGRAPH_H

#include"IGraph.h"
#include<list>

class ListGraph : public IGraph {
public:
    ListGraph(unsigned int count);
    ListGraph(const IGraph *Graph);
    ~ListGraph() override = default;

    void AddEdge(int from, int to, double weight) override;
    double FindWeight(int from ,int to) const override;
    void PrintEdges() const override;
    unsigned int VertCount() const override;
private:
    unsigned int cntVert;
    std::vector<std::list<std::pair<int, double>>> Edges;

    void GetNextVert(int v, std::vector<std::pair<int, double>>& vert) const override;
};


#endif //SALESMAN_LISTGRAPH_H
