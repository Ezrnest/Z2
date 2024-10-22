/*
 * Created by liyicheng on 2019/7/6.
 */

#include "GameEvent.h"

z2::EventType z2::GameEvent::getType() const {
    return type;
}

void z2::GameEvent::setType(z2::EventType type) {
    GameEvent::type = type;
}

z2::GameEvent::GameEvent(z2::EventType type) : type(type) {}


z2::GameEvent::~GameEvent() = default;

z2::InGameEvent::InGameEvent() : GameEvent(EventType::IN_GAME_EVENT) {}

