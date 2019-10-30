#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>

enum Rule {
    FirstRule,
    SecondRule,
    ThirdRule
};

struct Node {
    std::weak_ptr<Node> parent;
    std::weak_ptr<Node> suffix_link;
    int left = 0;
    int right = 0;
    bool is_leaf = false;
    std::map<char, std::shared_ptr<Node>> go;

    int number = 0;

    Node() = default;
    Node(const std::shared_ptr<Node>& parent, int l, int r, bool is_leaf):
            parent(parent), left(l), right(r), is_leaf(is_leaf) {}
};

std::ostream& operator<<(std::ostream& out, const Node& node) {
    out << node.left << "-" << node.right;
}

struct Info {  // содержит информацию о положении суффикса в дереве
    std::shared_ptr<Node> vertex;  // вершина, на ребре ведущем в которую расположен суффикс

    int end = 0;  // положение в ребре, в котором заканчивается суффикс
    // если совпадает с vertex->right, то значит, что суффикс заканчивается в самой вершине

    Info() = default;
    Info(const std::shared_ptr<Node>& v, int e): vertex(v), end(e) {}
    Info(const std::shared_ptr<Node>& v): vertex(v) {
        end = v->right;
    }
};

class SuffixTree {
public:
    std::shared_ptr<Node> root;
    int vert_count = 1;

    SuffixTree() {
        root = std::make_shared<Node>();
        root->parent = root;
        root->suffix_link = root;
    }

    void Build(const std::string& str);
    void ExtendTree(const std::string& str, Info& first, int idx);
    // продлевает суффиксы в дереве c [i .. idx - 1] до [i .. idx] начиная с некоторого суффикса first

    void ExtendSuffix(const std::string& str, Info& suf, int idx);
    // продлевает суффикс, заканчивающийся в suf символом str[idx] и обновляет suf информацией о продлённом суффикса

    void Jump(const std::string& str, Info& suf, int idx);
    void Print();
private:
    Rule _HowToExtend(const std::string& str, Info& suf, int idx);
    void _Print(const std::shared_ptr<Node>& curr, int& number);
};


void SuffixTree::_Print(const std::shared_ptr<Node>& curr, int& number) {
    if (curr != root) {
        std::cout << number << ": " << curr->left << " " << curr->right << std::endl;
    }
    for (int i = 0; i < 256; i++) {
        if (curr->go.count(i) != 0) {
            number++;
            _Print(curr->go[i], number);
        }
    }
}

void SuffixTree::Print() {
    int number = 0;
    _Print(root, number);
}


void SuffixTree::Jump(const std::string& str, Info& suf, int idx) {
    if (suf.vertex == root) {
        return;
    }
    std::shared_ptr<Node> vert = suf.vertex;
    int length = suf.end - suf.vertex->left;
    int pos = 0;
    // переходим на родителя
    vert = vert->parent.lock();

    int begin = suf.end - length;
    if (vert == root) {
        begin++;
        length--;
    }
    // переходим по суффиксной ссылке
    vert = vert->suffix_link.lock();

    // теперь нужно отойти на такое же расстояние, на которое поднялись
    while (pos < length) {
        vert = vert->go[str[begin + pos]];
        pos += vert->right - vert->left;
    } // окажемся в вершине, в которую ведёт ребро, на котором нужно остановиться

    int new_end = vert->right - (pos - length);
    suf = Info(vert, new_end);
}

Rule SuffixTree::_HowToExtend(const std::string& str, Info& suf, int idx) {
    if (suf.vertex->is_leaf && suf.end == idx) {
        return FirstRule;
    }
    if (suf.vertex->right == suf.end || suf.vertex == root) {  // суффикс заканчивается в вершине
        if (suf.vertex->go.count(str[idx]) == 0) {
            return SecondRule;
        } else {
            return ThirdRule;
        }
    } else {  // суффикс заканчивается в середине ребра
        if (str[idx] == str[suf.end]) {
            return ThirdRule;
        } else {
            return SecondRule;
        }
    }
}

