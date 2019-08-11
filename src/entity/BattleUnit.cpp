//
// Created by liyicheng on 2019/8/11.
//

#include "BattleUnit.h"

namespace z2 {


int BattleUnit::getAttackStrength() const {
    return attackStrength;
}

void BattleUnit::setAttackStrength(int attackStrength) {
    BattleUnit::attackStrength = attackStrength;
}

void BattleUnit::initialize(const Properties &prop) {
    GameUnit::initialize(prop);
    attackStrength = prop.getInt("attackStrength", attackStrength);
}

void BattleUnit::serializeDataPart(ostream &output) {
    GameUnit::serializeDataPart(output);
    output << attackStrength << ' ';
}

void BattleUnit::deserializeDataPart(istream &input, BattleUnit *en) {
    GameUnit::deserializeDataPart(input,en);
    input >> en->attackStrength;
}

BattleUnit::BattleUnit(unsigned int objectId) : GameUnit(objectId) {}
}