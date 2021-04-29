//
// Created by DmitryGulevich on 28.04.2019.
//

#include "ListGraph.h"

ListGraph::ListGraph(unsigned int count) {
    cntVert = count;
    Edges.resize(count);
}

ListGraph::ListGraph(const IGraph *Graph) {
    cntVert = Graph->VertCount();
    Edges.resize(cntVert);
    for (int i = 0; i < cntVert; ++i) {
        std::vector<std::pair<int, double>> tmp;
        Graph->GetNextVert(i, tmp);
        for (int j = 0; j < tmp.size(); ++j) {
            AddEdge(i, tmp[j].first, tmp[j].second);
        }
    }
}

void ListGraph::AddEdge(int from, int to, double weight) {
    Edges[to].push_front(std::make_pair(from, weight));
}

double ListGraph::FindWeight(int from, int to) const {
    for (std::pair<int, int> i: Edges[from]) {
        if (i.first == to) {
            return i.second;
        }
    }
    return -1;
}

void ListGraph::PrintEdges() const {
    for (int i = 0; i < Edges.size(); ++i) {
        for (std::pair<int, int> j: Edges[i]) {
            std::cout << i << " <-> " << j.first << " | " << j.second << std::endl;
        }
    }

}

unsigned int ListGraph::VertCount() const {
    return cntVert;
}

void ListGraph::GetNextVert(int v, std::vector<std::pair<int, double>>& vert) const {
    for (std::pair<int, int> i: Edges[v]) {
        vert.emplace_back(std::make_pair(i.first, i.second));
    }
}
