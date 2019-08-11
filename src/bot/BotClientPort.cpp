/*
 * Created by liyicheng on 2019/7/7.
 */

#include <c++/thread>
#include <utility>
#include "plog/Log.h"
#include "BotClientPort.h"
#include "core/messages/ControlMessage.h"
#include "core/Server.h"
#include "core/messages/PlayerMessage.h"
#include "Dummy.h"

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
                    performTurn();
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
    bot->init(server.lock(), world, getClientId());
    return true;
}

const std::weak_ptr<z2::Server> &z2::BotClientPort::getServer() const {
    return server;
}

void z2::BotClientPort::setServer(const std::shared_ptr<z2::Server> &server) {
    BotClientPort::server = server;
    bot->init(server, server->getWorld(), getClientId());
}


void z2::BotClientPort::botTurnFinish() {
    shared_ptr<Message> msg(new PlayerMessage(ControlMessageType::PlayerTurnFinish, getClientId()));
    PLOG(plog::info) << "Bot turn finished!";
    if(!server.expired()){
        server.lock()->acceptMessage(msg);
    }
}

void z2::BotClientPort::performTurn() {
    thread t([this]{
        bot->doBotTurn();
        botTurnFinish();
    });
    t.detach();
}

z2::BotClientPort::BotClientPort(shared_ptr<Bot> bot) : bot(std::move(bot)) {}

z2::BotClientPort::BotClientPort() : bot(new Dummy){

}
