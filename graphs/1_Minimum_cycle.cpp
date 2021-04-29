#include<iostream>
#include<vector>
#include<queue>
#include<list>


class IGraph {
public:
    virtual ~IGraph() {}
    virtual void AddEdge(int from, int to) = 0;
    virtual unsigned int VertCount() const = 0;
    virtual void GetNextVert(int v, std::vector<int>& vert) const = 0;
};

class VecGraph: protected IGraph {
public:
    VecGraph(unsigned int count);
    VecGraph(const IGraph *Graph);
    ~VecGraph() override = default;

    void AddEdge(int from, int to) override;
    unsigned int VertCount() const override;
    int ShortestCycle();
private:
    unsigned int cntVert;
    std::vector<std::list<int>> Edges;

    void GetNextVert(int v, std::vector<int>& vert) const override;
    int BFS(int v); // возвращает длину кратчайшего цикла, включающего вершину v
};

VecGraph::VecGraph(unsigned int count) {
    cntVert = count;
    Edges.resize(cntVert);
}

VecGraph::VecGraph(const IGraph *Graph) {
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

void VecGraph::AddEdge(int from, int to) {
    Edges[to].push_front(from);
    Edges[from].push_front(to);
}

unsigned int VecGraph::VertCount() const {
    return cntVert;
}

void VecGraph::GetNextVert(int v, std::vector<int>& vert) const {
    for (std::list<int>::const_iterator it = Edges[v].cbegin(); it != Edges[v].cend(); it++) {
        vert.push_back(*it);
    }
}


int VecGraph::BFS(int v) {  // возвращает длину кратчайшего цикла, включающего вершину v
    std::queue<int> que;
    que.push(v);

    std::vector<int> wayslen(cntVert, 0);
    std::vector<char> used(cntVert, 0);
    used[v] = 1;
    std::vector<int> parents(cntVert, -1);

    while (!que.empty()) {

        std::vector<int> vertices;
        int tmp = que.front();
        que.pop();
        GetNextVert(tmp, vertices);

        for (int i = 0; i < vertices.size(); ++i) {
            parents[vertices[i]] = tmp;
            if (vertices[i] == tmp) {
                return 1;
            }
            if (used[vertices[i]] == 1 && vertices[i] != parents[tmp]) {
                return wayslen[vertices[i]] + wayslen[tmp] + 1;
            }
            if (!used[vertices[i]]) {
                used[vertices[i]] = 1;
                wayslen[vertices[i]] = wayslen[tmp] + 1;
                que.push(vertices[i]);
            }
        }
    }
    return -1;
}

int VecGraph::ShortestCycle() {
    int min = BFS(0);
    for (int i = 1; i < cntVert; ++i) {
        int tmp = BFS(i);
        if (tmp > 0 && (min < 0 || tmp < min)) {
            min = tmp;
        }
    }
    return min;
}


int main() {
    unsigned int n;
    int m;
    std::cin >> n >> m;

    VecGraph gr(n);
    int x = 0;
    int y = 0;
    for (int i = 0; i < m; ++i) {
        std::cin >> x >> y;
        gr.AddEdge(x, y);
    }
    std::cout << gr.ShortestCycle() << std::endl;

    //system("pause");
    return 0;
}