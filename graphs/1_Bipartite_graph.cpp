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

class ListGraph: protected IGraph {
public:
    ListGraph(unsigned int count);
    ListGraph(const IGraph *Graph);
    ~ListGraph() override = default;

    void AddEdge(int from, int to) override;
    unsigned int VertCount() const override;
    bool IsBipartite();
private:
    unsigned int cntVert;
    std::vector<std::list<int>> Edges;

    void GetNextVert(int v, std::vector<int>& vert) const override;
    bool dfs(int v, std::vector<char>& colours);  // вернёт false, если выяснится, что граф не двудольный
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

bool ListGraph::dfs(int v, std::vector<char>& colours) {
    std::vector<int> vertices;
    GetNextVert(v, vertices);
    for (int i = 0; i < vertices.size(); ++i) {
        if (colours[vertices[i]] == colours[v]) {
            return false;
        }
        if (colours[vertices[i]] == 2) {  // 0 и 1 - цвета; 2 - вершина ещё не была посещена
            colours[vertices[i]] = 1 - colours[v];
            dfs(vertices[i], colours);
        }
    }
    return true;
}

bool ListGraph::IsBipartite() {
    std::vector<char> colours(cntVert, 2);
    for (int i = 0; i < cntVert; ++i) {
        if (colours[i] == 2) {
            colours[i] = 0;
            bool tmp = dfs(i, colours);
            if (!tmp) {
                return false;
            }
        }
    }
    return true;
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

    bool tmp = gr.IsBipartite();
    if (tmp) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }


    //system("pause");
    return 0;
}