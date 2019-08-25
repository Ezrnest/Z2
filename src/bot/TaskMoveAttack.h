/*
 * Created by liyicheng on 2019/8/25.
 */

#ifndef Z2_TASKMOVEATTACK_H
#define Z2_TASKMOVEATTACK_H

#include "TaskGuard.h"

namespace z2{
namespace bot{
class TaskMoveAttack : public TaskGuard {
public:
    TaskMoveAttack(Bot *bot, unsigned int entityId, unsigned int target);

    TaskMoveAttack(Bot *bot, unsigned int entityId, const Point &target);

    void doPerform() override;
};
}
}



#endif //Z2_TASKMOVEATTACK_H
