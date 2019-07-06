/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_UNITMOVE_H
#define Z2_UNITMOVE_H

#include "../../world/Point.h"
#include "GameMessage.h"

namespace z2 {
class UnitMove : public GameMessage {
private:
    const Point from;
    const Point dest;
public:
    UnitMove(const z2::Point &from, const z2::Point &dest);

    const Point &getFrom() const;

    const Point &getDest() const;
};

}
#endif //Z2_UNITMOVE_H
