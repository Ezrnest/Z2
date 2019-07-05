/*
 * Created by liyicheng on 2019/7/4.
 */

#include "UnitMove.h"

z2::UnitMove::UnitMove(const z2::Point &from, const z2::Point &dest)
        : GameMessage(GeneralMessageType::UnitMove),
          from(from), dest(dest) {}

const z2::Point &z2::UnitMove::getFrom() const {
    return from;
}

const z2::Point &z2::UnitMove::getDest() const {
    return dest;
}
