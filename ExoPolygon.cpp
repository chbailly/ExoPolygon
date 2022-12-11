// ExoPolygon.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <numeric>

struct Point {
    Point() = default;
    Point(double input_x, double input_y) :x(input_x), y(input_y) {}
    double x;
    double y;
};

double distance2(const Point& p1, const Point& p2) {
    double x = p2.x - p1.x;
    double y = p2.y - p1.y;
    return x * x + y * y;
}

double distance(const Point& p1, const Point& p2) {
    return sqrt(distance2(p1, p2));
}

struct Vector {
    double x;
    double y;
    Vector() : x(0), y(0) {}
    Vector(double i_x, double i_y) : x(i_x), y(i_y) {}
    double norm() {
        return sqrt(x * x + y * y);
    }
};

Vector operator*(double m, const Vector& v) {
    return Vector(m * v.x, m * v.y);
}

struct Segment {
    Segment() = default;
    Segment(Point input_p1, Point input_p2): p1(input_p1), p2(input_p2) {}
    Point p1;
    Point p2;
    Vector getVector() {
        return Vector(p2.x - p1.x, p2.y - p1.y);
    }
};



struct Polygon {
    Polygon(const char* filename) {
        std::ifstream is(filename);
        int x, y;
        int k = 0;
        int size;
        is >> size;
        points.reserve(size);
        while (!is.eof()) {
            is >> x;
            is >> y;
            points.emplace_back(x, y);
        }
    }

    auto len() {
        return points.size();
    }

    Segment side(int i) const {
        return Segment(points[i], points[i+1 >= points.size() ? 0:i+1]);
    }

    double perimetre() {
        double perimeter{ 0 };
        for (auto it = points.begin(); it != points.end(); it++) {
            if (it + 1 != points.end())
                perimeter += distance(*it, *(it + 1));
            else
                perimeter += distance(*it, *points.begin());
        }
        return perimeter;
    }

    std::vector<Point> points;
};

Point operator+(const Point& p, const Vector& v) {
    return Point(
        p.x + v.x,
        p.y + v.y
    );
}

bool intersect(Segment s1, Segment s2) {
    const auto& p1 = s1.p1;
    const auto& p2 = s1.p2;
    const auto& p3 = s2.p1;
    const auto& p4 = s2.p2;

    double slope1 = (p2.y - p1.y) / (p2.x - p1.x);
    double slope2 = (p4.y - p3.y) / (p4.x - p3.x);

    double y_intercept1 = p1.y - slope1 * p1.x;
    double y_intercept2 = p3.y - slope2 * p3.x;

    if (slope1 == slope2 && y_intercept1 != y_intercept2)
        return false;

    if (slope1 != slope2) {
        double x, y;
        x = (y_intercept2 - y_intercept1) / (slope1 - slope2);

        y = slope1 * x + y_intercept1;
 
        if ((p1.x <= x && x <= p2.x && p1.y <= y  && y <= p2.y) || (p2.x <= x && x <= p1.x && p2.y <= y && y <= p1.y))
            return true;
        else
            return false;

    }
    return true;
}


std::ostream& operator << (std::ostream& os, const Point& p) {
    os << p.x << " " << p.y;
    return os;
}

std::ostream& operator << (std::ostream& os, const Polygon& poly) {
    for (auto& p : poly.points)
        os << p << std::endl;
    return os;
}

std::ostream& operator << (std::ostream& os, const Segment& s) {
    os << s.p1 << ", " << s.p2;
    return os;
}

Vector step_vector(const Point& p1, const Point& p2, double step) {
    return  (step / distance(p1, p2)) * Segment(p1, p2).getVector() ;
}

struct Position {
    Position(const Polygon& i_p, int vertex, double input_step):p(i_p) {
        point = p.points[vertex];
        i_segment = vertex;
        i_step = 0;
        auto v = p.side(vertex).getVector();
        double norm = v.norm();
        step = input_step;
        nb_steps = (unsigned int)(1 +  norm / input_step);
        step_vector = (step / norm) * v;
        end = false;     
    }

    void next() {
        if (i_step == nb_steps - 1) {
            i_step = 0;
            i_segment++;
            if (i_segment < p.points.size()) {
                point = p.points[i_segment]; // reset to start of next side
                auto v = p.side(i_segment).getVector();
                double norm = v.norm();
                nb_steps = (unsigned int)(1 + norm / step);
                step_vector = (step / norm) * v;
            }
        }
        else {
            i_step++;
            point = point + step_vector;
        }
    }

    bool before_end_polygon() {
        return (i_segment == p.points.size()-1);
    }

    bool end_polygon() {
        return (i_segment == p.points.size());
    }

    Point point;
    unsigned int i_segment; // index of segement in polygon
    unsigned int i_step;

    Vector step_vector;
    unsigned int nb_steps;
    const Polygon& p;
    bool end;
    double step;
};

std::ostream& operator  << (std::ostream& os, const Position& p) {
    os << p.i_segment << ": " << p.point;
    return os;
}

int main()
{
    int nb_points = 10000;
    Polygon p("input.txt");
    //std::cout << p << std::endl;
    //std::cout << "perimeter:" << p.perimetre() << std::endl;

    double step = p.perimetre() / nb_points;

    Position start_position(p, 0, step);
    Position end_position(p, 1, step);
    double max_d2 = 0;

    Segment result_segment;
    char c;
    while(!start_position.before_end_polygon()) {
        //std::cout << "Start position:" << start_position << std::endl;
        Position end_position(p, start_position.i_segment + 1, step);
        while (!end_position.end_polygon()) {
            //std::cout << "End position:" << end_position << std::endl;
            double d = distance2(start_position.point, end_position.point);
            if (d > max_d2) {
                Segment s(start_position.point, end_position.point);
                bool inside = true;
                for (int k = 0; k < p.points.size(); k++) { //iterate over sides of polygon
                    if ((k == start_position.i_segment) || (k == end_position.i_segment))
                        continue;
                    if (intersect(s, p.side(k))) {
                        inside = false;
                        break;
                    }
                }
                if (inside) {
                    max_d2 = d;
                    result_segment = Segment(start_position.point, end_position.point);
                }
            }
            //std::cin.ignore();
            end_position.next();
        }
        start_position.next();
    }
    std::cout << result_segment << std::endl;
    std::cout << sqrt(max_d2) << std::endl;

}

