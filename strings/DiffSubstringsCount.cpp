#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <array>


struct EquivalenceClass {
    std::array<int, 2> cls = {0, 0};
    int idx = 0;

    EquivalenceClass() = default;
    EquivalenceClass(int c1, int c2, int i): cls({c1, c2}), idx(i) {}
    EquivalenceClass& operator=(const EquivalenceClass& cl) = default;


};

std::ostream& operator<<(std::ostream& os, const EquivalenceClass& cl) {
    os << cl.cls[0] << ";" << cl.cls[1] << "->" << cl.idx;
    return os;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    for (const auto& i: v) {
        std::cout << i << " ";
    }
    return os;
}


std::vector<int> InitializeSubstrings(const std::string& str) {
    std::vector<int> substrings_pos(str.size(), 0);
    std::vector<int> count(256, 0);
    for (int i = 0; i < str.size(); i++) {
        count[str[i]]++;
    }

    int block_start = count[0];
    for (int i = 1; i < count.size(); i++) {
        block_start += count[i];
        count[i] = block_start - count[i];
    }
    count[0] = 0;

    for (int i = 0; i < str.size(); i++) {
        substrings_pos[count[str[i]]] = i;
        count[str[i]]++;
    }
    return substrings_pos;
}

std::vector<int> InitializeClasses(const std::vector<int>& substrings_pos, const std::string& str) {
    std::vector<int> classes(str.size(), 0);
    classes[0] = 0;

    int class_number = 0;
    for (int i = 1; i < substrings_pos.size(); i++) {
        if (str[substrings_pos[i-1]] != str[substrings_pos[i]]) {
            class_number++;
        }
        classes[substrings_pos[i]] = class_number;
    }
    return  classes;
}

void GetClasses(std::vector<int>& classes,const std::vector<int>& substrings_pos,
                const std::vector<EquivalenceClass>& new_classes) {
    classes[0] = 0;
    int class_number = 0;
    for (int i = 1; i < substrings_pos.size(); i++) {
        if (new_classes[i-1].cls != new_classes[i].cls) {
            class_number++;
        }
        classes[substrings_pos[i]] = class_number;
    }

}

std::vector<EquivalenceClass> SortDoubleClasses(const std::vector<EquivalenceClass>& new_classes, int radix) {
    std::vector<int> count(new_classes.size(), 0);
    std::vector<EquivalenceClass> sorted(new_classes.size());

    for (int i = 0; i < new_classes.size(); i++) {  // счи
        count[new_classes[i].cls[radix-1]]++;
    }

    int block_start = count[0];
    count[0] = 0;
    for (int i = 1; i < count.size(); i++) {
        block_start += count[i];
        count[i] = block_start - count[i];
    }

    for (int i = 0; i < new_classes.size(); i++) {
        sorted[count[new_classes[i].cls[radix-1]]] = new_classes[i];
        count[new_classes[i].cls[radix-1]]++;
    }

    return sorted;
}

std::vector<EquivalenceClass> GetSubstringsPos(std::vector<int>& substrings_pos, const std::vector<int>& classes, int shift) {  // shift == 2^(k-1) на k-м шаге алгоритма
    std::vector<EquivalenceClass> new_classes;
    new_classes.reserve(classes.size());
    for (int i = 0; i < classes.size(); i++) {
        new_classes.emplace_back(classes[i], classes[(i + shift)%classes.size()], i);
    }

    new_classes = SortDoubleClasses(new_classes, 2);
    new_classes = SortDoubleClasses(new_classes, 1);

    for (int i = 0; i < new_classes.size(); i++) {
        substrings_pos[i] = new_classes[i].idx;
    }

    return new_classes;
}

std::vector<int> SuffixArray(const std::string& str) {
    std::string str1 = str;
    str1.push_back(' ');
    std::vector<int> substrings_pos = InitializeSubstrings(str1);
    std::vector<int> classes = InitializeClasses(substrings_pos, str1);
    std::vector<EquivalenceClass> new_classes;

    int deg = 1;
    while (deg <= str1.size()) {
        new_classes = GetSubstringsPos(substrings_pos, classes, deg);
        GetClasses(classes, substrings_pos, new_classes);
        deg *= 2;
    }
    substrings_pos.erase(substrings_pos.begin());
    return substrings_pos;
}

std::vector<int> LCP(const std::string& str, const std::vector<int>& suf_arr) {
    std::vector<int> lcp(str.size(), 0);
    std::vector<int> suf_pos(str.size());
    for (int i = 0; i < str.size(); i++) {
        suf_pos[suf_arr[i]] = i;
    }
    int prev = 0;
    for (int i = 0; i < str.size(); i++) {

        if (prev > 0) {
            prev--;
        }
        if (suf_pos[i] == 0) {
            lcp[suf_pos[i]] = -1;
        } else {
            int j = suf_arr[suf_pos[i] - 1];
            while (j + prev < str.size() && i + prev < str.size() && str[j + prev] == str[i + prev]) {
                prev++;
            }
            lcp[suf_pos[i]] = prev;
        }
    }
    return lcp;
}

int DifferentSubstrings(const std::string& str) {
    std::vector<int> suf_arr = SuffixArray(str);
    std::vector<int> lcp = LCP(str, suf_arr);
    std::vector<int> suf_pos(str.size(), 0);
    for (int i = 0; i < str.size(); i++) {
        suf_pos[suf_arr[i]] = i;
    }
    int result = 0;
    for (int i = 0; i < str.size(); i++) {
        if (suf_pos[i] == 0) {
            result += str.size() - i;
        }
        else {
            result += str.size() - i - lcp[suf_pos[i]];
        }
    }
    return result;
}


int main() {
    std::string str;
    std::cin >> str;

    std::cout << DifferentSubstrings(str) << std::endl;
    return 0;
}