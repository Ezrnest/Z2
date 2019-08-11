/*
 * Created by liyicheng on 2019/7/11.
 */

#include "MeleeUnit.h"


void z2::MeleeUnit::initialize(const Properties &prop) {
    BattleUnit::initialize(prop);
}

void z2::MeleeUnit::serializeDataPart(ostream &output) {
    BattleUnit::serializeDataPart(output);
}

void z2::MeleeUnit::deserializeDataPart(istream &input, z2::MeleeUnit *en) {
    BattleUnit::deserializeDataPart(input,en);
}

z2::MeleeUnit::MeleeUnit(unsigned int objectId) : BattleUnit(objectId) {}


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

z2::Entity *z2::MeleeUnit::create(int objectId, const Properties &initializer) {
    auto *en = new MeleeUnit(objectId);
    en->initialize(initializer);
    return en;
}
