/*
 * Created by liyicheng on 2019/8/25.
 */

#ifndef Z2_TASKCAPTURERESOURCE_H
#define Z2_TASKCAPTURERESOURCE_H

#include "Task.h"

namespace z2{
namespace bot{
class TaskCaptureResource : public Task{
private:
    Point targetMine;
    shared_ptr<TaskMine> mine;
    vector<shared_ptr<TaskGuard>> guardians;

    void autoGuard();

    void guardMine();

    void guardFarmer();

public:
    TaskCaptureResource(Bot *bot, const Point& targetMine, EntityId farmer, const vector<EntityId>& guardians);

    TaskCaptureResource(Bot *bot, const Point& targetMine, EntityId farmer);
//    TaskG
    void doPerform() override;

    const shared_ptr<TaskMine> &getMine() const;

    bool isFarmerAlive();

    void resetFarmer(EntityId farmer);

    void addGuardian(EntityId guradian);

    const vector<shared_ptr<TaskGuard>> &getGuardians() const;

    int getGuardianCount();

    const Point &getTargetMine() const;

    Resource getResource() const;
};
}
}



#endif //Z2_TASKCAPTURERESOURCE_H
