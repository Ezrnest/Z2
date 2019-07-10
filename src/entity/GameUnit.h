/*
 * Created by liyicheng on 2019/7/3.
 */

#ifndef Z2_GAMEUNIT_H
#define Z2_GAMEUNIT_H

#include "Entity.h"
#include "EntityWithHealth.h"
namespace z2 {

class GameUnit : public EntityWithHealth {
public:
    explicit GameUnit(unsigned int objectId);

    void initialize(const Properties &prop) override;

protected:
    void serializeDataPart(ostream &output) override;

    static void deserializeDataPart(istream &input, GameUnit *en);
};

}
#endif //Z2_GAMEUNIT_H
