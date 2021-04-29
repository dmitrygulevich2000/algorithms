//
// Created by DmitryGulevich on 28.02.2019.
//

#include "ArcGraph.h"


ArcGraph::ArcGraph(unsigned int count) {
    cntVert = count;
    Edges.resize(count);
}

ArcGraph::ArcGraph(const IGraph *Graph) {
    cntVert = Graph->VertCount();
    Edges.resize(cntVert);
    for (int i = 0; i < cntVert; ++i) {
        std::vector<int> tmp;
        Graph->GetNextVert(i, tmp);
        for (int j = 0; j < tmp.size(); ++j) {
            AddEdge(i, j);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    Edges.push_back(std::make_pair(from, to));
}

unsigned int ArcGraph::VertCount() const {
    return cntVert;
}

void ArcGraph::GetNextVert(int v, std::vector<int>& vert) const {
    for (int i = 0; i < Edges.size(); ++i) {
        if (Edges[i].first == v) {
            vert.push_back(Edges[i].second);
        }
    }
}

void ArcGraph::GetPrevVert(int v, std::vector<int>& vert) const {
    for (int i = 0; i < Edges.size(); ++i) {
        if (Edges[i].second == v) {
            vert.push_back(Edges[i].first);
        }
    }
}