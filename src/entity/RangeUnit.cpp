/*
 * Created by liyicheng on 2019/7/11.
 */

#include "RangeUnit.h"


z2::RangeUnit::RangeUnit(unsigned int objectId) : GameUnit(objectId) {

}

void z2::RangeUnit::initialize(const Properties &prop) {
    GameUnit::initialize(prop);
    attackStrength = prop.getInt("attackStrength", attackStrength);
    range = prop.getInt("range", range);
}

void z2::RangeUnit::serializeDataPart(ostream &output) {
    GameUnit::serializeDataPart(output);
    output << attackStrength << ' '
           << range << ' ';
}

void z2::RangeUnit::deserializeDataPart(istream &input, z2::RangeUnit *en) {
    GameUnit::deserializeDataPart(input, en);
    input >> en->attackStrength >> en->range;

}

int z2::RangeUnit::getAttackStrength() const {
    return attackStrength;
}

void z2::RangeUnit::setAttackStrength(int attackStrength) {
    RangeUnit::attackStrength = attackStrength;
}

int z2::RangeUnit::getRange() const {
    return range;
}

void z2::RangeUnit::setRange(int range) {
    RangeUnit::range = range;
}

void z2::RangeUnit::serializeTo(ostream &output) {

}

const string &z2::RangeUnit::getClassName() const {
    return className();
}

const string &z2::RangeUnit::className() {
    static string name = "RangeUnit";
    return name;
}

z2::RangeUnit *z2::RangeUnit::loadFrom(istream &input) {
    auto *unit = new RangeUnit(0);
    deserializeDataPart(input, unit);
    return unit;
}

z2::Entity *z2::RangeUnit::create(int objectId, const Properties &initializer) {
    auto* en = new RangeUnit(objectId);
    en->initialize(initializer);
    return en;
}
