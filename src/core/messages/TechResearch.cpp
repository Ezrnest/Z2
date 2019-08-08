/*
 * Created by liyicheng on 2019/8/7.
 */

#include "TechResearch.h"
#include <iostream>
z2::TechResearch::TechResearch(const string& techId, int playerId) : GameMessage(GameMessageType::TechResearch), techId(techId),
                                                           playerId(playerId) {}

const string& z2::TechResearch::getTechId() const {
    return techId;
}

int z2::TechResearch::getPlayerId() const {
    return playerId;
}

const string &z2::TechResearch::getClassName() const {
    static string name = "TechResearch";
    return name;
}

void z2::TechResearch::serializeData(ostream &output) {
//    GameMessage::serializeData(output);
    output << techId << ' '
           << playerId << ' ';
}

void z2::TechResearch::deserializeData(istream &input) {
//    GameMessage::deserializeData(input);
    input >> techId >> playerId;
}

z2::TechResearch::TechResearch() : GameMessage(GameMessageType::TechResearch) {}
