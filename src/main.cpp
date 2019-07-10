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
#include "core/GameConfiguration.h"
#include "core/SerializableRegistry.h"

using namespace std;
using namespace z2;

void init() {
//    Gam::initDefaultEntities();
    GameConfiguration::initRegistration();
}

int main() {
    init();
    shared_ptr<World> w(new World(5, 5, 2));
    w->createEntity(Point(0, 0), ConstructionBase::className(), 0);
    w->createEntity(Point(1, 1), Farmer::className(), 0);
//    w->pla


    shared_ptr<Server> server(new Server());
    server->setWorld(w);

    shared_ptr<LocalClient> local(new LocalClient());
    local->setRealServer(server);
    server->registerClient(local);

    shared_ptr<BotClientPort> bot(new BotClientPort());
    bot->setServer(server);
    server->registerClient(bot);

    shared_ptr<CommandLineGameGui> gui(new CommandLineGameGui());
    local->setView(static_pointer_cast<GameGui>(gui));
    gui->setControllerAndView(static_pointer_cast<Client>(local));

//    gui->printWorld();
    cout << " -------------- \n";
    stringstream ss;
    w->serializeTo(ss);
    cout << ss.str() << endl;
    shared_ptr<World> w2((World *) SerializableRegistry::instance().deserialize(ss));
    CommandLineGameGui::printWorld(*w2);
//    server->startGame();
//    gui->mainLoop();
}