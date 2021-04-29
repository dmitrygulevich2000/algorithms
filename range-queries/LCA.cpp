#include <iostream>
#include <vector>
#include <cmath>

struct Tree {
    std::vector<long long> parent;
    std::vector<long long> depth;
    std::vector<std::vector<long long>> ancestor;
    long long max_depth = 0;

    Tree() = default;
    explicit Tree(long long n);

    void CalcDepth();
    void CalcAncestors();
    int LCA(long long u, long long v);

    void get_depth(long long v);
};

Tree::Tree(long long n): parent(n, 0), depth(n, 0), max_depth(1) {
    long long tmp;
    for (long long i = 1; i < n; ++i) {
        std::cin >> tmp;
        parent[i] = tmp;
    }
    CalcDepth();
    long long max_degree = int(std::log(double(max_depth)) / std::log(double(2))) + 1;
    if (max_degree != 0) {
        ancestor = std::vector<std::vector<long long>>(max_degree, std::vector<long long>(parent.size(), 0));
    }
    CalcAncestors();
}

void Tree::CalcDepth() {
    std::vector<long long> result(parent.size(), 0);  // корень - 0 и для него глубина записана правильно
    for (long long i = 0; i < parent.size(); ++i) {
        get_depth(i);
    }
}

void Tree::get_depth(long long v) {
    if (v == 0 || depth[v] != 0) {  // вершина - корень, или результат для неё уже записан
        return;
    }
    get_depth(parent[v]);
    depth[v] = depth[parent[v]] + 1;
    if (depth[v] > max_depth) {
        max_depth = depth[v];
    }
}

void Tree::CalcAncestors() {
    ancestor[0] = parent;
    for (long long deg = 1; deg < ancestor.size(); ++deg) {  // то есть вычисляем предка vй вершины на расстоянии 2^deg
        for (long long v = 0; v < ancestor[0].size(); ++v) {
            ancestor[deg][v] = ancestor[deg - 1][ancestor[deg - 1][v]];
        }
    }
}

int Tree::LCA(long long u, long long v) {
    if (depth[v] < depth[u]) {
        std::swap(u, v);
    }
    if (u == 0) {
        return 0;
    }
    // выставим вершины на одну глубину
    long long jump = int(std::log(double(max_depth)) / std::log(double(2)));
    while (depth[u] != depth[v]) {
        long long next = ancestor[jump][v];
        if (depth[next] >= depth[u]) {
            v = next;
        }
        jump--;
    }
    if (u == v) {
        return u;
    }
    // будем искать общего предка
    jump = int(std::log(double(max_depth)) / std::log(double(2)));
    while(jump != -1) {
        long long pu = ancestor[jump][u];
        long long pv = ancestor[jump][v];
        if (pu != pv) {
            u = pu;
            v = pv;
        }
        jump--;
    }
    return parent[u];
}


int main() {
    long long N, M;
    std::cin >> N >> M;
    Tree tree(N);

    long long a1, a2, x, y, z;
    std::cin >> a1 >> a2 >> x >> y >> z;

    if (M > 0) {
        long long curr_res = tree.LCA(a1, a2);
        long long sum = curr_res;
        for (int i = 2; i <= M; ++i) {
            long long tmp1 = (x * a1 + y * a2 + z) % N;
            long long tmp2 = (x * a2 + y * tmp1 + z) % N;
            a1 = tmp1;
            a2 = tmp2;
            curr_res = tree.LCA((a1 + curr_res) % N, a2);
            sum += curr_res;
        }
        std::cout << sum << std::endl;
    }
}
