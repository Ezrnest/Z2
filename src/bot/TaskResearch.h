/*
 * Created by liyicheng on 2019/8/25.
 */

#ifndef Z2_TASKRESEARCH_H
#define Z2_TASKRESEARCH_H

#include "Task.h"

namespace z2{
namespace bot{
class TaskResearch : public Task{
public:
    explicit TaskResearch(Bot *bot);

protected:
    void doPerform() override;
};
}
}



#endif //Z2_TASKRESEARCH_H
