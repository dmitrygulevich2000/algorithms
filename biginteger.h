#include <iostream>
#include <vector>
#include <string>

const int BASE = 100;  // 10^BASE_DEG
const int BASE_SQRT = 10;  // 10^(BASE_DEG/2)
const int BASE_DEG = 2;

class BigInteger {
//private:
public:
    bool positive = true;
    std::vector<int> digits;  // цифры в формате LittleEndian
    // 0 задаётся пустым вектором цифр

    BigInteger(const std::vector<int>& d, int begin, int end, bool positive = true);

    static bool _ModuleLess(const std::vector<int>& x, const std::vector<int>& y);
    static std::vector<int> _ModulesSum(const std::vector<int>& x, const std::vector<int>& y);
    static std::vector<int> _ModulesDiff(const std::vector<int>& x, const std::vector<int>& y);
    // используется, чтобы вычитать из большего модуля меньший
    static BigInteger _MultiplyDigits(int x, int y);
    static std::pair<int, BigInteger> _ElementaryDivision(const BigInteger& x, const BigInteger& y, int begin = 0, int end = BASE);
    // находит результат деления x на y в случае, если результат - "цифра". возвращает частное и остаток
    static BigInteger _MultiplyByDigit(int digit);

public:
    BigInteger() = default;
    BigInteger(int x);
    BigInteger(const BigInteger& x) = default;
    BigInteger(BigInteger&& x) = default;
    BigInteger&operator=(const BigInteger& x) = default;
    BigInteger&operator=(BigInteger&& x) = default;

    explicit operator bool() const;
    std::string toString() const;

    BigInteger& operator+=(const BigInteger& x);
    BigInteger& operator-=(const BigInteger& x);

    BigInteger& operator*=(const BigInteger& y);
    BigInteger& operator/=(const BigInteger& y);
    BigInteger& operator%=(const BigInteger& y);

    BigInteger operator-() const;
    BigInteger& operator++();  // префиксный инкремент
    BigInteger operator++(int);
    BigInteger& operator--();  // префиксный декремент
    BigInteger operator--(int);

    friend bool operator<(const BigInteger& x, const BigInteger& y);
    friend bool operator>(const BigInteger& x, const BigInteger& y);
    friend bool operator<=(const BigInteger& x, const BigInteger& y);
    friend bool operator>=(const BigInteger& x, const BigInteger& y);
    friend bool operator==(const BigInteger& x, const BigInteger& y);
    friend bool operator!=(const BigInteger& x, const BigInteger& y);

    friend std::ostream& operator<<(std::ostream& out, const BigInteger& x);
    friend std::istream& operator>>(std::istream& in, BigInteger& x);

    friend BigInteger operator+(const BigInteger& x, const BigInteger& y);
    friend BigInteger operator-(const BigInteger& x, const BigInteger& y);
    friend BigInteger operator*(const BigInteger& x, const BigInteger& y);
    friend BigInteger operator/(const BigInteger& x, const BigInteger& y);
    friend BigInteger operator%(const BigInteger& x, const BigInteger& y);
};

BigInteger::BigInteger(const std::vector<int>& d, int begin, int end, bool positive): positive(positive) {
    // цифры в d в диапазоне [begin;end) лежат в формате LittleEndian;
    int i = std::max(0, begin);
    for (; i < end && i < int(d.size()); ++i) {
        digits.push_back(d[i]);
    }
}

BigInteger::BigInteger(int x) {
    positive = (x >= 0);
    if (x < 0) x = -x;
    while (x > 0) {
        digits.push_back(x % BASE);
        x /= BASE;
    }
}

BigInteger::operator bool() const {
    return !digits.empty();
}

std::string BigInteger::toString() const {
    if (digits.empty()) return "0";
    std::string str;
    if (!positive) str = "-";

    str += std::to_string(*(digits.end() - 1));
    for (int i = digits.size() - 2; i >= 0; --i) {
        std::string digit_str = std::to_string(digits[i]);
        for (int j = 0; j < BASE_DEG - int(digit_str.size()); ++j) {
            str += "0";
        }
        str += digit_str;
    }
    return str;
}

bool BigInteger::_ModuleLess(const std::vector<int>& x, const std::vector<int>& y) {
    if (x.size() < y.size()) return true;
    if (x.size() > y.size()) return false;
    for (int i = x.size() - 1; i >= 0; --i) {
        if (x[i] < y[i]) return true;
        if (x[i] > y[i]) return false;
    }
    return false;
}

