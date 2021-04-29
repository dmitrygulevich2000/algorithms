#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

const double PRECISION = 1e-7;

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
    bool IsCollinear(const MyVector3D& v) const;
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

bool MyVector3D::IsCollinear(const MyVector3D &v) const {
    if (x/v.x == y/v.y && x/v.x == z/v.z) {
        return true;
    } else {
        return false;
    }
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


double Point2SegmDistance(const MyVector3D& segm_begin, const MyVector3D& segm_end, const MyVector3D& point) {
    if (DotProduct(segm_end - segm_begin, point - segm_begin) <= 0) {
        MyVector3D res = segm_begin - point;
        return res.Length();
    }
    if (DotProduct(segm_begin - segm_end, point - segm_end) <= 0) {
        MyVector3D res = segm_end - point;
        return res.Length();
    }

    MyVector3D vect_prod = VectorProduct(segm_end - segm_begin, point - segm_begin);
    MyVector3D segm = segm_end - segm_begin;
    return vect_prod.Length() / segm.Length();
}

double Segm2SegmDistance(const MyVector3D& b1, const MyVector3D& e1,
                         const MyVector3D& b2, const MyVector3D& e2) {
    MyVector3D segm1 = e1 - b1;
    if (segm1.Length() < PRECISION * 1e-3) {  // при вычислении расстояния от точки до отрезка погрешность накапливается
        MyVector3D m = b1 + 1./2. * (e1 - b1);
        return Point2SegmDistance(b2, e2, m);
    }

    MyVector3D m1 = b1 + 1./3. * (e1 - b1);
    MyVector3D m2 = b1 + 2./3. * (e1 - b1);

    if (Point2SegmDistance(b2, e2, m1) < Point2SegmDistance(b2, e2, m2)) {
        return Segm2SegmDistance(b1, m2, b2, e2);
    } else {
        return Segm2SegmDistance(m1, e1, b2, e2);
    }

}

int main() {
    double x, y, z;
    std::cin >> x >> y >> z;
    MyVector3D b1(x, y, z);
    std::cin >> x >> y >> z;
    MyVector3D e1(x, y, z);

    std::cin >> x >> y >> z;
    MyVector3D b2(x, y, z);
    std::cin >> x >> y >> z;
    MyVector3D e2(x, y, z);

    std::cout << std::setprecision(10) << Segm2SegmDistance(b1, e1, b2, e2);
    return 0;
}