#include <iostream>
#include <vector>
#include <cmath>

enum Orientation {
    right = 0,
    left = 1,
    collinear = 2
};

class MyVector2D {
public:
    double x;
    double y;

    MyVector2D() = default;
    MyVector2D(const MyVector2D& v) = default;
    MyVector2D(double x, double y): x(x), y(y) {}

    double Length() const;
    MyVector2D& operator=(const MyVector2D& v) = default;

    MyVector2D& operator-=(const MyVector2D& v);
    MyVector2D operator-(const MyVector2D& v) const;
    MyVector2D& operator+=(const MyVector2D& v);
    MyVector2D operator+(const MyVector2D& v) const;
    MyVector2D operator-();

    friend double Cosine(MyVector2D v1, MyVector2D v2);
    friend Orientation GetOrientation(MyVector2D v1, MyVector2D v2);
};

MyVector2D& MyVector2D::operator-=(const MyVector2D &v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

MyVector2D MyVector2D::operator-(const MyVector2D &v) const {
    return MyVector2D(x - v.x, y-v.y);
}

MyVector2D& MyVector2D::operator+=(const MyVector2D &v) {
    x += v.x;
    y += v.y;
    return *this;
}

MyVector2D MyVector2D::operator+(const MyVector2D &v) const {
    return MyVector2D(x + v.x, y + v.y);
}

MyVector2D MyVector2D::operator-() {
    return MyVector2D(-x, -y);
}

double Cosine(MyVector2D v1, MyVector2D v2) {
    double dot_product = v1.x*v2.x + v1.y*v2.y;

    return dot_product / (v1.Length() * v2.Length());
}

Orientation GetOrientation(MyVector2D v1, MyVector2D v2) {
    double oriented_area = v1.x*v2.y - v1.y*v2.x;
    if (oriented_area > 0) {
        return right;
    } else if (oriented_area < 0) {
        return left;
    } else {
        return collinear;
    }
}

double MyVector2D::Length() const {
    return std::sqrt(x*x + y*y);
}

std::ostream& operator<<(std::ostream& out, const MyVector2D& v) {
    out << "(" << v.x << " ; " << v.y << ")";
    return out;
}

template<class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    for (const auto& i: v ) {
        out << i << " ";
    }
    return out;
}

bool ComparePolarAngles(MyVector2D v1, MyVector2D v2, MyVector2D start = MyVector2D(1, 0), bool clockwise = true) {
    // сравнивает полярные углы v1 и v2 при повороте по/против часовой стелки отностиельно направления start

    if (GetOrientation(v2, start) == collinear && Cosine(v2, start) > 0) {  // полярный угол 2го вектора равен 0
        return false;
    }
    if (GetOrientation(v1, start) == collinear && Cosine(v1, start) > 0) { // полярный угол 1го вектора равен 0
        return true;
    }

    bool first_less_than_180 = false;
    bool second_less_than_180 = false;
    if (GetOrientation(v1, start) == right) {
        first_less_than_180 = true;
    }
    if (GetOrientation(v2, start) == right) {
        second_less_than_180 = true;
    }
    if (!clockwise) {
        first_less_than_180 = !first_less_than_180;
        second_less_than_180 = !second_less_than_180;
    }

    // случай угла, равного 180
    if (GetOrientation(v1, start) == collinear) {
        first_less_than_180 = false;
    }
    if (GetOrientation(v2, start) == collinear) {
        second_less_than_180 = false;
    }

    if (first_less_than_180 && second_less_than_180) {
        return Cosine(v1, start) > Cosine(v2, start);
    } else if (first_less_than_180 && !second_less_than_180) {
        return true;
    } else if (!first_less_than_180 && second_less_than_180) {
        return false;
    } else {
        return Cosine(v1, start) < Cosine(v2, start);
    }
}

std::vector<MyVector2D> MinkowskiSum(const std::vector<MyVector2D>& p1, const std::vector<MyVector2D>& p2) {
    // предполагается, что точки в p1 и p2 отсортированы по часовой стрелке, начиная с самой нижней-правой
    std::vector<MyVector2D> result;
    int i = 0;
    int j = 0;

    while (i < p1.size() || j < p2.size()) {
        result.emplace_back(p1[i%p1.size()]+p2[j%p2.size()]);

        if (i == p1.size()) {
            j++;
            continue;
        }
        if (j == p2.size()) {
            i++;
            continue;
        }

        MyVector2D next1 = p1[(i+1)%p1.size()] - p1[i];
        MyVector2D next2 = p2[(j+1)%p2.size()] - p2[j];
        if (ComparePolarAngles(next1, next2, MyVector2D(-1, 0), true)) {
            i++;
        } else if (ComparePolarAngles(next2, next1, MyVector2D(-1, 0), true)) {
            j++;
        } else {
            i++;
            j++;
        }
    }
    return result;
}

bool DoesContain(const std::vector<MyVector2D>& polygon, MyVector2D point) {
    // проверяет, содержит ли многоугольник polygon точку point
    // предполагается, что вершины многоугольника отсортированы по часовой стрелке начиная с самой нижней-правой
    for (int i = 0; i < polygon.size(); i++) {
        MyVector2D edge = polygon[(i+1)%polygon.size()] - polygon[i];
        MyVector2D to_point = point - polygon[i];
        if (GetOrientation(edge, to_point) == right) {
            return false;
        }
    }
    return true;
}

std::vector<MyVector2D> RotatedPolygon(const std::vector<MyVector2D>& p) {
    // вращает многоугольник так, что первой оказывается самая нижняя-правая вершина
    int start_point = 0;
    for (int i = 1; i < p.size(); ++i) {  // найдём самую нижнюю-правую точку в p2
        if (p[i].y < p[start_point].y) {
            start_point = i;
        } else if (p[i].y == p[start_point].y && p[i].x > p[start_point].x) {
            start_point = i;
        }
    }
    std::vector<MyVector2D> result;
    for (int i = start_point; i < p.size(); ++i) {
        result.emplace_back(p[i]);
    }
    for (int i = 0; i < start_point; ++i) {
        result.emplace_back(p[i]);
    }
    return result;
}

bool DoIntersect(const std::vector<MyVector2D>& p1, const std::vector<MyVector2D>& p2) {
    std::vector<MyVector2D> p1_neg = p1;
    for (int i = 0; i < p1_neg.size(); i++) {
        p1_neg[i] = -p1_neg[i];
    }

    std::vector<MyVector2D> p1_neg_cp = RotatedPolygon(p1_neg);
    std::vector<MyVector2D> p2_cp = RotatedPolygon(p2);


    std::vector<MyVector2D> minkowski_sum = MinkowskiSum(p1_neg_cp, p2_cp);
    return DoesContain(minkowski_sum, MyVector2D(0, 0));
}


int main() {
    int n = 0;
    double x, y;

    std::cin >> n;
    std::vector<MyVector2D> p1;
    for (int i = 0; i < n; ++i) {
        std::cin >> x >> y;
        p1.emplace_back(x, y);
    }

    std::cin >> n;
    std::vector<MyVector2D> p2;
    for (int i = 0; i < n; ++i) {
        std::cin >> x >> y;
        p2.emplace_back(x, y);
    }

    if (DoIntersect(p1, p2)) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }

    return 0;
}