void SuffixTree::ExtendSuffix(const std::string& str, Info& suf, int idx) {
    // будем использовать функцию, чтобы продлевать суффиксы по 2 правилу
    // в suf будет позиция непродлённого суффикса - обязательно в реальной вершине
    if (suf.vertex->right == suf.end || suf.vertex == root) {
        // суффикс заканчивается в вершине - потому добавим лист
        suf.vertex->go[str[idx]] = std::make_shared<Node>(suf.vertex, idx, str.size(), true);
        vert_count++;
    } else {  // иначе нужно разбить ребро вершиной и добавить ответвление
        std::shared_ptr<Node> parent = suf.vertex->parent.lock();

        // создадим разделяющую вершину и добавим переход из родителя в неё
        std::shared_ptr<Node> sep_vertex = std::make_shared<Node>(parent, suf.vertex->left, suf.end, false);
        vert_count++;
        parent->go[str[sep_vertex->left]] = sep_vertex;

        //создадим лист, продлевающий наш суффикс
        sep_vertex->go[str[idx]] = std::make_shared<Node>(sep_vertex, idx, str.size(), true);
        vert_count++;

        // обновим родителя и метку для исходной вершины
        suf.vertex->parent = sep_vertex;
        suf.vertex->left = suf.end;

        // добавим переход из разделяющей вершины в исходную
        sep_vertex->go[str[suf.vertex->left]] = suf.vertex;

        // обновим suf
        suf = Info(sep_vertex, sep_vertex->right);
    }
}

void SuffixTree::Build(const std::string& str) {
    Info first = Info(root);
    for (int i = 0; i < str.size(); i++) {
        ExtendTree(str, first, i);
    }
}

void SuffixTree::ExtendTree(const std::string& str, Info& first, int idx) {
    if (idx == 0) {
        root->go[str[0]] = std::make_shared<Node>(root, 0, str.size(), true);
        vert_count++;
        first = Info(root->go[str[0]], 1);
    } else {
        // будем продлевать суффиксы по очереди, пока не обнаружим, что используем третье правило

        while (_HowToExtend(str, first, idx) == FirstRule) {
            Jump(str, first, idx);
        }

        std::shared_ptr<Node> vert_for_link_update = nullptr;
        while (_HowToExtend(str, first, idx) == SecondRule) {
            ExtendSuffix(str, first, idx);

            if (vert_for_link_update && vert_for_link_update->suffix_link.lock() == nullptr) {
                vert_for_link_update->suffix_link = first.vertex;
            }
            vert_for_link_update = first.vertex;
            Jump(str, first, idx);
        }
        // далее у нас действует уже третье правило: поэтому обновим последнюю суффиксную ссылку и передвием first
        if (vert_for_link_update && !vert_for_link_update->suffix_link.lock()) {
            vert_for_link_update->suffix_link = first.vertex;
        }

        if (first.vertex->right == first.end || first.vertex == root) {
            first.vertex = first.vertex->go[str[idx]];
            first.end = first.vertex->left + 1;
        } else {
            first.end++;
        }

    }
}



void _AdaptForTwoStrings(SuffixTree& tree, int first_size, const std::shared_ptr<Node>& curr, int& count) {
    for (auto& i: curr->go) {
        if (i.second->left < first_size && i.second->right > first_size) {  // нужно обрубить ребро на метке s.size()
            curr->go[i.first] = std::make_shared<Node>(curr, i.second->left, first_size, true);
        }
        count++;
        i.second->number = count - 1;
        _AdaptForTwoStrings(tree, first_size, i.second, count);

    }
}

void AdaptForTwoStrings(SuffixTree& tree, int first_size) {
    //  укорачивает рёбра, обновляет количество вершин, считает их номера
    int count = 1;
    _AdaptForTwoStrings(tree, first_size, tree.root, count);
    tree.vert_count = count;
}

void _PrintDoubleTree(const SuffixTree& tree, int first_size, const std::shared_ptr<Node>& curr) {
    if (curr != tree.root) {
//        std::cout << curr->parent.lock()->number << " ";
        printf("%d ", curr->parent.lock()->number);
        if (curr->left >= first_size) {
//            std::cout << 1 << " " << curr->left - first_size << " " << curr->right - first_size << std::endl;
            printf("1 %d %d\n", curr->left - first_size, curr->right - first_size);
        } else {
//            std::cout << 0 << " " << curr->left << " " << curr->right << std::endl;
            printf("0 %d %d\n", curr->left, curr->right);
        }
    }
    for (auto& i: curr->go) {
        _PrintDoubleTree(tree, first_size, i.second);
    }
}

void PrintDoubleTree(const SuffixTree& tree, int first_size) {
    _PrintDoubleTree(tree, first_size, tree.root);
}

int main() {
    std::string s;
    std::cin >> s;
    std::string t;
    std::cin >> t;

    SuffixTree T;
    T.Build(s+t);

    AdaptForTwoStrings(T, s.size());
    std::cout << T.vert_count << std::endl;
    PrintDoubleTree(T, s.size());
}