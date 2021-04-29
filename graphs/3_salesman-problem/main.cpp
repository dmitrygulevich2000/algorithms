#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>

#include "ArcGraph.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "DSU.h"

double Kruskal(IGraph& G, std::vector<std::vector<bool>>& span) {
    ArcGraph G1(G);
    if (G1.VertCount() <= 1) {
        return 0;
    }
    DSU dsu(G.VertCount());

    std::sort(G1.Edges.begin(), G1.Edges.end());

    double result = 0;
    for (Edge i: G1.Edges) {
        if (dsu.Union(i.from, i.to)) {
            result += i.weight;
            span[i.from][i.to] = true;
            span[i.to][i.from] = true;
        }
        if (dsu.count == 1) {
            return result;
        }
    }
}

void dfs(IGraph& G, int v, std::vector<char>& used,
         double& result, int& curr, const std::vector<std::vector<bool>>& span) {
    used[v] = 1;
    curr = v;
    std::vector<std::pair<int, double>> vert;
    G.GetNextVert(v, vert);

    for (auto i: vert) {
        if (used[i.first] == 0 && span[v][i.first]) {
            result += G.FindWeight(curr, i.first);
            // std::cout << curr << " <-> " << i.first << std::endl;
            dfs(G, i.first, used, result, curr, span);
        }
    }
}

double Hamilton(IGraph& G) {
    double result = 0;
    std::vector<char> used(G.VertCount(), 0);
    // span хранит информацию о том, какие рёбра лежат в мин.остове
    std::vector<std::vector<bool>> span(G.VertCount(), std::vector<bool>(G.VertCount(), false));

    Kruskal(G, span);

    int curr = 0;
    dfs(G, 0, used, result, curr, span);
    result += G.FindWeight(0, curr);
    return result;
}

void fill_graph(IGraph& G, int N, std::vector<std::pair<double, double>>& points) {
    std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::normal_distribution<double> d(double(N), 1);

    for (int i = 0; i < N; ++i) {
        double x = d(gen);
        double y = d(gen);
        points.emplace_back(std::make_pair(x, y));
    }
    for (int i = 0; i < points.size(); ++i) {

        //std::cout << points[i].first << " ; " << points[i].second << std::endl;

        for (int j = i + 1; j < N; ++j) {
            double side1 = std::abs(points[i].first - points[j].first);
            double side2 = std::abs(points[i].second - points[j].second);
            double dist = std::sqrt(std::pow(side1, 2) + std::pow(side2, 2));
            G.AddEdge(i, j, dist);
        }
    }
}

int main() {
    int b, e;
    std::cout << "Enter the semi-interval. For each number from it test will run for 10 times" << std::endl;
    std::cin >> b >> e;

    for (int N = b; N < e; ++N) {

        std::vector<double> results;
        double average = 0;
        for (int i = 0; i < 10; ++i) {
            std::vector<std::pair<double, double>> points;
            MatrixGraph G(N);
            fill_graph(G, N, points);

            double res = Hamilton(G);
            results.push_back(res);
            average += res;
            std::cout << "Total result: " << res << " for " << N << " points" << std::endl;
        }
        average = average / 10;
        double square_sum = 0;
        for (int i = 0; i < results.size(); ++i) {
            square_sum += std::pow(std::abs(results[i] - average), 2);
        }
        square_sum /= 10;
        double mean_square = std::sqrt(square_sum);
        std::cout << "Average result: " << average << std::endl;
        std::cout << "Standard deviation: " << mean_square << std::endl;
        std::cout << "-------------------------------------" << std::endl;
    }
    return 0;
}