bool operator<(const BigInteger& x, const BigInteger& y) {
    if (x.digits.empty() && y.digits.empty()) return false;
    if (x.digits.empty()) return y.positive;
    if (y.digits.empty()) return !x.positive;

    if (x.positive) {
        if (!y.positive) return false;
        else return BigInteger::_ModuleLess(x.digits, y.digits);
    } else {
        if (y.positive) return true;
        else return !BigInteger::_ModuleLess(x.digits, y.digits);
    }
}

bool operator>(const BigInteger& x, const BigInteger& y) {
    return y < x;
}

bool operator<=(const BigInteger& x, const BigInteger& y) {
    return !(y < x);
}

bool operator>=(const BigInteger& x, const BigInteger& y) {
    return !(x < y);
}

bool operator==(const BigInteger& x, const BigInteger& y) {
    return !(x < y) && !(y < x);
}

bool operator!=(const BigInteger& x, const BigInteger& y) {
    return (x < y) || (y < x);
}

std::ostream& operator<<(std::ostream& out, const BigInteger& x) {
    if (x.digits.empty()) {
        out << "0";
        return out;
    }
    if (!x.positive) {
        out << "-";
    }
    out << *(x.digits.end() - 1);
    for (int i = x.digits.size() - 2; i >=0; --i) {
        int digit = x.digits[i];
        int deg = BASE / 10;

        for (int j = 0; j < BASE_DEG; ++j) {
            out << (digit / deg) % 10;
            deg /= 10;
        }
    }
    return out;
}

std::istream& operator>>(std::istream& in, BigInteger& x) {
    bool positive = true;
    std::string str;
    in >> str;

    if (str.empty()) return in;  // пустой ввод

    int begin = 0;
    if (str[0] == '-') {
        positive = false;
        begin = 1;
    }
    while (str[begin] == '0') ++begin;  // обработка ведущих нулей
    if (begin == 1 - positive && (begin == int(str.size()) || str[begin] < '0' || str[begin] > '9')) {
        return in;  // на входе не число
    }

    x.digits.clear();
    if (begin == int(str.size()) || str[begin] < '0' || str[begin] > '9') return in; // на входе 0

    int end = begin + 1;
    while (end < int(str.size()) && '0' <= str[end] && str[end] <= '9') ++end;
    while (end > begin) {
        int digit = 0;
        int deg = 1;
        for (int i = 0; (i < BASE_DEG) && (end > begin); ++i) {
            digit += (str[end - 1] - '0')*deg;
            deg *= 10;
            end--;
        }
        x.digits.push_back(digit);
    }
    x.positive = positive;
    return in;
}

std::vector<int> BigInteger::_ModulesSum(const std::vector<int>& x, const std::vector<int>& y) {
    std::vector<int> result;
    int in_mind = 0;
    for (int i = 0; i < int(x.size()) || i < int(y.size()); ++i) {
        int number = in_mind;
        if (i < int(x.size())) number += x[i];
        if (i < int(y.size())) number += y[i];

        if (number >= BASE) {
            in_mind = 1;
            number -= BASE;
        } else {
            in_mind = 0;
        }
        result.push_back(number);
    }
    if (in_mind == 1) {
        result.push_back(1);
    }
    return result;
}

std::vector<int> BigInteger::_ModulesDiff(const std::vector<int>& x, const std::vector<int>& y) {
    // гарантируется, что модуль x не меньше модуля y
    std::vector<int> result;
    int in_mind = 0;
    bool is_zero = true;
    for (int i = 0; i < int(x.size()) || i < int(y.size()); ++i) {
        int number = in_mind;
        if (i < int(x.size())) number += x[i];
        if (i < int(y.size())) number -= y[i];

        if (number < 0) {
            in_mind = -1;
            number += BASE;
        } else {
            in_mind = 0;
        }
        if (number != 0) is_zero = false;
        result.push_back(number);
    }
    if (is_zero) result.clear();
    while(!result.empty() && *(result.end()-1) == 0) result.pop_back();
    return result;
}

