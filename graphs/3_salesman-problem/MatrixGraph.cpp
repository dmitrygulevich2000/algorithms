//
// Created by DmitryGulevich on 28.04.2019.
//

#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(unsigned int count) {
    cntVert = count;
    Matrix.resize(count);
    for (int i = 0; i < count; ++i) {
        Matrix[i].resize(count);
    }
}

MatrixGraph::MatrixGraph(const IGraph& Graph) {
    cntVert = Graph.VertCount();
    Matrix.resize(cntVert);
    for (int i = 0; i < cntVert; ++i) {
        Matrix[i].resize(cntVert);
    }
    for (int i = 0; i < cntVert; ++i) {
        std::vector<std::pair<int, double>> tmp;
        Graph.GetNextVert(i, tmp);
        for (int j = 0; j < tmp.size(); ++j) {
            AddEdge(i, tmp[j].first, tmp[j].second);
        }
    }
}

void MatrixGraph::AddEdge(int from, int to, double weight) {
    Matrix[from][to] = weight;
    Matrix[to][from] = weight;
}

unsigned int MatrixGraph::VertCount() const {
    return cntVert;
}

double MatrixGraph::FindWeight(int from, int to) const {
    return Matrix[from][to];
}

void MatrixGraph::PrintEdges() const {
    for (auto& i: Matrix) {
        for (auto j: i) {
            std::cout << j << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << "---------------------------" << std::endl;
}

void MatrixGraph::GetNextVert(int v, std::vector<std::pair<int, double>>& vert) const {
    for (int i = 0; i < cntVert; ++i) {
        if (Matrix[v][i] > 0) {
            vert.emplace_back(std::make_pair(i, Matrix[v][i]));
        }
    }
}