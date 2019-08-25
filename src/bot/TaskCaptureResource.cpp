/*
 * Created by liyicheng on 2019/8/25.
 */

#include "TaskCaptureResource.h"

#include <memory>
#include "TaskMine.h"
#include "TaskGuard.h"

namespace z2 {
namespace bot {
TaskCaptureResource::TaskCaptureResource(z2::Bot *bot, const z2::Point &targetMine, z2::EntityId farmer,
                                         const vector<EntityId> &gs) : Task(bot), targetMine(targetMine) {
    mine.reset(new TaskMine(bot, farmer, targetMine));
    for (auto id : gs) {
        guardians.push_back(std::make_shared<TaskGuard>(bot, id, farmer));
    }
}

TaskCaptureResource::TaskCaptureResource(Bot *bot, const Point &targetMine, EntityId farmer)
        : TaskCaptureResource(bot, targetMine, farmer, vector<EntityId>()) {}

void TaskCaptureResource::doPerform() {
    mine->perform();
    if (!mine->isActive()) {
        guardMine();
    }
    guardians = Task::performAndFilter<TaskGuard>(guardians);
}

const shared_ptr<TaskMine> &TaskCaptureResource::getMine() const {
    return mine;
}

const vector<shared_ptr<TaskGuard>> &TaskCaptureResource::getGuardians() const {
    return guardians;
}

int TaskCaptureResource::getGuardianCount() {
    return guardians.size();
}

bool TaskCaptureResource::isFarmerAlive() {
    return mine->isActive();
}

void TaskCaptureResource::addGuardian(EntityId guardian) {
    if (mine->isActive()) {
        shared_ptr<TaskGuard> tg(new TaskGuard(bot, guardian, mine->getEntityId()));
        guardians.push_back(tg);
    } else {
        shared_ptr<TaskGuard> tg(new TaskGuard(bot, guardian, targetMine));
        guardians.push_back(tg);
    }
}

void TaskCaptureResource::autoGuard() {
    if (mine->isActive()) {
        guardFarmer();
    } else {
        guardMine();
    }
}

void TaskCaptureResource::guardMine() {
    for (auto &tg : guardians) {
        tg->setTarget(targetMine);
    }
}

void TaskCaptureResource::guardFarmer() {
    EntityId eid = mine->getEntityId();
    for (auto &tg : guardians) {
        tg->setTarget(eid);
    }

}

void TaskCaptureResource::resetFarmer(EntityId farmer) {
    mine.reset(new TaskMine(bot, farmer, targetMine));
    guardFarmer();
}

const Point &TaskCaptureResource::getTargetMine() const {
    return targetMine;
}

Resource TaskCaptureResource::getResource() const {
    return bot->world->getTile(targetMine).getResource();
}


}
}
