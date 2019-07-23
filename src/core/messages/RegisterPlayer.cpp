#include <utility>

//
// Created by liyicheng on 2019/7/22.
//

#include "RegisterPlayer.h"
#include <iostream>


z2::RegisterPlayer::RegisterPlayer(int playerId, string playerName) : ControlMessage(ControlMessageType::RegisterPlayer), playerId(playerId),
                                                                             playerName(std::move(playerName)) {}

z2::RegisterPlayer::RegisterPlayer() : ControlMessage(ControlMessageType::RegisterPlayer), playerId(-1),
                                       playerName("") {

}


int z2::RegisterPlayer::getPlayerId() const {
    return playerId;
}

const string &z2::RegisterPlayer::getPlayerName() const {
    return playerName;
}

const string &z2::RegisterPlayer::getClassName() const {
    static string name = "RegisterPlayer";
    return name;
}

void z2::RegisterPlayer::serializeData(ostream &output) {
//    ControlMessage::serializeData(output);
    output << playerId << ' ' << playerName << ' ';
}

void z2::RegisterPlayer::deserializeData(istream &input) {
//    ControlMessage::deserializeData(input);
    input >> playerId >> playerName;
}

