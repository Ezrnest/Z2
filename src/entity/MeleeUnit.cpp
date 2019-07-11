/*
 * Created by liyicheng on 2019/7/11.
 */

#include "MeleeUnit.h"


void z2::MeleeUnit::initialize(const Properties &prop) {
    GameUnit::initialize(prop);
    attackStrength = prop.getInt("attackStrength", attackStrength);
}

void z2::MeleeUnit::serializeDataPart(ostream &output) {
    GameUnit::serializeDataPart(output);
    output << attackStrength << ' ';
}

void z2::MeleeUnit::deserializeDataPart(istream &input, z2::MeleeUnit *en) {
    GameUnit::deserializeDataPart(input,en);
    input >> en->attackStrength;
}

z2::MeleeUnit::MeleeUnit(unsigned int objectId) : GameUnit(objectId) {}

int z2::MeleeUnit::getAttackStrength() const {
    return attackStrength;
}

void z2::MeleeUnit::setAttackStrength(int attackStrength) {
    MeleeUnit::attackStrength = attackStrength;
}

void z2::MeleeUnit::serializeTo(ostream &output) {
    output << className() << ' ';
    serializeDataPart(output);
}

const string &z2::MeleeUnit::getClassName() const {
    return className();
}

const string &z2::MeleeUnit::className() {
    static string name = "MeleeUnit";
    return name;
}

z2::MeleeUnit *z2::MeleeUnit::loadFrom(istream &input) {
    auto* unit = new MeleeUnit(0);
    deserializeDataPart(input,unit);
    return unit;
}
