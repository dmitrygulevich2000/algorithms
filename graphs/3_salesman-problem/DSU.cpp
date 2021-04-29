//
// Created by DmitryGulevich on 28.04.2019.
//

#include "DSU.h"

int DSU::Find(int u) {
    //std::cout << "find( " << u << " ) " << std::endl;
    if (parent[u] != -1) {
        int res = Find(parent[u]);
        parent[u] = res;
        return res;
    } else {
        return u;
    }
}

bool DSU::Union(int u, int v) {
    int u_main = Find(u);
    int v_main = Find(v);
    if (u_main != v_main) {
        if (rank[u_main] < rank[v_main]) {
            parent[u_main] = v_main;
            count -= 1;
        } else {
            parent[v_main] = u_main;
            count -= 1;
            if (rank[u_main] == rank[v_main]) {
                rank[u_main]++; // достаточно поменять ранг для представителя полученного объединения
            }
        }
        return true;
    }
    return false;
}