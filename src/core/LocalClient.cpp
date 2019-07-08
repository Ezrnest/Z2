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

void z2::LocalClient::setServerPort(const shared_ptr<z2::ServerPort> &server) {

}

shared_ptr<z2::ServerPort> z2::LocalClient::getServer() {
    return shared_ptr<ServerPort>();
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

bool z2::LocalClient::syncWorld(const z2::World &world) {
    return true;
}

void z2::LocalClient::sendMessageToServer(const z2::MessagePtr &message) {
    server.lock()->acceptMessage(message);
}

void z2::LocalClient::dealWithControlMessage(const shared_ptr<ControlMessage> &message) {
    auto view = gui.lock();
    switch (message->getControlType()) {
        case ControlMessageType::StartGame: {

            break;
        }
        case ControlMessageType::EndGame: {
            break;
        }
        case ControlMessageType::PlayerTurnStart: {
            view->onPlayerTurnStarted(static_pointer_cast<PlayerMessage>(message)->getPlayerId());
            break;
        }
        case ControlMessageType::PlayerTurnFinish: {
            break;
        }
        case ControlMessageType::PlayerDefeated: {
            break;
        }
        case ControlMessageType::PlayerWin: {
            break;
        }
    }
}
