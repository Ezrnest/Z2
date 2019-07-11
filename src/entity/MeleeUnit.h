/*
 * Created by liyicheng on 2019/7/11.
 */

#ifndef Z2_MELEEUNIT_H
#define Z2_MELEEUNIT_H

#include "GameUnit.h"

namespace z2 {
class MeleeUnit : public GameUnit {
protected:
    int attackStrength = 25;
public:
    explicit MeleeUnit(unsigned int objectId);

    int getAttackStrength() const;

    void setAttackStrength(int attackStrength);

    void initialize(const Properties &prop) override;

public:
    const string &getClassName() const override;

    static const string& className();

    void serializeTo(ostream &output) override;

    static MeleeUnit* loadFrom(istream& input);

protected:
    void serializeDataPart(ostream &output) override;

    static void deserializeDataPart(istream &input, MeleeUnit *en);
};

}

#endif //Z2_MELEEUNIT_H
