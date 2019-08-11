//
// Created by liyicheng on 2019/8/10.
//

#include "SetPlayerData.h"
#include "world/World.h"
#include <iostream>

namespace z2 {


SetPlayerData::SetPlayerData() : GameMessage(GameMessageType::SetPlayerData) {}

void SetPlayerData::setDataForWorld(World &w) {
    if (w.isValidLivingPlayer(playerId)) {
        setDataToPlayer(w.getPlayer(playerId));
    }
}

void SetPlayerData::setDataToPlayer(Player &p) {
    p.setGold(gold);
    p.setTechPoints(techPoints);
    p.setMaxTechPoints(maxTechPoints);
}

void SetPlayerData::deserializeData(istream &input) {
//    GameMessage::deserializeData(input);
    input >> playerId >> gold >> techPoints >> maxTechPoints;
}

void SetPlayerData::serializeData(ostream &output) {
//    GameMessage::serializeData(output);
    output << playerId << ' '
           << gold << ' '
           << techPoints << ' '
           << maxTechPoints << ' ';
}

const string &SetPlayerData::getClassName() const {
    static string name = "SetPlayerData";
    return name;
}

SetPlayerData::SetPlayerData(int playerId, int gold, int techPoints, int maxTechPoints)
        : GameMessage(GameMessageType::SetPlayerData), playerId(playerId), gold(gold), techPoints(techPoints), maxTechPoints(maxTechPoints) {}
}