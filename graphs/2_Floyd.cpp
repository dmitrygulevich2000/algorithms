#include <iostream>
#include<vector>


class IGraph {
public:
    virtual ~IGraph() = default;
    virtual void AddEdge(int from, int to, int weight) = 0;
    virtual unsigned int VertCount() const = 0;
};

class FullMatrixGraph : protected IGraph {
public:
    FullMatrixGraph(unsigned int count);
    ~FullMatrixGraph() override = default;

    void AddEdge(int from, int to, int weight) override;
    unsigned int VertCount() const override;
    int GetWeight(int from, int to);

    unsigned int cntVert;
    std::vector<std::vector<int>> AdjMatrix;  // в ячейке [from][to] лежит вес ребра (from; to)
};

FullMatrixGraph::FullMatrixGraph(unsigned int count) {
    cntVert = count;
    AdjMatrix.resize(count);
    for (int i = 0; i < count; ++i) {
        AdjMatrix[i].resize(count);
    }
}

void FullMatrixGraph::AddEdge(int from, int to, int weight) {
    AdjMatrix[from][to] = weight;
}

unsigned int FullMatrixGraph::VertCount() const {
    return cntVert;
}

int FullMatrixGraph::GetWeight(int from, int to) {
    return AdjMatrix[from][to];
}


std::vector<std::vector<int>> Floyd(const FullMatrixGraph& G) {
    auto res = G.AdjMatrix;
    for (int i = 0; i < G.VertCount(); ++i) {
        res[i][i] = 0;
    }

    for (int k = 0; k < G.VertCount(); ++k) {
        for (int u = 0; u < G.VertCount(); ++u) {
            for (int v= 0; v < G.VertCount(); ++v) {
                res[u][v] = std::min(res[u][v], res[u][k] + res[k][v]);
            }
        }
    }
    return res;
}


int main() {
    unsigned int n;
    std::cin >> n;
    FullMatrixGraph G(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < G.VertCount(); ++j) {
            int w;
            std::cin >> w;
            G.AddEdge(i, j, w);
        }
    }

    auto d = Floyd(G);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < G.VertCount(); ++j) {
            std::cout << d[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}