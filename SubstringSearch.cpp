#include <iostream>
#include<vector>
#include<string>


std::vector<int> ZFunction(const std::string& s) {
    std::vector<int> result(s.size());
    result[0] = s.size();
    int left = 0;
    int right = 0;
    for (int i = 1; i < s.size(); ++i) {
        if (i > right) {

            int len = 0;  // наивный поиск Z(i)
            while (len < s.size() && s[len] == s[i + len]) {
                len += 1;
            }
            result[i] = len;

            if (len > 0) {  // обновление left и right
                right = i + len - 1;
                left = i;
            }

        } else {  // i <= right
            if (result[i - left] + i <= right) {
                result[i] = result[i - left];
            } else {

                int len = right - i + 1;  // наивный поиск Z(i) >= right - i
                while (len < s.size() && s[len] == s[i + len]) {
                    len += 1;
                }
                result[i] = len;

                right = i + len - 1;  // обновление left и right
                left = i;
            }
        }
    }
    return result;
}

std::vector<int> PrefixFunction(const std::string& s) {
    std::vector<int> result(s.size());
    result[0] = 0;
    for (int i = 1; i < s.size(); i++) {
        int prev = result[i - 1];
        while (true) {
            if (s[i] == s[prev]) {
                result[i] = prev + 1;
                break;
            } else if (prev == 0) {
                result[i] = 0;
                break;
            }
            prev = result[prev - 1];
        }
    }
    return result;
}

std::vector<int> SubstringSearch_Z(const std::string& str, const std::string& text) {
    // возвращает вектор, содержащий позиции начала вхождения str в text
    std::vector<int> z_func = ZFunction(str);
    std::vector<int> result(0);
    int left = -1;
    int right = -1;
    for (int i = 0; i < int(text.size()) - int(str.size()) + 1; ++i) {
        int Zi = 0;
        if (i > right) {

            int len = 0;  // наивный поиск Z(i)
            while (len < str.size() && str[len] == text[i + len]) {
                len += 1;
            }
            Zi = len;

            if (Zi > 0) {  // обновление left и right
                right = i + Zi - 1;
                left = i;
            }

        } else {  // i <= right

            if (z_func[i - left] + i <= right) {  // заметим, что i - left <= right - left < str.size()
                Zi = z_func[i - left];
            } else {

                int len = right - i + 1;  // наивный поиск Z(i) >= right - i
                while (len < str.size() && str[len] == text[i + len]) {
                    len += 1;
                }
                Zi = len;

                right = i + Zi - 1;  // обновление left и right
                left = i;
            }
        }

        if (Zi == str.size()) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> SubstringSearch_Prefix(const std::string& str, const std::string& text) {
    // возвращает вектор, содержащий позиции начала вхождения str в text
    std::vector<int> result;
    std::vector<int> prefix_func = PrefixFunction(str);
    int prev = 0; // предыдущее значение префикс-функции

    if (str[0] == text[0]) {
        prev = 1;
        if (str.size() == 1) {
            result.push_back(0);
        }
    }

    for (int pos = 1; pos < text.size(); ++pos) {
        while (true) {
            if (str.size() != prev) {
                if (text[pos] == str[prev]) {
                    prev += 1;
                    if (str.size() == prev) {
                        result.push_back(pos - str.size() + 1);
                    }
                    break;
                } else if (prev == 0) {
                    prev = 0;
                    break;
                }
            }
            prev = prefix_func[prev - 1];
        }
    }

    return result;
}


int main() {
    std::string str;
    std::cin >> str;
    std::string text;
    std::cin >> text;
    std::vector<int> search = SubstringSearch_Z(str, text);
    for (int i: search) {
        std::cout << i << " ";
    }

    return 0;
}