//
// Created by DmitryGulevich on 25.02.2019.
//

#ifndef PREDSTAVLENIE_GRAFA_MATRIXGRAPH_H
#define PREDSTAVLENIE_GRAFA_MATRIXGRAPH_H

#include"IGraph.h"

class MatrixGraph : protected IGraph {
public:
    MatrixGraph(unsigned int count);
    MatrixGraph(const IGraph *Graph);
    ~MatrixGraph() override = default;

    void AddEdge(int from, int to) override;
    unsigned int VertCount() const override;
private:
    unsigned int cntVert;
    std::vector<std::vector<bool>> AdjMatrix;  // в ячейке [from][to] содержит информацию о наличии ребра from->to

    void GetNextVert(int v, std::vector<int>& vert) const override;
    void GetPrevVert(int v, std::vector<int>& vert) const override;
};


#endif //PREDSTAVLENIE_GRAFA_MATRIXGRAPH_H
