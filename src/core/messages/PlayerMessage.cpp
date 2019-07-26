/*
 * Created by liyicheng on 2019/7/4.
 */

#include "PlayerMessage.h"
#include <iostream>

z2::PlayerMessage::PlayerMessage(z2::ControlMessageType type, int playerId)
        : ControlMessage(type), playerId(playerId) {}

int z2::PlayerMessage::getPlayerId() const {
    return playerId;
}

void z2::PlayerMessage::serializeData(ostream &output) {
    ControlMessage::serializeData(output);
    output << playerId << ' ';
}

void z2::PlayerMessage::deserializeData(istream &input) {
    ControlMessage::deserializeData(input);
    input >> playerId;
}

const string &z2::PlayerMessage::getClassName() const {
    static string name = "PlayerMessage";
    return name;
}

z2::PlayerMessage::PlayerMessage(): playerId(0) {}
