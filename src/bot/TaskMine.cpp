/*
 * Created by liyicheng on 2019/8/25.
 */

#include <entity/Farmer.h>
#include "TaskMine.h"

z2::bot::TaskMine::TaskMine(z2::Bot *bot, unsigned int entityId, const z2::Point &target) : EntityTask(bot, entityId),
                                                                                            target(target) {}

void z2::bot::TaskMine::doPerform() {
    auto w = bot->world;
    auto farmer = dynamic_pointer_cast<Farmer>(w->getEntity(entityId));
    if (!farmer) {
        cancel();
        return;
    }
    bool reached;
    if (farmer->getPos() == target) {
        reached = true;
    }else{
        bot->clearPos(target);
        reached = bot->moveToward(farmer->getObjectId(),target);
    }
    if (reached) {
        bot->perform(farmer);
    }
}

const z2::Point &z2::bot::TaskMine::getTarget() const {
    return target;
}
