#pragma once
#include <algorithm>
#include <cmath>
#include <vector>

// Your code here...
const double EPS = 1e-6;
const double PI = 3.14159265;


struct Point {
    double x, y;
    Point(const double &x, const double &y) : x(x), y(y) {}
    bool operator==(const Point &a) const {
        return x - a.x < EPS and y - a.y < EPS;
    }
    bool operator!=(const Point &a) const {
        return !(*this == a);
    }
    void rotate(Point center, double angle) {
        double new_x = center.x + (x - center.x) * cos(angle * PI / 180) - (y - center.y) * sin(angle * PI / 180);
        double new_y = center.y + (x - center.x) * sin(angle * PI / 180) + (y - center.y) * cos(angle * PI / 180);
        x = new_x;
        y = new_y;
    }
    void scale(Point center, double coefficient) {
        x = center.x + coefficient * (x - center.x);
        y = center.y + coefficient * (y - center.y);
    }
};

class Line {
public:
    double a;
    double b;
    double c;

    Line(const Point &p1, const Point &p2) {
        a = p1.y - p2.y;
        b = p2.x - p1.x;
        c = p1.x * p2.y - p2.x * p1.y;
    }
    Line(const double &angle, const double &shift) : a(1), b(-angle), c(-shift) {}
    Line(const Point &p, const double &angle) {
        a = 1;
        b = -angle;
        c = -p.y + angle * p.x;
    }
    bool operator==(const Line &line) const {
        return abs(a * line.b - b * line.a) < EPS &&
               abs(a * line.c - c * line.a) < EPS &&
               abs(b * line.c - c * line.b) < EPS;
    }
    bool operator!=(const Line &line) const {
        return !(*this == line);
    }
};

