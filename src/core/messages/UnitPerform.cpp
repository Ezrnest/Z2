/*
 * Created by liyicheng on 2019/7/11.
 */

#include "UnitPerform.h"

const z2::Point &z2::UnitPerform::getPos() const {
    return pos;
}

z2::UnitPerform::UnitPerform(const z2::Point &pos) : GameMessage(GameMessageType::UnitPerform), pos(pos) {}

z2::UnitPerform::UnitPerform() : GameMessage(GameMessageType::UnitPerform) {}

void z2::UnitPerform::serializeData(ostream &output) {
//    GameMessage::serializeData(output);
    pos.serializeData(output);
}

void z2::UnitPerform::deserializeData(istream &input) {
//    GameMessage::deserializeData(input);
    pos.deserializeData(input);
}

const string &z2::UnitPerform::getClassName()const {
    static string name = "UnitPerform";
    return name;
}
