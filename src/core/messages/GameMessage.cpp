/*
 * Created by liyicheng on 2019/7/5.
 */

#include "GameMessage.h"


z2::GameMessage::GameMessage(z2::GameMessageType type) : Message(GeneralMessageType::GameMessage), gameType(type){

}

z2::GameMessageType z2::GameMessage::getGameType() const {
    return gameType;
}
