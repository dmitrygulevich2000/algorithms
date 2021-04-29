#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <set>
#include <functional>

struct Face {  // хранит номера точек составляющих грань
    int a = 0;
    int b = 0;
    int c = 0;

    Face() = default;
    Face(int a, int b, int c): a(a), b(b), c(c) {}

    constexpr bool operator<(const Face& f) const {
        return (a < f.a) || (a == f.a && b < f.b) || (a == f.a && b == f.b && c < f.c);
    }
    bool operator==(const Face& f) const {
        return a == f.a && b == f.b && c == f.c;
    }
};

std::ostream& operator<<(std::ostream& out, const Face& f) {
    out << f.a << " " << f.b << " " << f.c;
    return out;
}

struct FaceCmp {
    constexpr bool operator()(const Face& f1, const Face& f2) const {
        return f1 < f2;
    }
};


enum Orientation {
    right = 0,
    left = 1,
    coplanar = 2
};

class MyVector3D {
private:
    double x = 0;
    double y = 0;
    double z = 0;
public:
    MyVector3D() = default;
    MyVector3D(const MyVector3D& v) = default;
    MyVector3D(double x, double y, double z): x(x), y(y), z(z) {}

    double Length() const;
    bool Lower(const MyVector3D& v2) const;
    MyVector3D& operator=(const MyVector3D& v) = default;


    MyVector3D& operator-=(const MyVector3D& v);
    MyVector3D operator-(const MyVector3D& v) const;
    MyVector3D& operator+=(const MyVector3D& v);
    MyVector3D operator+(const MyVector3D& v) const;
    MyVector3D& operator*=(double a);
    MyVector3D operator*(double a) const;
    friend MyVector3D operator*(double a, const MyVector3D& v);
    friend std::ostream& operator<<(std::ostream& out, const MyVector3D& v);
    MyVector3D operator-();

    friend double DotProduct(const MyVector3D& v1, const MyVector3D& v2);
    friend MyVector3D VectorProduct(const MyVector3D& v1, const MyVector3D& v2);
    friend double Cosine(const MyVector3D& v1, const MyVector3D& v2);
    friend double OrientedVolume(const MyVector3D& v1, const MyVector3D& v2, const MyVector3D& v3);
    friend Orientation GetOrientation(const MyVector3D& v1, const MyVector3D& v2, const MyVector3D& v3);
    friend bool IsCollinear(const MyVector3D& v1, const MyVector3D& v2);

    friend Face FindFirstFace(const std::vector<MyVector3D>& points);
    friend int GetPoint(const std::vector<MyVector3D>& points, MyVector3D a, MyVector3D b, MyVector3D c, int first, int second);
};

