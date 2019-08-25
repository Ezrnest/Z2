/*
 * Created by liyicheng on 2019/8/25.
 */

#include <plog/Log.h>
#include "TaskMoveAttack.h"

void z2::bot::TaskMoveAttack::doPerform() {
    PLOG_INFO << "[MoveAttack] Performing";
    auto w = bot->world;
    auto attacker = w->getEntity(entityId);
    if (!attacker) {
        cancel();
        return;
    }
    auto enemies = bot->getEnemiesAround(attacker->getPos(), enemyScanRange);
    if (!enemies.empty()) {
        bot->attack(attacker->getPos(), enemies[0]->getPos());
        return;
    }
    Point tp = getTarget();
    if (bot->moveToward(attacker, tp)) {
        done();
    }
}

z2::bot::TaskMoveAttack::TaskMoveAttack(z2::Bot *bot, unsigned int entityId, unsigned int target) : TaskGuard(bot,
                                                                                                              entityId,
                                                                                                              target) {}

z2::bot::TaskMoveAttack::TaskMoveAttack(z2::Bot *bot, unsigned int entityId, const z2::Point &target) : TaskGuard(bot,
                                                                                                                  entityId,
                                                                                                                  target) {}
