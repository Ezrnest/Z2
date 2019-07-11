/*
 * Created by liyicheng on 2019/7/5.
 */

#include "EntityPerform.h"

z2::EntityPerform::EntityPerform(const z2::Point &pos) : GameMessage(GameMessageType::UnitPerform), pos(pos) {}

z2::EntityPerform::EntityPerform() :
        GameMessage(GameMessageType::UnitPerform), pos() {

}


const z2::Point &z2::EntityPerform::getPos() const {
    return pos;
}

void z2::EntityPerform::setPos(const z2::Point &pos) {
    EntityPerform::pos = pos;
}

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

