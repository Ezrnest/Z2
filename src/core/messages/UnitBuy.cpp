/*
 * Created by liyicheng on 2019/7/4.
 */

#include "UnitBuy.h"

using namespace z2;


UnitBuy::UnitBuy(const string &identifier, const Point &pos, int playerId) :
        GameMessage(GameMessageType::UnitBuy), identifier(identifier), playerId(playerId), pos(pos) {}

const string &UnitBuy::getIdentifier() const {
    return identifier;
}

int UnitBuy::getPlayerId() const {
    return playerId;
}

const Point &UnitBuy::getPos() const {
    return pos;
}

UnitBuy::UnitBuy() : GameMessage(GameMessageType::UnitBuy), identifier(), playerId(-1), pos(0, 0) {

}

void UnitBuy::serializeData(ostream &output) {
//    GameMessage::serializeData(output);
    output << identifier << ' ' << playerId << ' ';
    pos.serializeData(output);
}

void UnitBuy::deserializeData(istream &input) {
//    GameMessage::deserializeData(input);
    input >> identifier >> playerId;
    pos.deserializeData(input);
}

const string &UnitBuy::getClassName()const {
    static string name = "UnitBuy";
    return name;
}


