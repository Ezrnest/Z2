/*
 * Created by liyicheng on 2019/7/11.
 */

#include "world/World.h"
#include "RangeUnit.h"


z2::RangeUnit::RangeUnit(unsigned int objectId) : BattleUnit(objectId) {

}

void z2::RangeUnit::initialize(const Properties &prop) {
    BattleUnit::initialize(prop);

    range = prop.getDouble("range", range);
}

void z2::RangeUnit::serializeDataPart(ostream &output) {
    BattleUnit::serializeDataPart(output);
    output << range << ' ';
}

void z2::RangeUnit::deserializeDataPart(istream &input, z2::RangeUnit *en) {
    BattleUnit::deserializeDataPart(input, en);
    input >> en->range;

}

double z2::RangeUnit::getRange() const {
    return range;
}

void z2::RangeUnit::setRange(double range) {
    RangeUnit::range = range;
}

void z2::RangeUnit::serializeTo(ostream &output) {
    output << className() << ' ';
    serializeDataPart(output);
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

double z2::RangeUnit::computeRangeAt(const z2::Point &pos, z2::World &w) {
    Tile &t = w.getTile(pos);
    if(t.getTerrain() == Terrain::HILL){
        return range+1;
    }
    return range;
}
