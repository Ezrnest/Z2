/*
 * Created by liyicheng on 2019/7/11.
 */

#ifndef Z2_RANGEUNIT_H
#define Z2_RANGEUNIT_H


#include "GameUnit.h"
#include "BattleUnit.h"

namespace z2 {
class RangeUnit : public BattleUnit{
protected:
    double range = 2.0;
public:
    double getRange() const;

    void setRange(double range);

    explicit RangeUnit(unsigned int objectId);

    void initialize(const Properties &prop) override;

    double computeRangeAt(const Point& pos, World& w);

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
