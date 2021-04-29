//
// Created by DmitryGulevich on 28.04.2019.
//

#ifndef SALESMAN_MATRIXGRAPH_H
#define SALESMAN_MATRIXGRAPH_H

#include"IGraph.h"

class MatrixGraph : public IGraph {
public:
    MatrixGraph(unsigned int count);
    MatrixGraph(const IGraph& Graph);
    ~MatrixGraph() override = default;

    void AddEdge(int from, int t, double weight) override;
    double FindWeight(int from ,int to) const override;
    void PrintEdges() const override;
    unsigned int VertCount() const override;
private:
    unsigned int cntVert;
    std::vector<std::vector<double>> Matrix;  // в ячейке [from][to] находится вес ребра from->to

    void GetNextVert(int v, std::vector<std::pair<int, double>>& vert) const override;
};



#endif //SALESMAN_MATRIXGRAPH_H
