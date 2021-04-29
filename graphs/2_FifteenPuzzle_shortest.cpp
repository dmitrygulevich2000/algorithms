#include <iostream>
#include<vector>
#include<set>
#include<cassert>
#include<unordered_map>
#include<string>

int field_size = 4;
int game_cells = field_size*field_size;

enum step { U, L, R, D, None };

class Vertex {
public:
    unsigned long long conf = 0;
    int zero_pos = -1;  // позиция нуля в векторном представлении

    explicit Vertex(std::vector<int> v) {
        for (int i = 0; i < v.size(); ++i) {
            assert(v.size() == game_cells);
            if (v[i] == 0) {
                zero_pos = i;
            }
            conf *= 16;
            conf += v[i];
        }
    }
    explicit Vertex(unsigned long long c): conf(c) {
        unsigned long long tmp = 15LL;
        unsigned long long flag = c & tmp;
        int i = game_cells - 1;
        while (flag != 0) {
            tmp *= 16LL;
            flag = c & tmp;
            i -= 1;
        }
        zero_pos = i;
    }
    ~Vertex() = default;

    void print();

    std::vector<int> to_vector() const;
    Vertex make_move(step x);
    void get_next_states(std::vector<std::pair<Vertex, step>>& vert);
    bool is_solvable();

    void Manhattan_dist(const std::vector<int>& state, int& heuristics) const;
    void Linear_conflicts(const std::vector<int>& state, int& heuristics) const;
    void Last_move(const std::vector<int>& state, int& heuristics) const;
    void Corner_cell(const std::vector<int>& state, int& heuristics) const;
    int Heuristics() const;
};

