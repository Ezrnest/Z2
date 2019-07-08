//
// Created by liyicheng on 2019/7/3.
//

#ifndef Z2_POINT_H
#define Z2_POINT_H

#include <array>

namespace z2 {
using namespace std;

class Point {
public:
    int x, y;

    Point() noexcept ;

    Point(int x, int y) noexcept;

    Point operator+(const Point &p) const;

    /**
     * Returns the gird distance from
     */
    int girdDistance(const Point &p) const;


    static const array<Point, 4> &directions() {
        static array<Point, 4> DIRECTIONS = {
                Point(1, 0), Point(0, 1), Point(-1, 0), Point(0, -1)
        };
        return DIRECTIONS;
    }


};


}
#endif //Z2_POINT_H
