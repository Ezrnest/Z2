/*
 * Created by liyicheng on 2019/7/4.
 */

#include "UnitBuy.h"

z2::UnitBuy::UnitBuy(const string &identifier, const z2::Point &pos, int playerId) :
        GameMessage(GeneralMessageType::UnitBuy),
        identifier(identifier),
        pos(pos), playerId(playerId) {}

const string &z2::UnitBuy::getIdentifier() const {
    return identifier;
}

int z2::UnitBuy::getPlayerId() const {
    return playerId;
}

const z2::Point &z2::UnitBuy::getPos() const {
    return pos;
}