BigInteger& BigInteger::operator+=(const BigInteger &x) {
    if (!(positive ^ x.positive)) {  // числа одного знака
        digits = BigInteger::_ModulesSum(digits, x.digits);
        return *this;
    }
    if (positive && !x.positive) {  // *this > 0
        if (!BigInteger::_ModuleLess(digits, x.digits)) {  // |*this| >= |x|
            digits = BigInteger::_ModulesDiff(digits, x.digits);
        } else {  // |*this| < |x|
            positive = false;
            digits = BigInteger::_ModulesDiff(x.digits, digits);
        }
    } else {  // *this <= 0
        if (!BigInteger::_ModuleLess(x.digits, digits)) {  // |*this| <= |x|
            positive = true;
            digits = BigInteger::_ModulesDiff(x.digits, digits);
        } else {  // |*this| > |x|
            digits = BigInteger::_ModulesDiff(digits, x.digits);
        }
    }
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger &x) {
    if (positive ^ x.positive) {  // числа разных знаков
        digits = BigInteger::_ModulesSum(digits, x.digits);
        return *this;
    }
    if (positive && x.positive) {  // *this > 0
        if (!BigInteger::_ModuleLess(digits, x.digits)) {  // |*this| >= |x|
            digits = BigInteger::_ModulesDiff(digits, x.digits);
        } else {  // |*this| < |x|
            positive = false;
            digits = BigInteger::_ModulesDiff(x.digits, digits);
        }
    } else {  // *this <= 0
        if (!BigInteger::_ModuleLess(x.digits, digits)) {  // |*this| <= |x|
            positive = true;
            digits = BigInteger::_ModulesDiff(x.digits, digits);
        } else {  // |*this| > |x|
            digits = BigInteger::_ModulesDiff(digits, x.digits);
        }
    }
    return *this;
}

BigInteger operator+(const BigInteger& x, const BigInteger& y) {
    BigInteger result = x;
    result += y;
    return result;
}

BigInteger operator-(const BigInteger& x, const BigInteger& y) {
    BigInteger result = x;
    result -= y;
    return result;
}

BigInteger BigInteger::operator-() const {
    BigInteger result = *this;
    result.positive = !result.positive;
    return result;
}

BigInteger& BigInteger::operator++() {  // префиксный
    *this += 1;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger result = *this;
    ++(*this);
    return result;
}

BigInteger& BigInteger::operator--() {  // префиксный
    *this -= 1;
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger result = *this;
    --(*this);
    return result;
}

BigInteger BigInteger::_MultiplyDigits(int x, int y) {
    // P = (x0 + x1)(y0 + y1)
    // xy = x1y1*BASE + (P - x1y1 - x2y2)*BASE_SQRT + x2y2

    int x0 = x % BASE_SQRT;
    int x1 = x / BASE_SQRT;
    int y0 = y % BASE_SQRT;
    int y1 = y / BASE_SQRT;

    int P = (x0 + x1)*(y0 + y1);
    int P1 = x1*y1;
    int P0 = x0*y0;
    int P2 = P - P0 - P1;

    BigInteger result;
    int in_mind = 0;

    // младшая цифра
    if (P0 + (P2 % BASE_SQRT)*BASE_SQRT >= BASE) in_mind = 1;
    result.digits.push_back((P0 + (P2 % BASE_SQRT)*BASE_SQRT)%BASE);

    // старшая цифра
    if (P1 + (P2 / BASE_SQRT) + in_mind != 0) {
        result.digits.push_back(P1 + (P2 / BASE_SQRT) + in_mind);
    } else if (result.digits[0] == 0) {
        result.digits.clear();
    }

    return result;
}

BigInteger& BigInteger::operator*=(const BigInteger &y) {
    if (*this == 0 || y == 0) {
        digits.clear();
        positive = true;
        return *this;
    }
//    std::cout << str <<*this << " * " << y << std::endl;

    int max_size = std::max(digits.size(), y.digits.size());
    if (max_size == 1) {
        bool p = !(positive ^ y.positive);
        BigInteger result = BigInteger::_MultiplyDigits(digits[0], y.digits[0]);
        positive = p;
        digits = result.digits;
//        std::cout << "Simple: " << *this << std::endl;
        return *this;
    }

    // конструктор работает корректно даже если диапазон вышел за границы вектора digits
    BigInteger P0(digits, 0, max_size/2);  // x0
    BigInteger P1(digits, max_size/2, max_size);  // x1
    BigInteger y0(y.digits, 0, max_size/2);
    BigInteger y1(y.digits, max_size/2, max_size);

//    std::cout << str << x1 << " " << x0 << " ; " << y1 << " " << y0 << std::endl;
//    str += "  ";

    BigInteger P = (P0 + P1);
    P *= (y0 + y1);
    P1 *= y1;
    P0 *= y0;

//    str.resize(str.size() - 2);

    BigInteger P2 = P - P1 - P0;
    // P1 *= 10^(max_size);
    if (P1 != 0) {
        for (int i = 0; i < (max_size/2)*2; ++i) {
            P1.digits.insert(P1.digits.begin(), 0);
        }
    }
    // P2 *= 10^(max_size/2)
    if (P2 != 0) {
        for (int i = 0; i < max_size/2; ++i) {
            P2.digits.insert(P2.digits.begin(), 0);
        }
    }

    bool p = !(positive ^ y.positive);
//    std::cout << str << "P = " << P << std::endl;
//    std::cout << str << *this << " * " << y << " = "
//    << P1 << " + " << P2 << " + " << P0 << " = " << P0 + P1 + P2 << std::endl;
    BigInteger result = P0 + P1 + P2;
    positive = p;
    digits = result.digits;
    return *this;
}

