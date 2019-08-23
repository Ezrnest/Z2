//
// Created by liyicheng on 2019/7/3.
//

#include <cmath>
#include "Point.h"
#include <iostream>

using namespace z2;
z2::Point::Point(int x, int y) noexcept : x(x), y(y) {}

z2::Point z2::Point::operator+(const z2::Point &p) const {
    return {x + p.x, y + p.y};
}
Point Point::operator-(const Point &p) const {
    return {x - p.x, y - p.y};
}


Point::Point() noexcept : Point(-1,-1){

}

int Point::girdDistance(const Point& p) const{
    return abs(x - p.x) + abs(y - p.y);
}

void Point::serializeTo(ostream &output) {

}

const string &Point::getClassName()const {
    return className();
}

string &Point::className() {
    static string name = "Point";
    return name;
}

void Point::serializeData(ostream &output) {
    output << x << ' ' << y << ' ';
}

void Point::deserializeData(istream &input) {
    input >> x >> y;
}

bool Point::isAdjacentTo(const Point &p) const {
    int dx = x - p.x;
    int dy = y - p.y;
    return (dx == 0 && (dy == -1 || dy == 1)) ||
           (dy == 0 && (dx == -1 || dx == 1));
}

int Point::distanceSq(const Point &p) const {
    int dx = x - p.x;
    int dy = y - p.y;
    return dx*dx + dy * dy;
}

bool Point::operator==(const Point &p) const {
    return x == p.x && y == p.y;
}



