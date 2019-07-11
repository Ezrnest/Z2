/*
 * Created by liyicheng on 2019/7/4.
 */

#include "UnitMove.h"

z2::UnitMove::UnitMove(const z2::Point &from, const z2::Point &dest)
        : GameMessage(GameMessageType::UnitMove),
          from(from), dest(dest) {}

z2::UnitMove::UnitMove() : GameMessage(GameMessageType::UnitMove) {

}


const z2::Point &z2::UnitMove::getFrom() const {
    return from;
}

const z2::Point &z2::UnitMove::getDest() const {
    return dest;
}

const string &z2::UnitMove::getClassName()const {
    static string name = "UnitMove";
    return name;
}

void z2::UnitMove::serializeData(ostream &output) {
//    GameMessage::serializeData(output);
    from.serializeData(output);
    dest.serializeData(output);
}


void z2::UnitMove::deserializeData(istream &input) {
//    GameMessage::deserializeData(input);
    from.deserializeData(input);
    dest.deserializeData(input);
}

