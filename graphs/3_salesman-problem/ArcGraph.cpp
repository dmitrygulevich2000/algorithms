//
// Created by DmitryGulevich on 28.04.2019.
//

#include "ArcGraph.h"

ArcGraph::ArcGraph(unsigned int count) {
    cntVert = count;
    Edges.resize(0);
}

ArcGraph::ArcGraph(const IGraph& Graph) {
    cntVert = Graph.VertCount();
    Edges.resize(cntVert);
    for (int i = 0; i < cntVert; ++i) {
        std::vector<std::pair<int, double>> tmp;
        Graph.GetNextVert(i, tmp);
        for (int j = 0; j < tmp.size(); ++j) {
            AddEdge(i, tmp[j].first, tmp[j].second);
        }
    }
}

void ArcGraph::AddEdge(int from, int to, double weight) {
    Edges.emplace_back(Edge(from, to, weight));
}

double ArcGraph::FindWeight(int from, int to) const {
    for(Edge i: Edges) {
        if (i.from == from && i.to == to) {
            return i.weight;
        }
    }
    return -1;
}

void ArcGraph::PrintEdges() const {
    for(Edge i: Edges) {
        std::cout << i.from << " <-> " << i.to << " | " << i.weight << std::endl;
    }
    std::cout << "-------------------" << std::endl;
}

unsigned int ArcGraph::VertCount() const {
    return cntVert;
}

void ArcGraph::GetNextVert(int v, std::vector<std::pair<int, double>>& vert) const {
    for (int i = 0; i < Edges.size(); ++i) {
        if (Edges[i].from == v) {
            vert.emplace_back(std::make_pair(Edges[i].to, Edges[i].weight));
        }
    }
}
