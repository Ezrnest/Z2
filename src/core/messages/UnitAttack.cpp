/*
 * Created by liyicheng on 2019/7/11.
 */

#include "UnitAttack.h"

z2::UnitAttack::UnitAttack(const z2::Point &from, const z2::Point &dest) : GameMessage(GameMessageType::UnitAttack),
                                                                           from(from),
                                                                           dest(dest) {}

z2::UnitAttack::UnitAttack() : GameMessage(GameMessageType::UnitAttack) {}

const z2::Point &z2::UnitAttack::getFrom() const {
    return from;
}

const z2::Point &z2::UnitAttack::getDest() const {
    return dest;
}

void z2::UnitAttack::serializeData(ostream &output) {
//    GameMessage::serializeData(output);
    from.serializeData(output);
    dest.serializeData(output);
}


void z2::UnitAttack::deserializeData(istream &input) {
//    GameMessage::deserializeData(input);
    from.deserializeData(input);
    dest.deserializeData(input);
}

const string &z2::UnitAttack::getClassName()const {
    static string name = "UnitAttack";
    return name;
}
