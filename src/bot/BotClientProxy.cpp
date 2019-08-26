/*
 * Created by liyicheng on 2019/7/7.
 */

#include <c++/thread>
#include <utility>
#include "plog/Log.h"
#include "BotClientProxy.h"
#include "core/messages/ControlMessage.h"
#include "core/Server.h"
#include "core/messages/PlayerMessage.h"
#include "Dummy.h"
#include "GameEndedException.h"

void z2::BotClientProxy::sendMessage(const std::shared_ptr<z2::Message> &command) {
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
            case ControlMessageType::RegisterPlayer:
                break;
            case ControlMessageType::SyncWorld:
                break;
            case ControlMessageType::Signal:
                break;
            case ControlMessageType::PlayerQuit:
                break;
        }
    }
}

bool z2::BotClientProxy::syncWorld(const shared_ptr<World> &world) {
    bot->init(server.lock(), world, getClientId());
    return true;
}

const std::weak_ptr<z2::Server> &z2::BotClientProxy::getServer() const {
    return server;
}

void z2::BotClientProxy::setServer(const std::shared_ptr<z2::Server> &server) {
    BotClientProxy::server = server;
    bot->init(server, server->getWorld(), getClientId());
}


void z2::BotClientProxy::botTurnFinish() {
    shared_ptr<Message> msg(new PlayerMessage(ControlMessageType::PlayerTurnFinish, getClientId()));
    PLOG(plog::info) << "Bot turn finished!";
    if(!server.expired()){
        server.lock()->acceptMessage(msg);
    }
}

void z2::BotClientProxy::performTurn() {
    auto shared = shared_from_this();
    thread t([shared]{
        try{
            shared->bot->doBotTurn();
            shared->botTurnFinish();
        }catch (GameEndedException& ex){
            PLOG_WARNING << "Game ended in bot's turn!";
        }
    });
    t.detach();
}

z2::BotClientProxy::BotClientProxy(shared_ptr<Bot> bot) : bot(std::move(bot)) {}

z2::BotClientProxy::BotClientProxy() : bot(new Dummy()){

}
