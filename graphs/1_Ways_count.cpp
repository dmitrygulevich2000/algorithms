#include <iostream>
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

class ListGraph: protected IGraph {
public:
    ListGraph(unsigned int count);
    ListGraph(const IGraph *Graph);
    ~ListGraph() override = default;

    void AddEdge(int from, int to) override;
    unsigned int VertCount() const override;
    int ShortestWay(int from, int to);
private:
    unsigned int cntVert;
    std::vector<std::list<int>> Edges;

    void GetNextVert(int v, std::vector<int>& vert) const override;
};

ListGraph::ListGraph(unsigned int count) {
    cntVert = count;
    Edges.resize(count);
}

ListGraph::ListGraph(const IGraph *Graph) {
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

void ListGraph::AddEdge(int from, int to) {
    Edges[to].push_front(from);
    Edges[from].push_front(to);
}

unsigned int ListGraph::VertCount() const {
    return cntVert;
}

void ListGraph::GetNextVert(int v, std::vector<int>& vert) const {
    for (std::list<int>::const_iterator it = Edges[v].cbegin(); it != Edges[v].cend(); it++) {
        vert.push_back(*it);
    }
}

int ListGraph::ShortestWay(int from, int to) {
    std::queue<int> que;
    std::vector<int> ways_count(cntVert, 0);
    std::vector<int> ways_lengths(cntVert, 0);

    que.push(from);
    ways_count[from] = 1;

    while(!que.empty()) {
        std::vector<int> vertices;
        int tmp = que.front();
        que.pop();
        GetNextVert(tmp, vertices);

        for (int i = 0; i < vertices.size(); ++i) {
            if (ways_count[vertices[i]] == 0) {
                que.push(vertices[i]);
                ways_lengths[vertices[i]] = ways_lengths[tmp] + 1;
                ways_count[vertices[i]] = ways_count[tmp];
            } else
            if (ways_lengths[vertices[i]] > ways_lengths[tmp]) {
                ways_count[vertices[i]] += ways_count[tmp];
            }
        }
    }
    return ways_count[to];
}

int main() {
    unsigned int v;
    std::cin >> v;
    ListGraph gr(v);
    int n;
    std::cin >> n;

    int x = 0;
    int y = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> x;
        std::cin >> y;
        gr.AddEdge(x, y);
    }
    std::cin >> x;
    std::cin >> y;

    std::cout << gr.ShortestWay(x, y) << std::endl;


    //system("pause");
    return 0;
}