#include <iostream>
#include <string>
#include <vector>
#include <queue>


int a_ascii = 97;

struct Vertex {
    std::vector<unsigned int> edges;
    int suffix_link = -1;
    int compressed_suffix_link = -1;
    bool is_terminate = false;
    std::vector<int> info;  // здесь будут храниться индексы конца вхождения в паттерн
                            // подстрок, заканчивающихся в этой вершине


    unsigned int parent = 0;
    char ch = 0;  // символ, по которому выполнен переход из предка в нас

    Vertex(): edges(26, -1) {}
    Vertex(unsigned int parent, char ch): parent(parent), ch(ch), edges(26, -1) {}
};

class PrefixTree {
public:
    std::vector<Vertex> trie;

    PrefixTree(): trie(1) {}

    unsigned int AddString(const std::string& str);  // возвращает индекс терминальной вершины для этой строки
    void AddInfo(unsigned int state, unsigned int end);

    std::vector<int> GetInfo(unsigned int state);
    bool IsTerminate(unsigned int state);
    unsigned int StateCount();

    // получение переходов и проч. с помощью ленивой рекурсии (в итоговом решении не использовал)
    unsigned int Transite(unsigned int state, char symbol);
    unsigned int GetSuffixLink(unsigned int state);
    unsigned int GetCompressedSuffixLink(unsigned int state);
    //

    void BuildLinks();

    void Print();

private:
    void build_suffix_link(unsigned int state);
    void build_compressed_suffix_link(unsigned int state);
};

void PrefixTree::Print() {
    std::cout << "size: " << trie.size() << std::endl;
    for (int i = 0; i < trie.size(); i++) {
        std::cout << i << ": " << trie[i].parent << " " << trie[i].ch << " " << (trie[i].is_terminate ? " t " : "");
//        std::cout << GetSuffixLink(i) << " " << GetCompressedSuffixLink(i);

//        std::cout << " -> ";
//        for (int j = 0; j < trie[i].edges.size(); j++) {
//            if (trie[i].edges[j] != -1) {
//                std::cout << char(j + a_ascii) << ";" << trie[i].edges[j] << " ";
//            }
//        }

        std::cout << std::endl;
    }
}

unsigned int PrefixTree::StateCount() {
    return trie.size();
}

void PrefixTree::AddInfo(unsigned int state, unsigned int end) {
    trie[state].info.push_back(end);
}

std::vector<int> PrefixTree::GetInfo(unsigned int state) {
    return trie[state].info;
}

bool PrefixTree::IsTerminate(unsigned int state) {
    return trie[state].is_terminate;
}

unsigned int PrefixTree::AddString(const std::string& str) {  // возвращает индекс терминальной вершины для этой строки
    unsigned int curr = 0;
    for (char  symbol: str) {
        if (trie[curr].edges[symbol - a_ascii] == -1) {
            trie[curr].edges[symbol - a_ascii] = trie.size();
            trie.emplace_back(curr, symbol);
            curr = trie.size() - 1;
        } else {
            curr = trie[curr].edges[symbol - a_ascii];
        }
    }
    trie[curr].is_terminate = true;
    return curr;
}

unsigned int PrefixTree::Transite(unsigned int state, char symbol) {
    if (trie[state].edges[symbol - a_ascii] != -1) {
        return trie[state].edges[symbol - a_ascii];
    }
    if (state == 0) {
        return 0;
    }
    unsigned int result = Transite(GetSuffixLink(state), symbol);
    trie[state].edges[symbol - a_ascii] = result;
    return result;
}

unsigned int PrefixTree::GetSuffixLink(unsigned int state) {
    if (trie[state].suffix_link != -1) {
        return trie[state].suffix_link;
    }
    if (state == 0 || trie[state].parent == 0) {
        trie[state].suffix_link = 0;
        return 0;
    }
    int result = Transite(GetSuffixLink(trie[state].parent), trie[state].ch);
    trie[state].suffix_link = result;
    return result;
}

