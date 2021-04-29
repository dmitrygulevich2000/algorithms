//
// Created by DmitryGulevich on 08.02.2019.
//

#include "ListGraph.h"

ListGraph::ListGraph(unsigned int count) {
    cntVert = count;
    InEdges.resize(count);
    OutEdges.resize(count);
}

ListGraph::ListGraph(const IGraph *Graph) {
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

void ListGraph::AddEdge(int from, int to) {
    InEdges[to].push_front(from);
    OutEdges[from].push_front(to);
}

unsigned int ListGraph::VertCount() const {
    return cntVert;
}

void ListGraph::GetNextVert(int v, std::vector<int>& vert) const {
    for (std::list<int>::const_iterator it = OutEdges[v].cbegin(); it != OutEdges[v].cend(); it++) {
        vert.push_back(*it);
    }
}

void ListGraph::GetPrevVert(int v, std::vector<int>& vert) const {
    for (std::list<int>::const_iterator it = InEdges[v].cbegin(); it != InEdges[v].cend(); it++) {
        vert.push_back(*it);
    }
}