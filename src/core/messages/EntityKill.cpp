#include <utility>

//
// Created by liyicheng on 2019/7/22.
//

#include "EntityKill.h"

z2::EntityKill::EntityKill(z2::Point pos) : GameMessage(GameMessageType::EntityKill), pos(std::move(pos)) {}

z2::EntityKill::EntityKill() : EntityKill(Point()) {

}

const z2::Point &z2::EntityKill::getPos() const {
    return pos;
}

const string &z2::EntityKill::getClassName() const {
    static string name = "EntityKill";
    return name;
}

void z2::EntityKill::deserializeData(istream &input) {
//    GameMessage::deserializeData(input);
    pos.deserializeData(input);
}

void z2::EntityKill::serializeData(ostream &output) {
//    GameMessage::serializeData(output);
    pos.serializeData(output);
}


