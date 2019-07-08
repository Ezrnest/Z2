/*
 * Created by liyicheng on 2019/7/6.
 */

#include "Client.h"
#include "messages/PlayerMessage.h"

z2::Client::~Client() = default;

void z2::Client::sendMessageToServer(const shared_ptr<z2::Message> &message) {
    getServer()->sendMessageToServer(message);
}

void z2::Client::sendTurnFinishMessage() {
    shared_ptr<Message> msg(new PlayerMessage(ControlMessageType::PlayerTurnFinish, getPlayerId()));
    sendMessageToServer(msg);

}
