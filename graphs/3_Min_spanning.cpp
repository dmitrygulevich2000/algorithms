#include <iostream>
#include <vector>
#include <algorithm>

struct Edge {
    int from = 0;
    int to = 0;
    long long weight = 0;

    Edge(int f, int t, long long w): from(f), to(t), weight(w) {}
    Edge() = default;
    bool operator<(const Edge& e) const {
        return this->weight < e.weight;
    }
};

class IGraph {
public:
    virtual ~IGraph() = default;
    virtual void AddEdge(int from, int to, long long weight) = 0;
    virtual unsigned int VertCount() const = 0;
    virtual void GetNextVert(int v, std::vector<int>& vert) const = 0;
    virtual void GetPrevVert(int v, std::vector<int>& vert) const = 0;
};

class ArcGraph : protected IGraph {
public:
    ArcGraph(unsigned int count);
    ~ArcGraph() override = default;

    void AddEdge(int from, int to, long long weight) override;
    unsigned int VertCount() const override;

    unsigned int cntVert;
    std::vector<Edge> Edges;
private:
    void GetNextVert(int v, std::vector<int>& vert) const override;
    void GetPrevVert(int v, std::vector<int>& vert) const override;
};

ArcGraph::ArcGraph(unsigned int count) {
    cntVert = count;
    Edges.resize(0);
}

void ArcGraph::AddEdge(int from, int to, long long weight) {
    Edges.emplace_back(Edge(from, to, weight));
}

unsigned int ArcGraph::VertCount() const {
    return cntVert;
}

void ArcGraph::GetNextVert(int v, std::vector<int>& vert) const {
    for (int i = 0; i < Edges.size(); ++i) {
        if (Edges[i].from == v) {
            vert.push_back(Edges[i].to);
        }
    }
}

void ArcGraph::GetPrevVert(int v, std::vector<int>& vert) const {
    for (int i = 0; i < Edges.size(); ++i) {
        if (Edges[i].to == v) {
            vert.push_back(Edges[i].from);
        }
    }
}


struct DSU {
    std::vector<int> parent;
    std::vector<int> rank; // буду обращаться только к рангу, хранящемуся в индексах представителей
    int count = 0;

    DSU(int count): parent(count, -1), rank(count, 1), count(count) {}
    DSU() = default;

    int Find(int u);
    bool Union(int u, int v);
};

int DSU::Find(int u) {
    //std::cout << "find( " << u << " ) " << std::endl;
    if (parent[u] != -1) {
        int res = Find(parent[u]);
        parent[u] = res;
        return res;
    } else {
        return u;
    }
}

bool DSU::Union(int u, int v) {
    int u_main = Find(u);
    int v_main = Find(v);
    if (u_main != v_main) {
        if (rank[u_main] < rank[v_main]) {
            parent[u_main] = v_main;
            count -= 1;
        } else {
            parent[v_main] = u_main;
            count -= 1;
            if (rank[u_main] == rank[v_main]) {
                rank[u_main]++; // достаточно поменять ранг для представителя полученного объединения
            }
        }
        return true;
    }
    return false;
}

long long Kruskal(ArcGraph& G) {
    if (G.VertCount() <= 1) {
        return 0;
    }
    DSU dsu(G.VertCount());

    std::sort(G.Edges.begin(), G.Edges.end());

    long long result = 0;
    for (int i = 0; i < G.Edges.size(); ++i) {
        if (dsu.Union(G.Edges[i].from, G.Edges[i].to)) {
            result += G.Edges[i].weight;
        }
        if (dsu.count == 1) {
            return result;
        }
    }
}

int main() {
    int V, E = 0;
    std::cin >> V >> E;
    ArcGraph G(V);
    for (int i = 0; i < E; ++i) {
        int from, to;
        long long weight;
        std::cin >> from >> to >> weight;
        G.AddEdge(from - 1, to - 1, weight);
    }
    std::cout << Kruskal(G) << std::endl;

    return 0;
}