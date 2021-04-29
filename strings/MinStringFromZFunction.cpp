#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

std::vector<char> alphabet;

void CopyPrefix(int& i, std::string& result, std::unordered_set<char>& banned, const std::vector<int>& z_function) {
    int length = z_function[i];  // длина копируемого Z-блока
    int end = i + length - 1;  // индекс конца копируемого Z-блока
    int pos = 1;  // позиция, с которой происходит копирование
    result += result[0];
    banned.insert(result[length]);
    while (pos < length) {
        if (i + pos + z_function[i + pos] - 1 >= end) {
            // сравниваем индекс конца копируемого Z-блока с индексом конца найденного Z-блока
            // т.е. если находим Z-блок, заканчивающийся не раньше текущего - копируем уже его

            i = i + pos;
            length = z_function[i];
            end = i + length - 1;
            pos = 0;
            if (i + z_function[i] - 1 > end) {
                // если найденный Z-блок заканчивается строго позже - очистим список забаненных букв
                banned.clear();
            }
            banned.insert(result[length]);  // запретим букву, идущую после найденного Z-блока
        }
        result += result[pos];
        pos++;
    }
    i = end;
}

std::string MinStringFromZFunction(const std::vector<int>& z_function) {
    std::string result(1, alphabet[0]);

    std::unordered_set<char> banned;
    // здесь будем сохранять буквы, которые нельзя записывать в позиции с нулевой Z-функцией

    for (int i = 1; i < z_function.size(); ++i) {
        if (z_function[i] > 0) {  // нужно скопировать префикс строки

            CopyPrefix(i, result, banned, z_function);

        } else {  // z_function[i] == 0
            int letter = 1;
            while (banned.count(alphabet[letter]) != 0) {  // ищем минимальную незабаненную букву алфавита
                letter++;
            }
            result += alphabet[letter];
            banned.clear();
        }
    }
    return result;
}


int main() {
    for (char i = 'a'; i <= 'z'; i++) {
        alphabet.push_back(i);
    }

    std::vector<int> z_function;
    int x = 0;
    while (std::cin >> x) {
        z_function.push_back(x);
    }

    std::cout << MinStringFromZFunction(z_function) << std::endl;

    return 0;
}