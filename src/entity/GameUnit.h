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
    explicit GameUnit(int objectId);

    void initialize(const Properties &prop) override;
};

}
#endif //Z2_GAMEUNIT_H