double segment_length(const Point &p1, const Point &p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

void reflex(Point &p, const Line &axis) {
    double x = ((axis.b * axis.b - axis.a * axis.a) * p.x - 2 * axis.a * (p.y * axis.b + axis.c)) /
               (axis.a * axis.a + axis.b * axis.b);
    double y = ((axis.a * axis.a - axis.b * axis.b) * p.y - 2 * axis.b * (p.x * axis.a + axis.c)) /
               (axis.a * axis.a + axis.b * axis.b);
    p.x = x;
    p.y = y;
}

class Shape {
public:
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool operator==(const Shape &another) const = 0;
    virtual void rotate(Point center, double angle) = 0;
    void reflex(Point center) {
        rotate(center, 180);
    }
    bool operator!=(const Shape &another) const {
        return !(*this == another);
    }
    virtual void reflex(Line axis) = 0;
    virtual void scale(Point center, double coefficient) = 0;
};

class Polygon : public Shape {
protected:
    std::vector<Point> vertices;
public:
    Polygon(const std::vector<Point> &vec) {
        vertices = vec;
    }
    size_t verticesCount() const {
        return vertices.size();
    }
    std::vector<Point> getVertices() const {
        return vertices;
    }
    double perimeter() const override {
        double sum = 0;
        for (size_t i = 0; i < vertices.size(); ++i) {
            sum += segment_length(vertices[i], vertices[(i + 1) % vertices.size()]);
        }
        return sum;
    }
    double signed_area() const {
        double sum = 0;
        for (size_t i = 0; i < vertices.size(); ++i) {
            sum += (vertices[i].x + vertices[(i + 1) % vertices.size()].x) *
                   (vertices[i].y - vertices[(i + 1) % vertices.size()].y);
        }
        return sum / 2;
    }
    double area() const override {
        return abs(signed_area());
    }
    bool operator==(const Shape &another) const override {
        const Polygon &casted = dynamic_cast<const Polygon &>(another);
        if (vertices.size() != casted.verticesCount())
            return false;
        auto another_vertices = casted.getVertices();
        if (signed_area() * casted.signed_area() < 0)
            std::reverse(another_vertices.begin(), another_vertices.end());

        size_t size = verticesCount();
        for (size_t i = 0; i < size; ++i) {
            if (another_vertices[i] == vertices[0]) {
                for (size_t j = 1; j < size; ++j) {
                    if (vertices[j] != another_vertices[(j + i) % size])
                        return false;
                }
                return true;
            }
        }
        return false;
    }
    void rotate(Point center, double angle) override {
        for (auto &p : vertices) {
            p.rotate(center, angle);
        }
    }
    void reflex(Line axis) override {
        for (auto &p : vertices) {
            ::reflex(p, axis);
        }
    }
    void scale(Point center, double coefficient) override {
        for (auto &p : vertices) {
            p.scale(center, coefficient);
        }
    }
};

class Ellipse : public Shape {
protected:
    Point focus1, focus2;
    double a;

public:
    Ellipse(const Point &focus1, const Point &focus2, double distance) : focus1(focus1), focus2(focus2), a(distance / 2) {}
    std::pair<Point, Point> focuses() const {
        return std::make_pair(focus1, focus2);
    }
    double eccentricity() const {
        double focal_distance = segment_length(focus1, focus2) / 2;
        return focal_distance / a;
    }
    Point center() const {
        return Point((focus2.x + focus1.x) / 2, (focus2.y + focus1.y) / 2);
    }
    double perimeter() const override {
        double b = a * sqrt(1 - eccentricity() * eccentricity());
        return PI * (3 * (a + b) - sqrt((3 * a + b) * (a + 3 * b)));
    }
    double area() const override {
        return PI * a * a * sqrt(1 - eccentricity() * eccentricity());
    }
    bool operator==(const Shape &another) const override {
        const Ellipse &casted = dynamic_cast<const Ellipse &>(another);
        return focus1 == casted.focus1 and focus2 == casted.focus2 and a == casted.a;
    }
    void rotate(Point center, double angle) override {
        focus1.rotate(center, angle);
        focus2.rotate(center, angle);
    }
    void reflex(Line axis) override {
        ::reflex(focus1, axis);
        ::reflex(focus2, axis);
    }
    void scale(Point center, double coefficient) override {
        focus1.scale(center, coefficient);
        focus2.scale(center, coefficient);
        a *= coefficient;
    }
};

class Circle : public Ellipse {
public:
    Circle(const Point &center, double radius) : Ellipse(Point(center.x, center.y),
                                                         Point(center.x, center.y),
                                                         2 * radius) {}
    double radius() const {
        return a;
    }
};

class Rectangle : public Polygon {
public:
    Rectangle(const Point &p1, const Point &p2, double k) : Polygon({p1}) {
        if (k < 1)
            k = 1 / k;
        vertices.push_back(Point(p1.x + (p2.x - p1.x) / sqrt(k * k + 1), p1.y + (p2.y - p1.y) / sqrt(k * k + 1)));
        vertices.push_back(p2);
        vertices.push_back(Point(p2.x - (p2.x - p1.x) / sqrt(k * k + 1), p2.y - (p2.y - p1.y) / sqrt(k * k + 1)));
    }
    Point center() const {
        return Point((vertices[0].x + vertices[2].x) / 2, (vertices[0].y + vertices[2].y) / 2);
    }
    std::pair<Line, Line> diagonals() const {
        return std::make_pair(Line(vertices[0], vertices[2]), Line(vertices[1], vertices[3]));
    }
};

class Square : public Rectangle {
public:
    Square(const Point &p1, const Point &p2) : Rectangle(p1, p2, 1) {}
    Circle circumscribedCircle() const {
        return Circle(center(), segment_length(vertices[0], vertices[2]) / 2);
    }
    Circle inscribedCircle() const {
        return Circle(center(), segment_length(vertices[0], vertices[1]) / 2);
    }
};

class Triangle : public Polygon {
public:
    Triangle(const Point &p1, const Point &p2, const Point &p3) : Polygon({p1, p2, p3}) {}
    Circle circumscribedCircle() const {
        double center_x = -0.5 * (vertices[0].y * (pow(vertices[1].x, 2) + pow(vertices[1].y, 2) - pow(vertices[2].x, 2) - pow(vertices[2].y, 2)) + vertices[1].y * (pow(vertices[2].x, 2) + pow(vertices[2].y, 2) - pow(vertices[0].x, 2) - pow(vertices[0].y, 2)) + vertices[2].y * (pow(vertices[0].x, 2) + pow(vertices[0].y, 2) - pow(vertices[1].x, 2) - pow(vertices[1].y, 2))) /
                          (vertices[0].x * (vertices[1].y - vertices[2].y) + vertices[1].x * (vertices[2].y - vertices[0].y) + vertices[2].x * (vertices[0].y - vertices[1].y));
        double center_y = 0.5 * (vertices[0].x * (pow(vertices[1].x, 2) + pow(vertices[1].y, 2) - pow(vertices[2].x, 2) - pow(vertices[2].y, 2)) + vertices[1].x * (pow(vertices[2].x, 2) + pow(vertices[2].y, 2) - pow(vertices[0].x, 2) - pow(vertices[0].y, 2)) + vertices[2].x * (pow(vertices[0].x, 2) + pow(vertices[0].y, 2) - pow(vertices[1].x, 2) - pow(vertices[1].y, 2))) /
                          (vertices[0].x * (vertices[1].y - vertices[2].y) + vertices[1].x * (vertices[2].y - vertices[0].y) + vertices[2].x * (vertices[0].y - vertices[1].y));
        double radius = segment_length(vertices[0], vertices[1]) * segment_length(vertices[1], vertices[2]) * segment_length(vertices[2], vertices[0]) /
                        (4 * area());
        return Circle(Point(center_x, center_y), radius);
    }
    Circle inscribedCircle() const {
        double a = segment_length(vertices[1], vertices[2]);
        double b = segment_length(vertices[0], vertices[2]);
        double c = segment_length(vertices[0], vertices[1]);
        Point center = Point((a * vertices[0].x + b * vertices[1].x + c * vertices[2].x) / (a + b + c),
                             (a * vertices[0].y + b * vertices[1].y + c * vertices[2].y) / (a + b + c));
        return Circle(center, 2 * area() / perimeter());
    }
    Point centroid() const {
        return Point((vertices[0].x + vertices[1].x + vertices[2].x) / 3,
                     (vertices[0].y + vertices[1].y + vertices[2].y) / 3);
    }
    Point orthocenter() const {
        Point circum_circle_center = circumscribedCircle().center();
        return Point(vertices[0].x + vertices[1].x + vertices[2].x - 2 * circum_circle_center.x,
                     vertices[0].y + vertices[1].y + vertices[2].y - 2 * circum_circle_center.y);
    }
    Line EulerLine() const {
        return Line(centroid(), orthocenter());
    }

    Circle ninePointsCircle() const {
        Point circum_circle_center = circumscribedCircle().center();
        Point orthocenter = this->orthocenter();
        Point center = Point((circum_circle_center.x + orthocenter.x) / 2, (circum_circle_center.y + orthocenter.y) / 2);
        return Circle(center, circumscribedCircle().radius() / 2);
    }
};