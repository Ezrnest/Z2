/*
 * Created by liyicheng on 2019/7/3.
 */

#include "EventDispatcher.h"
#include <algorithm>
using namespace z2;


void EventDispatcher::addListener(const EventListener &listener) {
    listeners.push_back(listener);
}

void EventDispatcher::publish(shared_ptr<GameEvent> &event) {
    for(auto& listener : listeners){
        listener(event);
    }
}
