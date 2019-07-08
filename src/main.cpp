#include <iostream>
#include "world/World.h"
#include "core/Server.h"
#include "core/EntityRepository.h"
#include "entity/Farmer.h"
#include "core/LocalClient.h"
#include "commandLine/CommandLineGameGui.h"
#include "entity/ConstructionBase.h"
#include "util/LogUtil.h"
#include "core/BotClientPort.h"

using namespace std;
using namespace z2;

void init() {
    EntityRepository::initDefaultEntities();
}

int main() {
    init();
    shared_ptr<World> w(new World(5, 5, 2));
    w->createEntity(Point(0, 0), ConstructionBase::getIdentifier(), 1);
    w->createEntity(Point(1, 1), Farmer::getIdentifier(), 1);
//    w->pla


    shared_ptr<Server> server(new Server());
    server->setWorld(w);

    shared_ptr<BotClientPort> bot(new BotClientPort());
    bot->setServer(server);
    server->registerClient(bot);

    shared_ptr<LocalClient> local(new LocalClient());
    local->setRealServer(server);
    server->registerClient(local);

    shared_ptr<CommandLineGameGui> gui(new CommandLineGameGui());
    local->setView(static_pointer_cast<GameGui>(gui));
    gui->setControllerAndView(static_pointer_cast<Client>(local));

//    gui->printWorld();

    server->startGame();
    gui->mainLoop();
}