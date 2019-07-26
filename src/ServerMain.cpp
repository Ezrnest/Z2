//
// Created by liyicheng on 2019/7/25.
//



using namespace std;

#define ASIO_STANDALONE
#include <iostream>
#include <string>
#include <core/Lobby.h>
#include "world/World.h"
#include "core/Server.h"
#include "core/EntityRepository.h"
#include "entity/Farmer.h"
#include "core/LocalClient.h"
#include "commandLine/CommandLineGameGui.h"
#include "entity/ConstructionBase.h"
#include "core/BotClientPort.h"
#include "core/GameConfiguration.h"
#include "core/SerializableRegistry.h"



using namespace std;
using namespace z2;


shared_ptr<World> buildWorld() {
    shared_ptr<World> w(new World(5, 5, 3));
    w->createEntity(Point(0, 0), ConstructionBase::className(), 0);
    w->createEntity(Point(1, 1), Farmer::className(), 0);
    w->createEntity(Point(3, 3), Farmer::className(), 1);
    w->getTile(0, 3).setResource(Resource::MINE);
    return w;
}

const int port = 12351;

int main() {
    cout << "Starting..." << endl;
    GameConfiguration::initAll();
    auto w = buildWorld();
    vector<PlayerType> players{PlayerType::LOCAL_PLAYER, PlayerType::BOT_PLAYER, PlayerType::REMOTE_PLAYER};
    Lobby lobby(players,port,w);

    shared_ptr<CommandLineGameGui> gui(new CommandLineGameGui());
//    local->setView(static_pointer_cast<GameGui>(gui));
//    gui->setControllerAndView(static_pointer_cast<Client>(local));

    auto server = lobby.startGame(gui, 1000000);
    if(!server){
        return 0;
    }
    gui->mainLoop();
}