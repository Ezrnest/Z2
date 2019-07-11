/*
 * Created by liyicheng on 2019/7/11.
 */

#ifndef Z2_RANGEUNIT_H
#define Z2_RANGEUNIT_H


#include "GameUnit.h"

namespace z2 {
class RangeUnit : public GameUnit{
protected:
    int attackStrength = 25;

    int range = 2;
public:
    int getAttackStrength() const;

    void setAttackStrength(int attackStrength);

    int getRange() const;

    void setRange(int range);

    explicit RangeUnit(unsigned int objectId);

    void initialize(const Properties &prop) override;

public:
    const string &getClassName() const override;

    static const string& className();

    void serializeTo(ostream &output) override;

    static RangeUnit* loadFrom(istream& input);

    static Entity* create(int objectId, const Properties& initializer);

protected:
    void serializeDataPart(ostream &output) override;

    static void deserializeDataPart(istream &input, RangeUnit *en);
};

}


#endif //Z2_RANGEUNIT_H
