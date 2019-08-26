
#define ASIO_STANDALONE

#include "gamewindow.h"
#include "imagerepository.h"
#include "SoundRepository.h"
#include "mainwindow.h"
#include <QApplication>
#include "config/GameConfiguration.h"
#include "world/World.h"
#include "entity/Farmer.h"
#include "entity/ConstructionBase.h"
#include "core/Server.h"
#include "core/LocalClient.h"
#include "bot/BotClientProxy.h"
#include <QTranslator>
using namespace z2;

static QTranslator* trans = nullptr;
void initLang(QApplication& app){
    auto& config =  z2::GameConfiguration::instance();
    string lang = config.getProp().get("language","cn");
    trans = new QTranslator;
    if(lang == "cn"){
        trans->load(":/lang_cn.qm");
    }else{
        cout << "Loading en:" << trans->load(":/lang_en.qm") << endl;
    }
    app.installTranslator(trans);
}

void initAboutQT(QApplication& app){
    initLang(app);
    auto resDir = z2::GameConfiguration::getResourceDir();
    z2::ImageRepository::instance().initFromFolder(resDir.subFile("image"));
    z2::SoundRepository::instance().initFromFolder(resDir.subFile("sound"));
    qRegisterMetaType<shared_ptr<GameEvent>>("shared_ptr<GameEvent>");
}

void initAll(QApplication& app){
    z2::GameConfiguration::initAll();
    initAboutQT(app);
}

shared_ptr<z2::World> getWorld(){
    shared_ptr<World> w(new World(7, 7, 2));
    w->createEntity(Point(0, 0), "ConstructionBase", 0);
    //        w->createEntity(Point(0, 1), "Fighter", 0);
    //        w->createEntity(Point(1, 1), "Farmer", 0);
    w->createEntity(Point(3, 3), "Farmer", 1);
    w->getTile(0, 3).setResource(Resource::MINE);
    w->getTile(3,4).setResource(Resource::GEM);
    w->getTile(2,0).setTerrain(Terrain::MOUNTAIN);
    w->getTile(2,1).setTerrain(Terrain::MOUNTAIN);
    w->getTile(2,2).setTerrain(Terrain::MOUNTAIN);
    w->getPlayer(0).setName("LocalPlayer");
    w->getPlayer(1).setName("BotPlayer");
    //        w->configure();
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
    shared_ptr<BotClientProxy> bot(new BotClientProxy());
    bot->setServer(server);
    server->registerClient(bot);

    //    w->resetVisibility(0);
}

int runGameWindow(QApplication& a){
    GameWindow w;
    shared_ptr<Server> server;
    shared_ptr<LocalClient> client;
    buildTestGame(server,client);

    w.setClient(client);
    //    shared_ptr<
    w.show();
    w.setServer(server);
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
    QApplication a(argc, argv);

    initAll(a);

    return runMainMenu(a);

}
