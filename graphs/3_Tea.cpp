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
    virtual int getflow(int u, int v) const = 0;
    virtual int getcapacity(int u, int v) const = 0;
    virtual void setcapacity(int u, int v, int c) = 0;
};

class MatrixGraph : public IGraph {
public:
    MatrixGraph(unsigned int count);
    MatrixGraph(const IGraph& Graph);
    ~MatrixGraph() override = default;

    void AddEdge(int from, int to, int capacity) override;
    unsigned int VertCount() const override;
    void setflow(int u, int v, int f) override;
    int getflow(int u, int v) const override;
    int getcapacity(int u, int v) const override;
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

MatrixGraph::MatrixGraph(const IGraph& Graph) {
    cntVert = Graph.VertCount();
    Matrix.resize(Graph.VertCount());
    for (int i = 0; i < Graph.VertCount(); ++i) {
        Matrix[i].resize(Graph.VertCount());
    }
    for (int i = 0; i < cntVert; ++i) {
        std::vector<int> vert;
        Graph.GetNextVert(i, vert);
        for (int j = 0; j < vert.size(); ++j) {
            AddEdge(i, vert[j], Graph.getcapacity(i, vert[j]));
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

int MatrixGraph::getflow(int u, int v) const {
    return Matrix[u][v].second;
}

void MatrixGraph::setflow(int u, int v, int f) {
    Matrix[u][v].second = f;
}

int MatrixGraph::getcapacity(int u, int v) const {
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
            G.setcapacity(i, j, G.getcapacity(i, j) - G.getflow(i, j));
            G.setflow(i, j, 0);
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

int how_many_days(IGraph& G, int stuff, int end, const std::vector<std::vector<int>>& wishes) {
    int begin = 0;
    while (end - begin > 1) {
        int m = (begin + end)/2;

        MatrixGraph G1(G);  // скопируем граф и обновим веса рёбер от истока в сотрудников и от сотрудников к сортам
        for (int i = 0; i < stuff; ++i) {
            for (int j = 0; j < wishes[i].size(); ++j) {
                G1.setcapacity(1 + i, 1 + stuff + wishes[i][j], m);
            }
            G1.setcapacity(0, 1+i, m);
        }

        int flow = Dinitsa(G1);
        if (flow < m * stuff) {
            end = m;
        } else {
            begin = m;
        }
    }
    return begin;
}

int main() {
    int tea, stuff;
    std::cin >> stuff >> tea;
    MatrixGraph G(stuff + tea + 2);
    int end = 0; // посчитаем количество пакетиков
    for (int i = 0; i < tea; ++i) {
        int sort;
        std::cin >> sort; // это количество пакетиков i-го сорта
        end += sort;
        G.AddEdge(1 + stuff + i, G.VertCount() - 1, sort); // добавляем рёбра от сортов чая к стоку
    }


    std::vector<std::vector<int>> wishes(stuff);

    for (int i = 0; i < stuff; ++i) {
        int count;
        std::cin >> count;
        for (int j = 0; j < count; ++j) {
            int sort;
            std::cin >> sort; // это один из любимых сортов i-го сотрудника
            wishes[i].push_back(sort - 1);
            G.AddEdge(1 + i, 1 + stuff + sort - 1, 0);
        }
        G.setcapacity(0, 1 + i, 0);
    }

    std::cout << how_many_days(G, stuff, end/stuff + 1, wishes);

    return 0;
}
