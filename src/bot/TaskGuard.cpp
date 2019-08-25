/*
 * Created by liyicheng on 2019/8/25.
 */

#include <plog/Log.h>
#include "TaskGuard.h"

z2::bot::TaskGuard::TaskGuard(z2::Bot *bot, unsigned int entityId, unsigned int target) : EntityTask(bot, entityId),
                                                                                          targetEntity(target) {}

void z2::bot::TaskGuard::doPerform() {
    PLOG_INFO << "[Guard] Performing";
    auto w = bot->world;
    auto guardian = w->getEntity(entityId);
    if (!guardian) {
        cancel();
        return;
    }
    Point tp = getTarget();
//    PLOG_INFO << "[Guard] tp.x=" << tp.x;
    if (guardian->getPos().girdDistance(tp) > maxGridDistance) {
        bot->moveToward(guardian, tp);
        return;
    }
    auto enemies = bot->getEnemiesAround(tp, enemyScanRange);
    if (!enemies.empty()) {
        bot->attack(guardian->getPos(), enemies[0]->getPos());
        return;
    }
    if (guardian->getPos().girdDistance(tp) > desiredDistance) {
        bot->moveToward(guardian, tp);
        return;
    }
}

z2::bot::TaskGuard::TaskGuard(z2::Bot *bot, unsigned int entityId, const z2::Point &target) : EntityTask(bot, entityId),
                                                                                              targetPos(target),
                                                                                              targetEntity(0) {
    targetIsUnit = false;
}

z2::bot::TaskGuard::TaskGuard(z2::Bot *bot, unsigned int entityId) : TaskGuard(bot, entityId, entityId) {}

z2::Point z2::bot::TaskGuard::getTarget() {
    auto w = bot->world;
    if (targetIsUnit) {
        auto t = w->getEntity(targetEntity);
        if (t) {
            return t->getPos();
        } else {
            auto guardian = w->getEntity(entityId);
            if (guardian) {
                return guardian->getPos();
            } else {
                cancel();
                return {};
            }
        }
    } else {
        return targetPos;
    }
}

unsigned int z2::bot::TaskGuard::getTargetEntity() const {
    return targetEntity;
}

const z2::Point &z2::bot::TaskGuard::getTargetPos() const {
    return targetPos;
}

bool z2::bot::TaskGuard::isTargetIsUnit() const {
    return targetIsUnit;
}

int z2::bot::TaskGuard::getMaxGridDistance() const {
    return maxGridDistance;
}

void z2::bot::TaskGuard::setMaxGridDistance(int maxGridDistance) {
    TaskGuard::maxGridDistance = maxGridDistance;
}

int z2::bot::TaskGuard::getEnemyScanRange() const {
    return enemyScanRange;
}

void z2::bot::TaskGuard::setEnemyScanRange(int enemyScanRange) {
    TaskGuard::enemyScanRange = enemyScanRange;
}

void z2::bot::TaskGuard::setTarget(z2::EntityId target) {
    targetEntity = target;
    targetIsUnit = true;
}

void z2::bot::TaskGuard::setTarget(const z2::Point &target) {
    targetPos = target;
    targetIsUnit = false;
}
