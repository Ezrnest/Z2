/*
 * Created by liyicheng on 2019/7/5.
 */

#include "EntityPerform.h"

z2::EntityPerform::EntityPerform(z2::GameMessageType type, const z2::Point &pos) : GameMessage(type), pos(pos) {}

const z2::Point &z2::EntityPerform::getPos() const {
    return pos;
}

void z2::EntityPerform::setPos(const z2::Point &pos) {
    EntityPerform::pos = pos;
}
