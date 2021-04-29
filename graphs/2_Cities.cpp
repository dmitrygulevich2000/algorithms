#include <iostream>
#include<vector>
#include<set>
#include<unordered_map>


class IGraph {
public:
    virtual ~IGraph() {}
    virtual void AddEdge(int from, int to, int weight) = 0;
    virtual unsigned int VertCount() const = 0;
    virtual void GetNextVert(int v, std::vector<std::pair<int, int>>& vert) const = 0;
};

class ListGraph : public IGraph {
public:
    ListGraph(unsigned int count);
    ~ListGraph() override = default;

    void AddEdge(int from, int to, int weight) override;
    unsigned int VertCount() const override;
private:
    unsigned int cntVert;
    std::vector<std::vector<std::pair<int, int>>> Edges;

    void GetNextVert(int v, std::vector<std::pair<int, int>>& vert) const override;
};

ListGraph::ListGraph(unsigned int count) {
    cntVert = count;
    Edges.resize(count);
}

//ListGraph::ListGraph(const IGraph *Graph) {
//    cntVert = Graph->VertCount();
//    InEdges.resize(cntVert);
//    InEdges.resize(cntVert);
//    for (int i = 0; i < cntVert; ++i) {
//        std::vector<int> tmp;
//        Graph->GetNextVert(i, tmp);
//        for (int j = 0; j < tmp.size(); ++j) {
//            AddEdge(i, j);
//        }
//    }
//}

void ListGraph::AddEdge(int from, int to, int weight) {
    Edges[to].push_back(std::make_pair(from, weight));
    Edges[from].push_back(std::make_pair(to, weight));
}

unsigned int ListGraph::VertCount() const {
    return cntVert;
}

void ListGraph::GetNextVert(int v, std::vector<std::pair<int, int>>& vert) const {
    for (auto it = Edges[v].cbegin(); it != Edges[v].cend(); it++) {
        vert.push_back(*it);
    }
}


int Dijkstra(IGraph& G, int s, int t) {  // считает кратчайший путь от s до t
    std::set<std::pair<int, int>> pr_queue;  // хранит пары (приоритет; вершина)
    pr_queue.insert(std::make_pair(0, s));

    std::vector<int> d(G.VertCount(), -1);
    d[s] = 0;

    while (!pr_queue.empty()) {
        std::pair<int, int> curr = *(pr_queue.begin());
        pr_queue.erase(pr_queue.begin());
        if (curr.second == t) {
            return curr.first;
        }
        std::vector<std::pair<int, int>> vert;
        G.GetNextVert(curr.second, vert);
        for (int i = 0; i < vert.size(); ++i) {
            int d1 = curr.first + vert[i].second;
            if (d[vert[i].first] == -1) {
                d[vert[i].first] = d1;
                pr_queue.insert(std::make_pair(d[vert[i].first], vert[i].first));
            } else if (d[vert[i].first] > d1) {
                pr_queue.erase(std::make_pair(d[vert[i].first], vert[i].first));
                d[vert[i].first] = std::min(d[vert[i].first], d1);
                pr_queue.insert(std::make_pair(d[vert[i].first], vert[i].first));
            }
        }
    }
}


int main() {
    unsigned int V;
    int E;
    std::cin >> V;
    std::cin >> E;

    ListGraph G(V);
    for (int i = 0; i < E; ++i) {
        int f, t, w;
        std::cin >> f >> t >> w;
        G.AddEdge(f, t, w);
    }
    int s;
    int t;
    std::cin >> s;
    std::cin >> t;

    std::cout << Dijkstra(G, s, t);

    return 0;
}