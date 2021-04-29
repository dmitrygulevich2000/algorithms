#include<iostream>
#include<vector>
#include<queue>
#include<list>
#include<cassert>

class IGraph {
public:
    virtual ~IGraph() {}
    virtual void AddEdge(int from, int to) = 0;
    virtual void AddVertex() = 0;
    virtual unsigned int VertCount() const = 0;
    virtual void Reverse() = 0;

    virtual void GetNextVert(int v, std::vector<int>& vert) const = 0;
    virtual void GetPrevVert(int v, std::vector<int>& vert) const = 0;
};

class ListGraph : public IGraph {
public:
    ListGraph(unsigned int count);
    ListGraph(const IGraph *Graph);
    ~ListGraph() override = default;

    void AddEdge(int from, int to) override;
    unsigned int VertCount() const override;
    void AddVertex() override;
    void Reverse() override; // заменяет граф на транспонированный
private:
    unsigned int cntVert;
    std::vector<std::list<int>> InEdges, OutEdges;

    void GetNextVert(int v, std::vector<int>& vert) const override;
    void GetPrevVert(int v, std::vector<int>& vert) const override;
};

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

void ListGraph::AddVertex() {
    ++cntVert;
    InEdges.push_back(std::list<int>());
    OutEdges.push_back(std::list<int>());

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

void ListGraph::Reverse() {
    std::swap(InEdges, OutEdges);
}


void dfs1(IGraph* G, int v, std::vector<char>& used, std::vector<int>& Outs) { // dfs для подсчёта информации о выходе из вершин
    std::vector<int> vert;
    G->GetNextVert(v, vert);
    for (int i = 0; i < vert.size(); ++i) {
        if (used[vert[i]] == 0) {
            used[vert[i]] = 1;
            dfs1(G, vert[i], used, Outs);
        }
    }
    Outs.push_back(v);  // вектор будет заполнен номерами вершин в порядке возрастания времени выхода
}
void TOutSort(IGraph* G, std::vector<int>& Outs) {  // вектор Outs будет заполнен номерами вершин в порядке возрастания времени выхода
    G->Reverse();
    std::vector<char> used(G->VertCount(), 0);
    for (int i = 0; i < G->VertCount(); ++i) {
        if (used[i] == 0) {
            used[i] = 1;
            dfs1(G, i, used, Outs);
        }
    }
    assert(Outs.size() == G->VertCount());
    G->Reverse();
}

void dfs2(IGraph* G, int v, int counter, std::vector<int>& comp, IGraph* G1) {
    std::vector<int> vert;
    std::vector<char> edges(counter, 0); // содержит информацию о наличии рёбер между текущей КСС и предыдущими
    G->GetNextVert(v, vert);
    for (int i = 0; i < vert.size(); ++i) {
        if (comp[vert[i]] == 0) {
            comp[vert[i]] = counter;
            dfs2(G, vert[i], counter, comp, G1);
        } else if (comp[vert[i]] < counter && edges[comp[vert[i]] - 1] == 0) {
            edges[comp[vert[i]] - 1] = 1;
            assert(counter == G1->VertCount());
            G1->AddEdge(counter - 1, comp[vert[i]]);
        }
    }
}
IGraph* GraphCondensation(IGraph* G, std::vector<int> Outs, IGraph* G1) {
    int counter = 0;
    std::vector<int> comp(G->VertCount(), 0);  // значение элемента с индексом i будет показывать, к какой КСС относится i-я вершина (нумерация КСС с 1)
    for (int i = Outs.size() - 1; i >= 0 ; --i) {
        if (comp[Outs[i]] == 0) {
            ++counter;
            comp[Outs[i]] = counter;
            G1->AddVertex();
            dfs2(G, Outs[i], counter, comp, G1);
        }
    }
    return G1;
}

void dfs3(IGraph* G, int v, int& counter, std::vector<char>& used) {
    std::vector<int> vert;
    G->GetNextVert(v, vert);
    if (vert.empty()) {
        ++counter;
    }
    for (int i = 0; i < vert.size(); ++i) {
        if (used[vert[i]] == 0) {
            used[vert[i]] = 1;
            dfs3(G, vert[i], counter, used);
        }
    }
}
int SinksCount(IGraph* G) {  // считает количество стоков в графе(в т.ч. изолированные вершины)
    int counter = 0;
    std::vector<char> used(G->VertCount(), 0);
    for (int i = 0; i < G->VertCount(); ++i) {
        if (used[i] == 0) {
            used[i] = 1;
            dfs3(G, i, counter, used);
        }
    }
    return counter;
}

int main() {
    int V;
    int E;
    std::cin >> V;
    std::cin >> E;
    IGraph* G = new ListGraph(V);

    int x;
    int y;
    for (int i = 0; i < E; ++i) {
        std::cin >> x;
        std::cin >> y;
        G->AddEdge(x-1, y-1);
    }

    std::vector<int> Outs;
    TOutSort(G, Outs);
    /*std::cout << "!-! " << std::endl;
    for (int i = 0; i < Outs.size(); ++i) {
        std::cout << Outs[i] << ", ";
    }
    std::cout << std::endl;*/

    IGraph* G1 = new ListGraph(static_cast<unsigned int>(0));  // сконденсированный граф
    GraphCondensation(G, Outs, G1);
    //std::cout << "!-! " << G1->VertCount() << std::endl;

    x = SinksCount(G1);
    //std::cout << "!-! " << x << std::endl;

    G1->Reverse();
    //std::cout << "!-! " << std::endl;

    y = SinksCount(G1);  // стоки транспонированного графа - истоки исходного(включ. изолированные вершины)
    //std::cout << "!-! " << y << std::endl;

    if (G1->VertCount() != 1) {
        std::cout << std::max(x, y) << std::endl;
    } else {
        std::cout << 0 << std::endl;
    }
    delete G;
    delete G1;

    //system("pause");
    return 0;
}