BigInteger operator*(const BigInteger &x, const BigInteger &y) {
    BigInteger result = x;
    result *= y;
    return result;
}

std::pair<int, BigInteger> BigInteger::_ElementaryDivision(const BigInteger &x, const BigInteger &y, int begin, int end) {
    int m = (end + begin)/2;
    BigInteger product = y * m;
    BigInteger mod = x - product;
    if (mod < 0) {
        return _ElementaryDivision(x, y, begin, m);
    } else if (mod < y) {
        return std::make_pair(m, mod);
    } else {
        return _ElementaryDivision(x, y, m, end);
    }
}

//BigInteger& BigInteger::operator/=(const BigInteger &y) {
//    std::vector<int> result;
//    bool sign = !(positive ^ y.positive);
//    int pos = digits.size() - y.digits.size();
//
//    BigInteger divider;
//    if (y < 0) divider = -y;
//    else divider = y;
//
//    BigInteger divident(digits, pos, digits.size());
////    std::cout << "divident1: " << divident << " y: " << y << std::endl;
//    if (divident < divider) {
//        --pos;
//        if (pos < 0) {
//            positive = true;
//            digits.clear();
//            return *this;
//        }
//        divident.digits.insert(divident.digits.begin(), digits[pos]);
//    }
//
////    std::cout << "divident2: " << divident << std::endl;
//    while (pos > 0) {
//        std::pair<int, BigInteger> step = BigInteger::_ElementaryDivision(divident, divider);
//        //       std::cout << "div = " << step.first << " mod = " << step.second << " pos = " << pos << std::endl;
//        // step = <цифра результата, остаток>
//
//        result.insert(result.begin(), step.first);
//        --pos;
//        divident = step.second;
//        divident.digits.insert(divident.digits.begin(), digits[pos]);
//
//        while (pos >= 0 && divident < divider) {
//            result.insert(result.begin(), 0);
//            if (pos == 0) {
//                digits = result;
//                positive = sign;
//                return *this;
//            }
//            --pos;
//            divident.digits.insert(divident.digits.begin(), digits[pos]);
//        }
//    }
//    std::pair<int, BigInteger> last_step = BigInteger::_ElementaryDivision(divident, divider);
//    if (last_step.first != 0) result.insert(result.begin(), last_step.first);
//
//    digits = result;
//    positive = sign;
//    return *this;
//}

BigInteger& BigInteger::operator/=(const BigInteger &y) {
    BigInteger result = 0;
    bool sign = !(positive ^ y.positive);

    BigInteger divident;
    if (positive) divident = *this;
    else divident = -(*this);

    BigInteger divider;
    if (y.positive) divider = y;
    else divider = -y;

    if (divident < divider) {
        *this = 0;
        return *this;
    }
//    std::cout << divident << " " << divider << std::endl;
    while (divident >= divider) {
        int size_diff = divident.digits.size() - divider.digits.size() - 1;
        if (*(divider.digits.end() - 1) < *(divident.digits.end() - 1)) size_diff++;
        BigInteger higher_divider = divider;
        BigInteger addition = 1;
        for (int i = 0; i < size_diff; ++i) {
            higher_divider.digits.insert(higher_divider.digits.begin(), 0);
            addition.digits.insert(addition.digits.begin(), 0);
        }
        while (divident >= higher_divider) {
            result += addition;
            divident -= higher_divider;
        }
    }

    *this = result;
    positive = sign;
    return *this;
}

BigInteger operator/(const BigInteger& x, const BigInteger& y) {
    BigInteger result = x;
    result /= y;
    return result;
}

BigInteger& BigInteger::operator%=(const BigInteger &y) {
    BigInteger div = *this / y;
    *this -= y * div;
    return *this;
}

BigInteger operator%(const BigInteger& x, const BigInteger& y) {
    BigInteger result = x;
    result %= y;
    return result;
}