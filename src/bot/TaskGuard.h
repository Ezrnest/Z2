/*
 * Created by liyicheng on 2019/8/25.
 */

#ifndef Z2_TASKGUARD_H
#define Z2_TASKGUARD_H

#include "Task.h"

namespace z2{
namespace bot{
class TaskGuard : public EntityTask{
protected:
    EntityId targetEntity;
    Point targetPos;
    bool targetIsUnit = true;

    int maxGridDistance = 3;

    int desiredDistance = 1;

    int enemyScanRange = 2;

    Point getTarget();

public:
    TaskGuard(Bot *bot, unsigned int entityId, unsigned int target);

    TaskGuard(Bot *bot, unsigned int entityId, const Point &target);

    TaskGuard(Bot *bot, unsigned int entityId);

    EntityId getTargetEntity() const;

    const Point &getTargetPos() const;

    bool isTargetIsUnit() const;

    void setTarget(EntityId target);

    void setTarget(const Point& target);

    int getMaxGridDistance() const;

    void setMaxGridDistance(int maxGridDistance);

    int getEnemyScanRange() const;

    void setEnemyScanRange(int enemyScanRange);

    void doPerform() override;
};

}
}



#endif //Z2_TASKGUARD_H
