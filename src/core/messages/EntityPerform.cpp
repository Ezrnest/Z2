/*
 * Created by liyicheng on 2019/7/11.
 */

#include "EntityPerform.h"

const z2::Point &z2::EntityPerform::getPos() const {
    return pos;
}

z2::EntityPerform::EntityPerform(const z2::Point &pos) : GameMessage(GameMessageType::EntityPerform), pos(pos) {}

z2::EntityPerform::EntityPerform() : GameMessage(GameMessageType::EntityPerform) {}

void z2::EntityPerform::serializeData(ostream &output) {
//    GameMessage::serializeData(output);
    pos.serializeData(output);
}

void z2::EntityPerform::deserializeData(istream &input) {
//    GameMessage::deserializeData(input);
    pos.deserializeData(input);
}

const string &z2::EntityPerform::getClassName()const {
    static string name = "EntityPerform";
    return name;
}
