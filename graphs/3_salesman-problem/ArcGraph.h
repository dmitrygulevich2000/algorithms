//
// Created by DmitryGulevich on 28.04.2019.
//

#ifndef SALESMAN_ARCGRAPH_H
#define SALESMAN_ARCGRAPH_H

#include "IGraph.h"

struct Edge {
    int from = 0;
    int to = 0;
    double weight = 0;

    Edge(int f, int t, double w): from(f), to(t), weight(w) {}
    Edge() = default;
    bool operator<(const Edge& e) const {
        return this->weight < e.weight;
    }
};


class ArcGraph : public IGraph {
public:
    ArcGraph(unsigned int count);
    ArcGraph(const IGraph& Graph);
    ~ArcGraph() override = default;

    void AddEdge(int from, int to, double weight) override;
    double FindWeight(int from ,int to) const override;
    void PrintEdges() const override;
    unsigned int VertCount() const override;

    std::vector<Edge> Edges;
private:
    unsigned int cntVert;

    void GetNextVert(int v, std::vector<std::pair<int, double>>& vert) const override;
};

#endif //SALESMAN_ARCGRAPH_H
