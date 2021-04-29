#include <iostream>
#include <vector>
#include <queue>
#include <limits>


class IGraph {
public:
    virtual ~IGraph() = default;
    virtual void AddEdge(int from, int to, int capacity) = 0;
    virtual unsigned int VertCount() const = 0;

    virtual void GetNextVert(int v, std::vector<int>& vert) const = 0;
    virtual void GetPrevVert(int v, std::vector<int>& vert) const = 0;

    virtual void setflow(int u, int v, int f) = 0;
    virtual int getflow(int u, int v) = 0;
    virtual int getcapacity(int u, int v) = 0;
    virtual void setcapacity(int u, int v, int c) = 0;
};

class MatrixGraph : public IGraph {
public:
    MatrixGraph(unsigned int count);
    MatrixGraph(const IGraph *Graph);
    ~MatrixGraph() override = default;

    void AddEdge(int from, int to, int capacity) override;
    unsigned int VertCount() const override;
    void setflow(int u, int v, int f) override;
    int getflow(int u, int v) override;
    int getcapacity(int u, int v) override;
    void setcapacity(int u, int v, int c) override;
private:
    unsigned int cntVert;
    std::vector<std::vector<std::pair<int, int>>> Matrix;
    // в ячейке (i; j) хранится пара <пропускная способность, поток> для ребра i->j

    void GetNextVert(int v, std::vector<int>& vert) const override;
    void GetPrevVert(int v, std::vector<int>& vert) const override;
};

MatrixGraph::MatrixGraph(unsigned int count) {
    cntVert = count;
    Matrix.resize(count);
    for (int i = 0; i < count; ++i) {
        Matrix[i].resize(count);
    }
}

MatrixGraph::MatrixGraph(const IGraph *Graph) {
    cntVert = Graph->VertCount();
    Matrix.resize(Graph->VertCount());
    for (int i = 0; i < Graph->VertCount(); ++i) {
        Matrix[i].resize(Graph->VertCount());
    }
    for (int i = 0; i < cntVert; ++i) {
        std::vector<int> tmp;
        Graph->GetNextVert(i, tmp);
        for (int j = 0; j < tmp.size(); ++j) {
            AddEdge(i, tmp[j], tmp[j]);
        }
    }
}

void MatrixGraph::AddEdge(int from, int to, int capacity) {
    Matrix[from][to] = std::make_pair(capacity, 0);
}

unsigned int MatrixGraph::VertCount() const {
    return cntVert;
}

void MatrixGraph::GetNextVert(int v, std::vector<int>& vert) const {
    for (int i = 0; i < cntVert; ++i) {
        if (Matrix[v][i].first > 0) {
            vert.push_back(i);
        }
    }
}

void MatrixGraph::GetPrevVert(int v, std::vector<int>& vert) const {
    for (int i = 0; i < cntVert; ++i) {
        if (Matrix[i][v].first > 0) {
            vert.push_back(i);
        }
    }
}

int MatrixGraph::getflow(int u, int v) {
    return Matrix[u][v].second;
}

void MatrixGraph::setflow(int u, int v, int f) {
    Matrix[u][v].second = f;
}

int MatrixGraph::getcapacity(int u, int v) {
    return Matrix[u][v].first;
}

void MatrixGraph::setcapacity(int u, int v, int c) {
    Matrix[u][v].first = c;
}

std::vector<int> layered_network(IGraph& G) {
    std::vector<int> dist(G.VertCount(), -1);
    std::queue<int> q;
    q.push(0);
    dist[0] = 0;

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        std::vector<int> vert;
        G.GetNextVert(curr, vert);
        for (int i = 0; i < vert.size(); ++i) {
            if (dist[vert[i]] == -1) {
                dist[vert[i]] = dist[curr] + 1;
                q.push(vert[i]);
            }
        }
    }
    return dist;
}

bool dfs1(IGraph& G, int v, int& min, int& flow,
          std::vector<int>& ptr, const std::vector<int>& dist) {
    if (v == G.VertCount() - 1) { // это сток
        flow += min;
        return true;
    }
    std::vector<int> vert;
    G.GetNextVert(v, vert);
    for (int j = ptr[v]; j < vert.size(); ++j) {
        int i = vert[j];
        if (dist[i] - dist[v] == 1) { // если ребро в слоистой сети
            int min1 = std::min(min, G.getcapacity(v, i) - G.getflow(v, i));
            if (min1 > 0 && dfs1(G, i, min1, flow, ptr, dist)) {
                min = min1;
                G.setflow(v, i, G.getflow(v, i) + min);
                G.setflow(i, v, G.getflow(i, v) - min);
                return true;
            } else {
                ptr[v]++; // через ребро v->i уже нельзя дойти до стока
            }
        }
    }
    return false;
}

int blocking_flow(IGraph& G) {
    std::vector<int> dist = layered_network(G);
    std::vector<int> ptr(G.VertCount(), 0);
    int flow = 0;
    int min = std::numeric_limits<int>::max();
    while (dfs1(G, 0, min, flow, ptr, dist)) {
        min = std::numeric_limits<int>::max();
    }  // 0 - исток
    return flow;
}

void residual_network(IGraph& G) {
    for (int i = 0; i < G.VertCount(); ++i) {
        for (int j = 0; j < G.VertCount(); ++j) {
            G.AddEdge(i, j, G.getcapacity(i, j) - G.getflow(i, j));
        }
    }
}

int Dinitsa(IGraph& G) {
    int max_flow = 0;
    int block = blocking_flow(G);
    while (block != 0) {
        max_flow += block;
        residual_network(G);

        block = blocking_flow(G);
    }
    return max_flow;
}

int main() {
    int V, E;
    std::cin >> V >> E;
    MatrixGraph G(V);
    for (int i = 0; i < E; ++i) {
        int from, to;
        long long capacity;
        std::cin >> from >> to >> capacity;
        G.AddEdge(from-1, to-1, capacity);
    }

    for (int a = 0; a < G.VertCount(); ++a) {
        for (int b = 0; b < G.VertCount(); ++b) {
            std::cout << G.getcapacity(a, b) << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------------" << std::endl;

    std::cout << Dinitsa(G) << std::endl;

    return 0;
}