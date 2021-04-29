#include <iostream>
#include<vector>


class IGraph {
public:
    virtual ~IGraph() {}
    virtual void AddEdge(int from, int to, double weight) = 0;
    virtual unsigned int VertCount() const = 0;
    virtual void GetNextVert(int v, std::vector<std::pair<int, double>>& vert) const = 0;
};

class ListGraph : public IGraph {
public:
    ListGraph(unsigned int count);
    ~ListGraph() override = default;

    void AddEdge(int from, int to, double weight) override;
    unsigned int VertCount() const override;
private:
    unsigned int cntVert;
    std::vector<std::vector<std::pair<int, double>>> InEdges, OutEdges;

    void GetNextVert(int v, std::vector<std::pair<int, double>>& vert) const override;
};

ListGraph::ListGraph(unsigned int count) {
    cntVert = count;
    InEdges.resize(count);
    OutEdges.resize(count);
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

void ListGraph::AddEdge(int from, int to, double weight) {
    InEdges[to].push_back(std::make_pair(from, weight));
    OutEdges[from].push_back(std::make_pair(to, weight));
}

unsigned int ListGraph::VertCount() const {
    return cntVert;
}

void ListGraph::GetNextVert(int v, std::vector<std::pair<int, double>>& vert) const {
    for (auto it = OutEdges[v].cbegin(); it != OutEdges[v].cend(); it++) {
        vert.push_back(*it);
    }
}

bool ArbitrageFordBellman(IGraph& G) {  // вернет true, если будет найден отрицательный цикл
    std::vector<double> d(G.VertCount(), -1);
    d[0] = 1;
    for (int k = 0; k <= G.VertCount(); ++k) {
        for (int u = 0; u < G.VertCount(); ++u) {
            std::vector<std::pair<int, double>> vert;
            G.GetNextVert(u, vert);
            for (int i = 0; i < vert.size(); ++i) {
                int v = vert[i].first;
                if (d[u] != -1) {
                    if (d[v] < d[u]*vert[i].second) {
                        d[v] = d[u]*vert[i].second;
                        if (k == G.VertCount()) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}


int main() {
    unsigned int n;
    std::cin >> n;
    ListGraph G(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                double w;
                std::cin >> w;
                if (w != -1) {
                    G.AddEdge(i, j, w);
                }
            }
        }
    }

    if (ArbitrageFordBellman(G)) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }

    return 0;
}