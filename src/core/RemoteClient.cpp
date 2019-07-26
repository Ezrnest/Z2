//
// Created by liyicheng on 2019/7/23.
//

#include <core/messages/PlayerMessage.h>
#include "RemoteClient.h"
#include "world/World.h"
#include "GameGui.h"
using namespace z2;

RemoteClient::RemoteClient(int playerId) : playerId(playerId){

}


void z2::RemoteClient::acceptMessage(const shared_ptr<z2::Message> &command) {
    switch (command->getGeneralType()) {
        case GeneralMessageType::ControlMessage: {
            dealWithControlMessage(static_pointer_cast<ControlMessage>(command));
            break;
        }
        case GeneralMessageType::GameMessage: {
            world->dealWithMessage(static_pointer_cast<GameMessage>(command));
            gui.lock()->update();
        }
        case GeneralMessageType::ChatMessage: {
            //TODO
            break;
        }
    }
}

void z2::RemoteClient::setServerProxy(const shared_ptr<z2::ServerProxy> &server) {
    serverProxy = server;
}

shared_ptr<z2::ServerProxy> z2::RemoteClient::getServer() {
    return serverProxy;
}

shared_ptr<z2::World> z2::RemoteClient::getWorld() {
    return world;
}

int z2::RemoteClient::getPlayerId() {
    return playerId;
}
void RemoteClient::setPlayerId(int playerId) {
    RemoteClient::playerId = playerId;
}

const string &RemoteClient::getPlayerName() {
    return playerName;
}

void RemoteClient::setPlayerName(const string &playerName) {
    RemoteClient::playerName = playerName;
}


void z2::RemoteClient::sendMessageToServer(const z2::MessagePtr &message) {
    serverProxy->sendMessageToServer(message);
}

void z2::RemoteClient::setView(const shared_ptr<z2::GameGui> &gui) {
    RemoteClient::gui = gui;
}


void RemoteClient::dealWithControlMessage(const shared_ptr<ControlMessage> &message) {
    auto view = gui.lock();
    switch (message->getControlType()) {

        case ControlMessageType::RegisterPlayer:break;
        case ControlMessageType::SyncWorld:{
            dealWithSyncWorld(static_pointer_cast<SyncWorld>(message));
            break;
        }
        case ControlMessageType::StartGame:{
            view->onGameStarted();
            break;
        }
        case ControlMessageType::EndGame:break;
        case ControlMessageType::PlayerTurnStart: {
            auto msg = static_pointer_cast<PlayerMessage>(message);
            world->onPlayerTurnStart(msg->getPlayerId());
            view->onPlayerTurnStarted(static_pointer_cast<PlayerMessage>(message)->getPlayerId());
            break;
        }
        case ControlMessageType::PlayerTurnFinish:{
            world->onPlayerTurnFinish();
//            view->on(static_pointer_cast<PlayerMessage>(message)->getPlayerId());
            break;
        }
        case ControlMessageType::PlayerDefeated:{
            break;
        }
        case ControlMessageType::PlayerWin:{
            view->onPlayerWin(static_pointer_cast<PlayerMessage>(message)->getPlayerId());
            break;
        }
    }
}

void RemoteClient::dealWithSyncWorld(const shared_ptr<SyncWorld> &msg) {
    world = msg->getWorld();
}

void RemoteClient::onConnectionLost() {
    serverProxy.reset();
}

bool RemoteClient::isGameRunning() {
    return bool(serverProxy);
}


