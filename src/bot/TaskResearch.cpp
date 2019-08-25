/*
 * Created by liyicheng on 2019/8/25.
 */

#include "TaskResearch.h"
#include <algorithm>
#include <core/messages/TechResearch.h>
#include "world/Technology.h"
namespace z2 {
namespace bot {
TaskResearch::TaskResearch(z2::Bot *bot) : Task(bot) {}

void TaskResearch::doPerform() {
    auto w = bot->world;
    auto techList = w->getResearchableTechFor(bot->playerId);
    if (techList.empty()) {
        done();
        return;
    }
    auto it = min_element(techList.begin(),techList.end(), [](const Technology* t1,const Technology* t2){
        return t1->getPrice() < t2->getPrice();
    });
    auto tech = (*it);
    bot->makeOperation(make_shared<TechResearch>(tech->getId(), bot->playerId));
}


}
}

