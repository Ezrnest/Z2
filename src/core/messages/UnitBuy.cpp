/*
 * Created by liyicheng on 2019/7/4.
 */

#include "UnitBuy.h"
using namespace z2;
UnitBuy::UnitBuy(const string &identifier, const int playerId, const Point &pos,
                     const Properties &prop) : GameMessage(GameMessageType::UnitBuy), identifier(identifier),
                                               playerId(playerId), pos(pos),
                                               prop(prop) {}


UnitBuy::UnitBuy(const string &identifier, const Point &pos, int playerId) :
        GameMessage(GameMessageType::UnitBuy), identifier(identifier), playerId(playerId), pos(pos),
        prop() {}

const string &UnitBuy::getIdentifier() const {
    return identifier;
}

int UnitBuy::getPlayerId() const {
    return playerId;
}

const Point &UnitBuy::getPos() const {
    return pos;
}

const Properties &UnitBuy::getProp() const {
    return prop;
}

void UnitBuy::setProp(const Properties &prop) {
    UnitBuy::prop = prop;
}


