//
// Created by liyicheng on 2019/8/11.
//

#ifndef Z2_BATTLEUNIT_H
#define Z2_BATTLEUNIT_H


#include "GameUnit.h"

namespace z2 {

class BattleUnit : public GameUnit{
protected:
    int attackStrength = 100;

public:
    explicit BattleUnit(unsigned int objectId);

    int getAttackStrength() const;

    void setAttackStrength(int attackStrength);

    void initialize(const Properties &prop) override;

protected:
    void serializeDataPart(ostream &output) override;

    static void deserializeDataPart(istream &input, BattleUnit *en);
};

}


#endif //Z2_BATTLEUNIT_H
