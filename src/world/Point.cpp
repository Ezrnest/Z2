//
// Created by liyicheng on 2019/7/3.
//

#include "Point.h"
using namespace z2;
z2::Point::Point(int x, int y) noexcept : x(x), y(y) {}

z2::Point z2::Point::operator+(const z2::Point &p) const {
    return {x + p.x, y + p.y};
}

Point::Point() noexcept : Point(-1,-1){

}

int Point::girdDistance(const Point& p) const{
    return abs(x - p.x) + abs(y - p.y);
}
