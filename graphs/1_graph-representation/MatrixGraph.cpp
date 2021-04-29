//
// Created by DmitryGulevich on 25.02.2019.
//

#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(unsigned int count) {
    cntVert = count;
    AdjMatrix.resize(count);
    for (int i = 0; i < count; ++i) {
        AdjMatrix[i].resize(count);
    }
}

MatrixGraph::MatrixGraph(const IGraph *Graph) {
    cntVert = Graph->VertCount();
    AdjMatrix.resize(cntVert);
    for (int i = 0; i < cntVert; ++i) {
        AdjMatrix[i].resize(cntVert);
    }
    for (int i = 0; i < cntVert; ++i) {
        std::vector<int> tmp;
        Graph->GetNextVert(i, tmp);
        for (int j = 0; j < tmp.size(); ++j) {
            AddEdge(i, j);
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    AdjMatrix[from][to] = true;
}

unsigned int MatrixGraph::VertCount() const {
    return cntVert;
}

void MatrixGraph::GetNextVert(int v, std::vector<int>& vert) const {
    for (int i = 0; i < cntVert; ++i) {
        if (AdjMatrix[v][i] == true) {
            vert.push_back(i);
        }
    }
}

void MatrixGraph::GetPrevVert(int v, std::vector<int>& vert) const {
    for (int i = 0; i < cntVert; ++i) {
        if (AdjMatrix[i][v] == true) {
            vert.push_back(i);
        }
    }
}
