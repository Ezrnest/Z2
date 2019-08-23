
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

shared_ptr<World> getWorld() {
    shared_ptr<World> w(new World(8, 8, 2));
    w->createEntity(Point(0, 0), ConstructionBase::className(), 0);
//    w->createEntity(Point(1, 1), Farmer::className(), 0);
//    w->createEntity(Point(0, 1), "Archer", 0);
    w->createEntity(Point(1, 1), "Killer", 0);
    w->createEntity(Point(3, 3), Farmer::className(), 1);
    w->getTile(0, 3).setResource(Resource::MINE);
    w->getTile(2, 0).setTerrain(Terrain::MOUNTAIN);
    w->getTile(2, 1).setTerrain(Terrain::MOUNTAIN);
    w->getTile(2, 2).setTerrain(Terrain::MOUNTAIN);
//    w->getTile(2,0).setTerrain(Terrain::MOUNTAIN);
//    w->createEntity(Point(1,2), )
//    w->configure();
    return w;
}

void m1() {
    EventType t;
    auto w = getWorld();


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
    gui->printWorld();
    cout << " -------------- \n";
    stringstream ss;
    w->serializeTo(ss);
    cout << ss.str() << endl;
    shared_ptr<World> w2((World *) SerializableRegistry::instance().deserialize(ss));
    CommandLineGameGui::printWorld(*w2);

//    server->startGame();
//    gui->mainLoop();
}

void m2() {
    vector<PlayerSetting> players{PlayerSetting(0, -1, 0, PlayerType::LOCAL_PLAYER),
                                  PlayerSetting(1, -1, 1, PlayerType::BOT_PLAYER),
                                  PlayerSetting(2, -1, 2, PlayerType::BOT_PLAYER),
                                  PlayerSetting(3, -1, 2, PlayerType::BOT_PLAYER)};
    auto map = MapRepository::instance().getMaps()[0];
    GameInitSetting setting(players, map);
    auto world = setting.buildWorld();
    CommandLineGameGui::printWorld(*world);
//    File f = File::currentDirectory();
//    string path = f.subFile("save1.txt").getPath();
//    GameMap::saveWorldTo(path,"",world);
//    map = GameMap::loadWorldFrom(path);
//    auto w2 = map->buildWorld(players);
//    CommandLineGameGui::printWorld(*w2);
}

void m3() {
    auto w = getWorld();
    auto en = static_pointer_cast<GameUnit>(w->createEntity(Point(1, 0), "Scout", 0));
    auto path = w->findPath(Point(1, 0), Point(3, 0), en);
    CommandLineGameGui::printWorld(*w);
    for (auto &it : path) {
        cout << it.first.x << ", " << it.first.y << endl;
    }
}

int main() {
    init();
    m3();
}