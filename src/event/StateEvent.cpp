//
// Created by liyicheng on 2019/7/28.
//

#include "StateEvent.h"

namespace z2 {


StateEvent::StateEvent(StateEventType sType) : GameEvent(EventType::STATE_EVENT), sType(sType) {

}

StateEventType StateEvent::getSType() const {
    return sType;
}

PlayerEvent::PlayerEvent(StateEventType sType, int playerId) : StateEvent(sType), playerId(playerId) {}

int PlayerEvent::getPlayerId() const {
    return playerId;
}

GroupWonEvent::GroupWonEvent(StateEventType sType, int groupId) : StateEvent(sType), groupId(groupId) {}

int GroupWonEvent::getGroupId() const {
    return groupId;
}
}