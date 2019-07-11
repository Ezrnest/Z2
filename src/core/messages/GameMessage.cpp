/*
 * Created by liyicheng on 2019/7/5.
 */

#include "GameMessage.h"
#include <iostream>

z2::GameMessage::GameMessage(z2::GameMessageType type) : Message(GeneralMessageType::GameMessage), gameType(type){

}

z2::GameMessageType z2::GameMessage::getGameType() const {
    return gameType;
}

void z2::GameMessage::serializeData(ostream &output) {
//    Message::serializeData(output);
    output << static_cast<int>(gameType) << ' ';
}

void z2::GameMessage::deserializeData(istream &input) {
//    Message::deserializeData(input);
    int t;
    input >> t;
    gameType = static_cast<GameMessageType>(t);
}
