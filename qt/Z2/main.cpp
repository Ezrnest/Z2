
#define ASIO_STANDALONE

#include "gamewindow.h"
#include "mainwindow.h"
#include <QApplication>
#include "config/GameConfiguration.h"
#include "world/World.h"
#include "entity/Farmer.h"
#include "entity/ConstructionBase.h"
#include "core/Server.h"
#include "core/LocalClient.h"
#include "bot/BotClientPort.h"
using namespace z2;
shared_ptr<z2::World> getWorld(){
    shared_ptr<World> w(new World(5, 5, 2));
        w->createEntity(Point(0, 0), "ConstructionBase", 0);
        w->createEntity(Point(0, 1), "Fighter", 0);
        w->createEntity(Point(1, 1), "Farmer", 0);
        w->createEntity(Point(3, 3), "Farmer", 1);
        w->getTile(0, 3).setResource(Resource::MINE);
        w->getTile(2,0).setTerrain(Terrain::MOUNTAIN);
        w->getTile(2,1).setTerrain(Terrain::MOUNTAIN);
        w->getTile(2,2).setTerrain(Terrain::MOUNTAIN);
        w->getPlayer(0).setName("LocalPlayer");
        w->getPlayer(1).setName("BotPlayer");
        return w;
}

void buildTestGame(shared_ptr<Server>& server, shared_ptr<LocalClient>& client){
    auto w= getWorld();
    server.reset(new Server());
        server->setWorld(w);
        shared_ptr<LocalClient> local(new LocalClient());
        local->setRealServer(server);
        server->registerClient(local);
    client = local;
        shared_ptr<BotClientPort> bot(new BotClientPort());
        bot->setServer(server);
        server->registerClient(bot);

}

int runGameWindow(QApplication& a){
    GameWindow w;
    shared_ptr<Server> server;
    shared_ptr<LocalClient> client;
    buildTestGame(server,client);

    w.setClient(client);
    //    shared_ptr<
    w.show();
    server->startGame();
    return a.exec();
}

int runMainMenu(QApplication& a){
    MainWindow w;
    w.show();
    return a.exec();
}

int main(int argc, char *argv[])
{
    z2::GameConfiguration::initAll();

    QApplication a(argc, argv);

    return runMainMenu(a);

}