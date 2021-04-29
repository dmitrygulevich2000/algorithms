#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>


struct Segment { // хранятся в узлах дерева и хранят информацию о суффиксе, постфиксе и макс. подотрезке
    int suf = 0;
    int post = 0;
    int max = 0;
    int len = 1;

    Segment() = default;
    Segment(int s, int p, int m, int l): suf(s), post(p), max(m), len(l) {}
};

Segment Concatenate(const Segment& left, const Segment& right) {
    if (left.len == 0) {
        return right;
    }
    if (right.len == 0) {
        return left;
    }

    int suf, post, max;
    suf = left.suf;
    if (left.suf == left.len) {
        suf += right.suf;
    }

    post = right.post;
    if (right.post == right.len) {
        post += left.suf;
    }

    int tmp = std::max(left.max, right.max);
    max = std::max(tmp, left.post + right.suf);

    return {suf, post, max, left.len + right.len};
}

struct SegmentTree {
    std::vector<Segment> tree;

    SegmentTree() = default;
    explicit SegmentTree(int size);

    void Build();
    int GetMaxSeq(int l, int r);
    void Print();

    Segment get_seq(int l, int r, int x, int lx, int rx);
    int find_size(int size);
    int left(int i);
    int right(int i);
};

int SegmentTree::left(int i) {
    return 2*i + 1;
}
int SegmentTree::right(int i) {
    return 2*i + 2;
}

SegmentTree::SegmentTree(int size) {
    int pow_of_two = find_size(size);
    tree.resize(2*pow_of_two - 1);
    for (int i = 0; i < size; i++) {
        int x;
        std::cin >> x;
        tree[pow_of_two - 1 + i] = Segment(x, x, x, 1);
    }
    Build();
}

int SegmentTree::find_size(int size) {
    int new_size = 1;
    int tmp = size / 2;
    while(tmp != 0) {
        new_size *= 2;
        tmp /= 2;
    }
    if (new_size < size) {
        new_size *= 2;
    }
    return new_size;
}

void SegmentTree::Build() {
    for (int i = tree.size()/2 - 1; i >= 0; i--) {
        tree[i] = Concatenate(tree[left(i)], tree[right(i)]);
    }
}

int SegmentTree::GetMaxSeq(int l, int r) {
    return get_seq(l, r, 0, 0, tree.size()/2).max;
}

Segment SegmentTree::get_seq(int l, int r, int x, int lx, int rx) {
//    std::cout << l << ";" << r << ";" << x << ";" << lx << ";" << rx << std::endl;

    Segment result(0, 0, 0, 0);  // нейтральный элемент
    if( l <= lx && r >= rx ) {
        return tree[x];
    }
    if( l > rx || r < lx ) {
        return result; // нейтральный элемент
    }

    int mx = ( lx + rx ) / 2;
    result = Concatenate(get_seq(l, r, left(x), lx, mx), result);
    result = Concatenate(result, get_seq(l, r, right(x), mx + 1, rx));
    return result;
}

void SegmentTree::Print() {
    int deg = 1;
    int flag = 0;
    for (int i = 0; i < tree.size(); ++i) {
        std::cout << std::setw(2) << tree[i].suf << ";"
        << std::setw(2) << tree[i].post << ";"
        << std::setw(2) << tree[i].max << ";" << std::setw(2) << tree[i].len << "  ";
        flag++;
        if (flag == deg) {
            flag = 0;
            deg *= 2;
            std::cout << "\n";
        }
    }
}

int main() {
    int N, M;
    std::cin >> N >> M;
    SegmentTree tree(N);

    //tree.Print();
    for (int i = 0; i < M; ++i) {
        int x, y;
        std::cin >> x >> y;
        std::cout << tree.GetMaxSeq(x, y) << std::endl;
    }
}