MyVector3D& MyVector3D::operator-=(const MyVector3D &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

MyVector3D MyVector3D::operator-(const MyVector3D &v) const {
    return MyVector3D(x - v.x, y-v.y, z-v.z);
}

MyVector3D& MyVector3D::operator+=(const MyVector3D &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

MyVector3D MyVector3D::operator+(const MyVector3D &v) const {
    return MyVector3D(x + v.x, y + v.y, z + v.z);
}

MyVector3D& MyVector3D::operator*=(double a) {
    x *= a;
    y *= a;
    z *= a;
    return *this;
}

MyVector3D MyVector3D::operator*(double a) const {
    return MyVector3D(x*a, y*a, z*a);
}

MyVector3D operator*(double a, const MyVector3D& v) {
    return MyVector3D(v.x*a, v.y*a, v.z*a);
}

std::ostream& operator<<(std::ostream& out, const MyVector3D& v) {
    out << "(" << v.x << " ; " << v.y << " ; " << v.z << ")";
    return out;
}

MyVector3D MyVector3D::operator-() {
    return MyVector3D(-x, -y, -z);
}


double MyVector3D::Length() const {
    return std::sqrt(x*x + y*y + z*z);
}

bool IsCollinear(const MyVector3D &v1, const MyVector3D &v2) {
    if (v1.x/v2.x == v1.y/v2.y && v1.x/v2.x == v1.z/v2.z) {
        return true;
    } else {
        return false;
    }
}

bool MyVector3D::Lower(const MyVector3D& v) const {
    return ((z < v.z) || (z == v.z && y < v.y) || (z == v.z && y == v.y && x < v.x));
}

double DotProduct(const MyVector3D& v1, const MyVector3D& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

MyVector3D VectorProduct(const MyVector3D& v1, const MyVector3D& v2) {
    return MyVector3D(v1.y*v2.z - v1.z*v2.y, v1.x*v2.z - v1.z*v2.x, v1.x*v2.y - v1.y*v2.x);
}

double Cosine(const MyVector3D& v1, const MyVector3D& v2) {
    double dp = DotProduct(v1, v2);
    return dp / (v1.Length() * v2.Length());
}

double OrientedVolume(const MyVector3D& v1, const MyVector3D& v2, const MyVector3D& v3) {
    double res = v1.x*v2.y*v3.z + v1.z*v2.x*v3.y + v1.y*v2.z*v3.x;
    res = res - v1.z*v2.y*v3.x - v1.x*v2.z*v3.y - v1.y*v2.x*v3.z;
    return res;
}

Orientation GetOrientation(const MyVector3D& v1, const MyVector3D& v2, const MyVector3D& v3) {
    double volume = OrientedVolume(v1, v2, v3);

    if (volume > 0) {
        return right;
    } else if (volume < 0) {
        return left;
    } else {
        return coplanar;
    }
}

template<class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    for (const auto& i: v ) {
        out << i << " ";
    }
    return out;
}



int GetPoint(const std::vector<MyVector3D>& points, MyVector3D a, MyVector3D b, MyVector3D c, int first, int second) {
    // дублирование кода связано с тем, что в поиске первой грани участвуют как точки из множества, так и другие точки
    double max_cos = -1;
    int result = 0;
    MyVector3D n1 = VectorProduct(b - a, c - a);

    for (int i = 0; i < points.size(); ++i) {
        MyVector3D x = points[i];
        if (i != first && i != second) {
            if (GetOrientation(b - a, c - a, x - a) == coplanar) {
                return i;
            }

            MyVector3D n2 = VectorProduct(x - b, c - b);
            if (n2.z > 0) {
                n2 = -n2;
            }

            double cos = Cosine(n1, n2);
            if (cos > max_cos) {
                max_cos = cos;
                result = i;
            }
        }

    }
    return result;
}

Face FindFirstFace(const std::vector<MyVector3D>& points) {
    int first = 0;
    for (int i = 0; i < points.size(); ++i) {
        if (points[i].Lower(points[first])) {
            first = i;
        }
    }

    MyVector3D tmp1 = points[first] + MyVector3D(1, 0, 0);
    MyVector3D tmp2 = points[first] + MyVector3D(0, 1, 0);
    int second = GetPoint(points, tmp1, points[first], tmp2, first, -1);

    int third = 0;
    if (IsCollinear(points[second] - points[first], tmp2 - points[first])) {
        third = GetPoint(points, tmp1, points[first], points[second], first, second);
    } else if (points[second].x > tmp2.x) {
        third = GetPoint(points, tmp2, points[second], points[first], first, second);
    } else {
        third = GetPoint(points, tmp2, points[first], points[second], first, second);
    }

    MyVector3D n = VectorProduct(points[second] - points[first], points[third] - points[first]);
    if (n.z > 0) {
        std::swap(second, third);
    }
    return {first, second, third};
}

int GetNextPoint(const std::vector<MyVector3D>& points, Face f) {
    double max_cos = -1;
    int result = 0;
    MyVector3D n1 = VectorProduct(points[f.b] - points[f.a], points[f.c] - points[f.a]);

    for (int i = 0; i < points.size(); ++i) {
        MyVector3D x = points[i];
        if (i != f.a && i != f.b && i != f.c) {
            MyVector3D n2 = VectorProduct(x - points[f.b], points[f.c] - points[f.b]);
            double cos = Cosine(n1, n2);
            if (cos > max_cos) {
                max_cos = cos;
                result = i;
            }
        }
    }
    return result;
}

void AddToResult(std::set<Face, FaceCmp>& result, Face f) {
    if (f.a <= f.b && f.a <= f.c) {
        result.emplace(f.a, f.b, f.c);
    } else if (f.b < f.a && f.b < f.c) {
        result.emplace(f.b, f.c, f.a);
    } else if (f.c < f.a && f.c < f.b) {
        result.emplace(f.c, f.a, f.b);
    }
}

std::set<Face, FaceCmp> GiftWrapping(const std::vector<MyVector3D>& points) {
    std::stack<Face> st;
    // точки в гранях лежащих в стеке упорядочены так, чтобы векторное произведение
    // [b - a, c - a] давало внешнюю к оболочке нормаль
    std::set<std::pair<int, int>> used;
    // хранятся пары индексов точек, составляющих ребро
    std::set<Face, FaceCmp> result;
    // грани добавляются с тем порядком точек, который нужен при выводе ответа

    Face first = FindFirstFace(points);
    AddToResult(result, first);
    if (points.size() == 3) {
        return result;
    }

    st.emplace(first.a, first.b, first.c);
    st.emplace(first.b, first.c, first.a);
    st.emplace(first.c, first.a, first.b);
    used.insert(std::pair<int, int>(first.a, first.b));
    used.insert(std::pair<int, int>(first.b, first.c));
    used.insert(std::pair<int, int>(first.c, first.a));

    while (!st.empty()) {
        Face f = st.top();
        st.pop();

        int x = GetNextPoint(points, f);
        Face new_face(f.b, x, f.c);
        AddToResult(result, new_face);

        std::pair<int, int> new_edge1(f.c, x);
        std::pair<int, int> new_edge2(x, f.c);
        if (used.count(new_edge1) == 0 && used.count(new_edge2) == 0) {
            st.push(new_face);
            used.insert(new_edge1);
        }
        new_face = {f.c, f.b, x};
        new_edge1 = {f.b, x};
        new_edge2 = {x, f.b};
        if (used.count(new_edge1) == 0 && used.count(new_edge2) == 0) {
            st.push(new_face);
            used.insert(new_edge1);
        }
    }
    return result;
}

int main() {
    int test_count = 0;
    std::cin >> test_count;

    for (int i = 0; i < test_count; ++i) {
        std::vector<MyVector3D> points;
        int n = 0;
        std::cin >> n;

        for (int j = 0; j < n; ++j) {
            double x, y, z;
            std::cin >> x >> y >> z;
            points.emplace_back(x, y, z);
        }

        std::set<Face, FaceCmp> hull = GiftWrapping(points);

        std::cout << hull.size() << std::endl;
        for (const auto& j: hull) {
            std::cout << "3 " << j << std::endl;
        }
    }
}