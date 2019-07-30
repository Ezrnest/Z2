
#define ASIO_STANDALONE

#include <iostream>
#include "world/World.h"
#include "core/Server.h"
#include "config/EntityRepository.h"
#include "entity/Farmer.h"
#include "core/LocalClient.h"
#include "commandLine/CommandLineGameGui.h"
#include "entity/ConstructionBase.h"
#include "bot/BotClientPort.h"
#include "config/GameConfiguration.h"
#include "config/SerializableRegistry.h"
#include "config/MapRepository.h"
#include "world/GameInitSetting.h"
using namespace std;
using namespace z2;

void init() {

    GameConfiguration::initAll();
}

void m1(){
    shared_ptr<World> w(new World(5, 5, 2));
    w->createEntity(Point(0, 0), ConstructionBase::className(), 0);
    w->createEntity(Point(1, 1), Farmer::className(), 0);
    w->createEntity(Point(3, 3), Farmer::className(), 1);
    w->getTile(0, 3).setResource(Resource::MINE);
    w->getTile(2,0).setTerrain(Terrain::MOUNTAIN);
    w->getTile(2,1).setTerrain(Terrain::MOUNTAIN);
    w->getTile(2,2).setTerrain(Terrain::MOUNTAIN);
//    w->getTile(2,0).setTerrain(Terrain::MOUNTAIN);
//    w->createEntity(Point(1,2), )


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

//    w->nextPlayer();
//    gui->printWorld();
//    cout << " -------------- \n";
//    stringstream ss;
//    w->serializeTo(ss);
//    cout << ss.str() << endl;
//    shared_ptr<World> w2((World *) SerializableRegistry::instance().deserialize(ss));
//    CommandLineGameGui::printWorld(*w2);

    server->startGame();
    gui->mainLoop();
}

void m2(){
    vector<PlayerSetting> players{PlayerSetting(0,0,0,PlayerType::LOCAL_PLAYER),
                                  PlayerSetting(1,1,1,PlayerType::LOCAL_PLAYER),
                                  PlayerSetting(2,2,2,PlayerType::LOCAL_PLAYER)};
    auto map = MapRepository::instance().getMaps()[0];
    GameInitSetting setting(players,map);
    auto world =setting.buildGame();
    CommandLineGameGui::printWorld(*world);
}

int main() {
    init();
    m2();
}