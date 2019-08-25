/*
 * Created by liyicheng on 2019/8/25.
 */

#ifndef Z2_TASKMINE_H
#define Z2_TASKMINE_H

#include "Task.h"

namespace z2{
namespace bot{
class TaskMine : public EntityTask{
private:
    Point target;
public:
    TaskMine(Bot *bot, unsigned int entityId, const Point &target);

    const Point &getTarget() const;

    void doPerform() override;
};
}
}



#endif //Z2_TASKMINE_H
