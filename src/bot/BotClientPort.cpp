/*
 * Created by liyicheng on 2019/7/7.
 */

#include "plog/Log.h"
#include "BotClientPort.h"
#include "core/messages/ControlMessage.h"
#include "core/Server.h"
#include "core/messages/PlayerMessage.h"

void z2::BotClientPort::sendMessage(const std::shared_ptr<z2::Message> &command) {
    if (command->getGeneralType() == GeneralMessageType::ControlMessage) {
        shared_ptr<ControlMessage> msg = static_pointer_cast<ControlMessage>(command);
        switch (msg->getControlType()) {

            case ControlMessageType::StartGame:
                break;
            case ControlMessageType::EndGame:
                break;
            case ControlMessageType::PlayerTurnStart: {
                shared_ptr<PlayerMessage> playerMessage = static_pointer_cast<PlayerMessage>(msg);
                if(playerMessage->getPlayerId() == getClientId()){
                    doBotTurn();
                }
                break;
            }
            case ControlMessageType::PlayerTurnFinish:
                break;
            case ControlMessageType::PlayerDefeated:
                break;
            case ControlMessageType::PlayerWin:
                break;
            case ControlMessageType::RegisterPlayer:
                break;
            case ControlMessageType::SyncWorld:
                break;
            case ControlMessageType::Signal:
                break;
        }
    }
}

bool z2::BotClientPort::syncWorld(const shared_ptr<World> &world) {
    return true;
}

const std::weak_ptr<z2::Server> &z2::BotClientPort::getServer() const {
    return server;
}

void z2::BotClientPort::setServer(const std::shared_ptr<z2::Server> &server) {
    BotClientPort::server = server;
}

void z2::BotClientPort::doBotTurn() {
    PLOG(plog::info) << "Bot turn!";
    botTurnFinish();
}

void z2::BotClientPort::botTurnFinish() {
    shared_ptr<Message> msg(new PlayerMessage(ControlMessageType::PlayerTurnFinish, getClientId()));
    PLOG(plog::info) << "Bot turn finished!";
    server.lock()->acceptMessage(msg);
}
