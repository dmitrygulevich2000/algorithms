#include <iostream>
#include <vector>
#include <cmath>

struct SparseTable {
    std::vector<std::vector<std::pair<int, int>>> table;
    // буду хранить пару индексов: индекс минимума и индекс второй статистики
    std::vector<int> deg;
    std::vector<int> log;


    SparseTable() = default;
    explicit SparseTable(int height, int width): table(height, std::vector<std::pair<int, int>>(width, std::pair<int, int>(0, 0))) {
        deg.push_back(1);
        log.push_back(-1);
        for (int i = 1; i < height; ++i) {
            deg.push_back(int(std::pow(2, i)));
        }
        for (int i = 1; i <= width; ++i) {
            log.push_back(int(std::log(double(i)) / std::log(double(2))));
        }
    }

    void PreCalculation(const std::vector<int>& seq);
    std::pair<int, int> Union(std::pair<int, int> one, std::pair<int, int> two, const std::vector<int>& seq) const ;
    // позволяет получить результат для объединения отрезков, где first и last - результаты для этих отрезков

    int GetSecStat(int b, int e, const std::vector<int>& seq); // результат запроса на [b, e]
};

void SparseTable::PreCalculation(const std::vector<int>& seq) {
// к первой строчке SparseTable обращений не будет, ровно как и к последнему столбцу
    for (int i = 0; i < table[1].size() - 1; ++i) {
        if (seq[i] <= seq[i+1]) {
            table[1][i] = std::make_pair(i, i + 1);
        } else {
            table[1][i] = std::make_pair(i+1, i);
        }
    }

    for (int i = 2; i < table.size(); ++i) {
        for (int j = 0; j < table[i].size(); ++j) {
            if (j < table[i].size() - deg[i-1]) {
                table[i][j] = Union(table[i-1][j], table[i-1][j + deg[i-1]], seq);
            } else {
                table[i][j] = table[i-1][j];
            }
        }
    }
}

std::pair<int, int> SparseTable::Union(std::pair<int, int> one, std::pair<int, int> two, const std::vector<int>& seq) const {
    // функция для получения результатов на объединении двух отрезков, даже если они пересекаются
    if (one.first != two.first) {
        // в этом случае результат - <мин из минимумов ; макс из минимумов или мин из вторых статистик>
        if (seq[one.first] > seq[two.first]) {
            std::swap(one, two);
        }
        int res1 = one.first;
        int res2 = 0;
        if (seq[one.second] < seq[two.first]) {
            res2 = one.second;
        } else {
            res2 = two.first;
        }
        return std::make_pair(res1, res2);
    } else {  // one.first == two.first
        // в этом - <любой мин ; мин из вторых статистик>
        int res1 = one.first;
        int res2 = one.second;
        if (seq[one.second] > seq[two.second]) {
            res2 = two.second;
        }
        return std::make_pair(res1, res2);
    }

}

int SparseTable::GetSecStat(int b, int e, const std::vector<int>& seq) {  // результат запроса на [b, e]
    int len = log[e - b + 1];
    auto res = Union(table[len][b], table[len][e - deg[len] + 1], seq);
    return seq[res.second];
}


int main() {
    int N, M;
    std::cin >> N >> M;
    std::vector<int> seq;

    int x, y;
    for (int i = 0; i < N; i++) {
        std::cin >> x;
        seq.push_back(x);
    }
    SparseTable table(int(std::log(double(N)) / std::log(double(2))) + 1, N);
    table.PreCalculation(seq);

//    for (int i = 0; i < table.table.size(); ++i) {
//        for (int j = 0; j < table.table[i].size(); ++j) {
//            std::cout << table.table[i][j].first << ";" << table.table[i][j].second << "  ";
//        }
//        std::cout << "\n";
//    }
//    std::cout << "\n";

    for (int i = 0; i < M; i++) {
        std::cin >> x >> y;
        std::cout << table.GetSecStat(x-1, y-1, seq) << std::endl;
    }
}