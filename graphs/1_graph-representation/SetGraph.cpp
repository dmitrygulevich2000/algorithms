//
// Created by DmitryGulevich on 28.02.2019.
//

#include "SetGraph.h"


SetGraph::SetGraph(unsigned int count) {
    cntVert = count;
    InEdges.resize(count);
    OutEdges.resize(count);
}

SetGraph::SetGraph(const IGraph *Graph) {
    cntVert = Graph->VertCount();
    InEdges.resize(cntVert);
    InEdges.resize(cntVert);
    for (int i = 0; i < cntVert; ++i) {
        std::vector<int> tmp;
        Graph->GetNextVert(i, tmp);
        for (int j = 0; j < tmp.size(); ++j) {
            AddEdge(i, j);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    InEdges[to].insert(from);
    OutEdges[from].insert(to);
}

unsigned int SetGraph::VertCount() const {
    return cntVert;
}

void SetGraph::GetNextVert(int v, std::vector<int>& vert) const {
    for (auto it = OutEdges[v].cbegin(); it != OutEdges[v].cend(); it++) {
        vert.push_back(*it);
    }
}

void SetGraph::GetPrevVert(int v, std::vector<int>& vert) const {
    for (auto it = InEdges[v].cbegin(); it != InEdges[v].cend(); it++) {
        vert.push_back(*it);
    }
}