unsigned int PrefixTree::GetCompressedSuffixLink(unsigned int state) {
    if (trie[state].compressed_suffix_link != -1) {
        return trie[state].compressed_suffix_link;
    }
    if (state == 0) {
        trie[state].compressed_suffix_link = 0;
        return 0;
    }

    unsigned int result = GetSuffixLink(state);
    if (!trie[result].is_terminate) {
        result = GetCompressedSuffixLink(result);
    }
    trie[state].compressed_suffix_link = result;
    return result;
}


void PrefixTree::build_suffix_link(unsigned int state) {
    unsigned int parent = trie[state].parent;
    if (parent == 0) {
        trie[state].suffix_link = 0;
    } else {
        parent = trie[parent].suffix_link;
        char ch = trie[state].ch;
        trie[state].suffix_link = trie[parent].edges[ch - a_ascii];
    }
}

void PrefixTree::build_compressed_suffix_link(unsigned int state) {
    unsigned int suf_link = trie[state].suffix_link;
    if (trie[suf_link].is_terminate) {
        trie[state].compressed_suffix_link = suf_link;
    } else {
        trie[state].compressed_suffix_link = trie[suf_link].compressed_suffix_link;
    }
}

void PrefixTree::BuildLinks() {
    std::queue<unsigned int> q;

    trie[0].suffix_link = 0;  // заполним базу: значения для корня бора
    trie[0].compressed_suffix_link = 0;
    for (int i = 0; i < trie[0].edges.size(); i++) {
        if (trie[0].edges[i] != -1) {
            q.push(trie[0].edges[i]);
        } else {
            trie[0].edges[i] = 0;
        }
    }

    while (!q.empty()) {
        unsigned int curr = q.front();
        q.pop();

        build_suffix_link(curr);

        for (int i = 0; i < trie[curr].edges.size(); i++) {  // построим переходы для текущего состояния
            if (trie[curr].edges[i] != -1) {
                q.push(trie[curr].edges[i]);  // заодно добавим "честные" переходы в очередь
            } else {
                unsigned int suf_link = trie[curr].suffix_link;
                trie[curr].edges[i] = trie[suf_link].edges[i];
            }
        }

        build_compressed_suffix_link(curr);
    }
}


unsigned int FillTrie(PrefixTree& t, const std::string& pattern) {  // возвращает количество подстрок паттерна между '?'
    std::string s;
    unsigned int i = 0;
    unsigned int cnt = 0;
    while (i < pattern.size()) {
        if (pattern[i] != '?') {
            while (i < pattern.size() && pattern[i] != '?') {  // считываем подстроку паттерна до следующего '?'
                s += pattern[i];
                i++;
            }
            unsigned int terminate = t.AddString(s);
            t.AddInfo(terminate, i - 1);
            cnt++;
            s.clear();
        } else {
            i++;
        }
    }
    return cnt;
}


void FindPattern(const std::string& pattern, const std::string& text) {
    if (pattern.empty() || text.empty()) {
        return;
    }

    PrefixTree t;
    int str_count = FillTrie(t, pattern);
    if (t.StateCount() == 1) {
        for (int i = 0; i < int(text.size()) - int(pattern.size()) + 1; i++) {
            std::cout << i << " ";
        }
        return;
    }

    t.BuildLinks();


    std::vector<int> search_counter(text.size(), 0);  // количество найденных подстрок паттерна, начинающегося с i позиции
    unsigned int current_state = 0;
    unsigned int text_size = text.size();
    unsigned int pattern_size = pattern.size();
    for (int i = 0; i < text_size; i++) {

        current_state = t.trie[current_state].edges[text[i] - a_ascii];
        int curr_found = current_state;

        while (curr_found != 0) {
            if (t.trie[curr_found].is_terminate) {
                std::vector<int> info = t.trie[curr_found].info;

                for (int end: info) {
                    int start_index = i - end;
                    if (start_index >= 0) {
                        search_counter[start_index]++;
                        if (search_counter[start_index] == str_count && start_index + pattern_size <= text_size) {
                            std::cout << start_index << " ";
                        }
                    }
                }
            }
            curr_found = t.trie[curr_found].compressed_suffix_link;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string pattern;
    std::string text;
    std::cin >> pattern >> text;

    FindPattern(pattern, text);
    std::cout << "\n";

    return 0;
}