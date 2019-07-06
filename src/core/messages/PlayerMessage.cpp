/*
 * Created by liyicheng on 2019/7/4.
 */

#include "PlayerMessage.h"


z2::PlayerMessage::PlayerMessage(z2::ControlMessageType type, int playerId)
        : ControlMessage(type), playerId(playerId) {}

int z2::PlayerMessage::getPlayerId() const {
    return playerId;
}
