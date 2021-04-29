//
// Created by DmitryGulevich on 28.04.2019.
//

#ifndef SALESMAN_DSU_H
#define SALESMAN_DSU_H

#include<vector>

struct DSU {
    std::vector<int> parent;
    std::vector<int> rank; // буду обращаться только к рангу, хранящемуся в индексах представителей
    int count = 0;

    DSU(int count): parent(count, -1), rank(count, 1), count(count) {}
    DSU() = default;

    int Find(int u);
    bool Union(int u, int v);
};

#endif //SALESMAN_DSU_H