void Vertex::print() {
    std::vector<int> state = this->to_vector();
    for (int i = 0; i < field_size; ++i) {
        for (int j = 0; j < field_size; ++j) {
            std::cout << state[i * field_size + j] << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<int> Vertex::to_vector() const {
    std::vector<int> res1;
    unsigned long long tmp = conf;
    for (int i = 0; i < game_cells; ++i) {
        res1.push_back(int(tmp & 15));
        tmp /= 16;
    }
    std::vector<int> res2;
    for (int i = 0; i < game_cells; ++i) {
        res2.push_back(res1[game_cells - 1 - i]);
    }
    return res2;
}

Vertex Vertex::make_move(step x) {
    std::vector<int> state = this->to_vector();
    if (x == D) {
        std::swap(state[zero_pos], state[zero_pos + field_size]);
    } else if (x == U) {
        std::swap(state[zero_pos], state[zero_pos - field_size]);
    } else if (x == R) {
        std::swap(state[zero_pos], state[zero_pos + 1]);
    } else if (x == L) {
        std::swap(state[zero_pos], state[zero_pos - 1]);
    }
    return Vertex(state);
}

void Vertex::get_next_states(std::vector<std::pair<Vertex, step>>& vert) {
    std::vector<int> state = this->to_vector();
    if (zero_pos / field_size < field_size - 1) {  // соответствует перемещению пустой клетки вниз
        std::swap(state[zero_pos], state[zero_pos + field_size]);
        vert.emplace_back(std::pair<Vertex, step>(Vertex(state), D));
        std::swap(state[zero_pos], state[zero_pos + field_size]);
    }

    if (zero_pos / field_size > 0) {  // соответствует перемещению пустой клетки вверх
        std::swap(state[zero_pos], state[zero_pos - field_size]);
        vert.emplace_back(std::pair<Vertex, step>(Vertex(state), U));
        std::swap(state[zero_pos], state[zero_pos - field_size]);
    }

    if (zero_pos % field_size != field_size - 1) {  // соответствует перемещению пустой клетки вправо
        std::swap(state[zero_pos], state[zero_pos + 1]);
        vert.emplace_back(std::pair<Vertex, step>(Vertex(state), R));
        std::swap(state[zero_pos], state[zero_pos + 1]);
    }

    if (zero_pos % field_size != 0) {  // соответствует перемещению пустой клетки влево
        std::swap(state[zero_pos], state[zero_pos - 1]);
        vert.emplace_back(std::pair<Vertex, step>(Vertex(state), L));
        std::swap(state[zero_pos], state[zero_pos - 1]);
    }
}

bool Vertex::is_solvable() {
    std::vector<int> state = this->to_vector();
    int invariant = 0;
    assert(state.size() == game_cells);
    for (int i = 0; i < game_cells; ++i) {
        for (int j = i+1; j < game_cells; ++j) {
            if (state[i] > state[j] && state[i] != 0 && state[j] != 0) {
                invariant += 1;
            }
        }
    }
    if (field_size % 2 == 0) {  // для пятнашек
        invariant += zero_pos / field_size + 1;
    }

    if (invariant % 2 == 0) {
        return true;
    } else {
        return false;
    }
}

void Vertex::Manhattan_dist(const std::vector<int>& state, int& heuristics) const {
    int required = 0;
    for (int i = 0; i < game_cells; ++i) {
        if (state[i] != 0) {
            required = state[i] - 1;
            heuristics += std::abs(i / field_size - required / field_size);
            heuristics += std::abs(i % field_size - required % field_size);
        }
    }
}

void Vertex::Linear_conflicts(const std::vector<int>& state, int& heuristics) const {
// по строкам
    for (int i = 0; i < field_size; ++i) {
        for (int j = 0; j < field_size; ++j) {
            for (int k = j+1; k < field_size; ++k) {
                int n = i * field_size;
                if ( state[n+j] > state[n+k] &&
                     state[n+j] != 0 && state[n+k] != 0 &&
                     i*field_size < state[n+j] && state[n+j] <= (i+1)*field_size &&
                     i*field_size < state[n+k] && state[n+k] <= (i+1)*field_size ) {
                    heuristics += 2;
                }
            }
        }
    }
// по столбцам
    for (int i = 0; i < field_size; ++i) {
        for (int j = 0; j < field_size; ++j) {
            for (int k = j+1; k < field_size; ++k) {
                int n1 = j * field_size + i;
                int n2 = k * field_size + i;
                if ( state[n1] > state[n2] &&
                     state[n1] != 0 && state[n2] != 0 &&
                     state[n1] % field_size == (i + 1) % field_size &&
                     state[n2] % field_size == (i + 1) % field_size ) {
                    heuristics += 2;
                }
            }
        }
    }
}

void Vertex::Corner_cell(const std::vector<int>& state, int& heuristics) const { // написана неправильно, не успользовал
//  левый верхний угол
    if (state[0] != 1 && state[1] == 2 && state[1] != 0
        && state[field_size] == field_size + 1 && state[field_size] != 0) {
        heuristics += 2;
    }
// правый верхний угол
    if (state[field_size - 1] != field_size && state[field_size - 2] == field_size - 1 && state[field_size - 2] != 0
        && state[2*field_size] == 2*field_size + 1 && state[2*field_size] != 0) {
        heuristics += 2;
    }
// левый нижний угол
    if (state[field_size*(field_size - 1)] != field_size*(field_size - 1) + 1
        && state[field_size*(field_size - 2)] == field_size*(field_size - 1) + 1
        && state[field_size - 2] != 0
        && state[field_size*(field_size - 1) + 1] == field_size*(field_size - 1) + 2
        && state[field_size*(field_size - 1) + 1] != 0) {
        heuristics += 2;
    }
}

void Vertex::Last_move(const std::vector<int>& state, int& heuristics) const { // написана неправильно, не использовал
    for (int i = 0; i < field_size; ++i) {
        if (state[game_cells - field_size + i] == 12) {
            heuristics += 2;
        }
        if (state[(i+1)*field_size - 1] == 15) {
            heuristics += 2;
        }
    }
}

int Vertex::Heuristics() const {
    int heuristics = 0;
    std::vector<int> state = this->to_vector();
    Manhattan_dist(state, heuristics);
    Linear_conflicts(state, heuristics);
    //Last_move(state, heuristics);
    //Corner_cell(state, heuristics);
    return heuristics;
}

std::string A_star(const Vertex& V) {
    std::unordered_map<unsigned long long, step> parents;
    std::unordered_map<unsigned long long, int> dist;
    std::set<std::pair<int, unsigned long long>> pr_queue;
    std::string res;

    parents.insert(std::make_pair(V.conf, None));
    dist.insert(std::make_pair(V.conf, 0));
    pr_queue.insert(std::make_pair(V.Heuristics(), V.conf));

    Vertex end({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0});

    while(!pr_queue.empty()) {
        Vertex curr((*pr_queue.begin()).second);
        pr_queue.erase(pr_queue.begin());
        if (curr.conf == end.conf) {  // решение найдено!
            while (parents[curr.conf] != None) {
                step x = parents[curr.conf];
                if (x == D) {
                    res = "D" + res;
                } else if (x == U) {
                    res = "U" + res;
                } else if (x == R) {
                    res = "R" + res;
                } else if (x == L) {
                    res = "L" + res;
                }
                curr = curr.make_move(x);
                assert(parents.count(curr.conf) != 0);
            }
            return res;
        }

        std::vector<std::pair<Vertex, step>> vert;
        curr.get_next_states(vert);

        for (int i = 0; i < vert.size(); ++i) {
            int d1 = dist[curr.conf] + 1 + vert[i].first.Heuristics();
            int d2 = dist[curr.conf] + 1;
            if (dist.count(vert[i].first.conf) == 0) {
                dist[vert[i].first.conf] = d2;
                pr_queue.insert(std::make_pair(d1, vert[i].first.conf));
                parents[vert[i].first.conf] = step(3 - vert[i].second);  // получим ход, который нужно сделать, чтобы отступить к родителю
            }
            else if (dist[vert[i].first.conf] > d2) {
                pr_queue.erase(std::make_pair(dist[vert[i].first.conf] + vert[i].first.Heuristics(), vert[i].first.conf));
                dist[vert[i].first.conf] = d2;
                pr_queue.insert(std::make_pair(d1, vert[i].first.conf));
                parents[vert[i].first.conf] = step(3 - vert[i].second);  // получим ход, который нужно сделать, чтобы отступить к родителю
            }
        }
    }
}


int main() {
    std::vector<int> v(game_cells, 0);
    for (int i = 0; i < game_cells; ++i) {
        std::cin >> v[i];
    }
    Vertex vertex(v);

    if (vertex.is_solvable()) {
        std::string s = A_star(vertex);
        std::cout << s.size() << std::endl;
        std::cout << s << std::endl;
    } else {
        std::cout << "-1" << std::endl;
    }

    return 0;
}