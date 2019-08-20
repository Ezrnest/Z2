/*
 * Created by liyicheng on 2019/7/6.
 */

#include "LocalClient.h"
#include "GameGui.h"
#include "messages/ControlMessage.h"
#include "messages/PlayerMessage.h"

const weak_ptr<z2::Server> &z2::LocalClient::getRealServer() const {
    return server;
}

void z2::LocalClient::setRealServer(const shared_ptr<z2::Server> &server) {
    LocalClient::server = server;
}

void z2::LocalClient::setServerProxy(const shared_ptr<z2::ServerProxy> &server) {

}

shared_ptr<z2::ServerProxy> z2::LocalClient::getServer() {
    return shared_ptr<ServerProxy>();
}

void z2::LocalClient::setView(const shared_ptr<z2::GameGui> &gui) {
    LocalClient::gui = gui;
}

void z2::LocalClient::acceptMessage(const shared_ptr<z2::Message> &command) {
    //TODO

    switch (command->getGeneralType()) {
        //TODO
        case GeneralMessageType::ControlMessage: {
            dealWithControlMessage(static_pointer_cast<ControlMessage>(command));
            break;
        }
        case GeneralMessageType::GameMessage: {
            gui.lock()->update();
            break;
        }
        case GeneralMessageType::ChatMessage: {
            break;
        }
    }

//    gui->

}

shared_ptr<z2::World> z2::LocalClient::getWorld() {
    return server.lock()->getWorld();
}

int z2::LocalClient::getPlayerId() {
    return getClientId();
}

void z2::LocalClient::sendMessage(const shared_ptr<z2::Message> &command) {
    acceptMessage(command);
}

bool z2::LocalClient::syncWorld(const shared_ptr<World> &world) {
    return true;
}

void z2::LocalClient::sendMessageToServer(const z2::MessagePtr &message) {
    server.lock()->acceptMessage(message);
}

void z2::LocalClient::dealWithControlMessage(const shared_ptr<ControlMessage> &message) {
    auto view = gui.lock();
    switch (message->getControlType()) {
        case ControlMessageType::StartGame: {
            view->onGameStarted();
            break;
        }
        case ControlMessageType::EndGame: {
            view->onGameStopped();
            break;
        }
        case ControlMessageType::PlayerTurnStart: {
            view->onPlayerTurnStarted(static_pointer_cast<PlayerMessage>(message)->getPlayerId());
            break;
        }
        case ControlMessageType::PlayerTurnFinish: {
            view->onPlayerTurnFinished(static_pointer_cast<PlayerMessage>(message)->getPlayerId());
            break;
        }

        case ControlMessageType::RegisterPlayer:
            break;
        case ControlMessageType::SyncWorld:
            break;
        case ControlMessageType::Signal:
            break;
        case ControlMessageType::PlayerQuit: {
            auto msg = static_pointer_cast<PlayerMessage>(message);
            view->onPlayerQuit(msg->getPlayerId());
            break;
        }
    }
}

const string &z2::LocalClient::getPlayerName() {
    return playerName;
}

void z2::LocalClient::setPlayerName(const string &playerName) {
    LocalClient::playerName = playerName;
}

bool z2::LocalClient::isGameRunning() {
    auto s = server.lock();
    if (!s) {
        return false;
    }
    return s->getGameState() == Server::GameState::RUNNING;
}

