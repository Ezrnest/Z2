//
// Created by liyicheng on 2019/7/25.
//

#include <iostream>
#include <string>
using namespace std;

#define ASIO_STANDALONE
#include <iostream>
#include <core/RemoteClient.h>
#include <core/RemoteServerProxy.h>
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
#include "plog/Log.h"


using namespace std;
using namespace z2;

const int port = 12351;
int main(){
    GameConfiguration::initAll();
    shared_ptr<RemoteClient> rc(new RemoteClient(2));

    shared_ptr<RemoteServerProxy> proxy(new RemoteServerProxy(rc));
    rc->setServerProxy(proxy);

    shared_ptr<CommandLineGameGui> gui(new CommandLineGameGui());
    rc->setView(gui);
    gui->setControllerAndView(rc);
    if(!RemoteServerProxy::tryConnect(proxy,"127.0.0.1", port)){
        cout << "failure, return";
        return 0;
    }
    gui->mainLoop();
}