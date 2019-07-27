

//
// Created by liyicheng on 2019/7/22.
//
#include "Message.h"
#include "messages/SyncWorld.h"
#include <sstream>
#include "RemoteClientProxy.h"
#include "plog/Log.h"
using namespace z2;

RemoteClientProxy::RemoteClientProxy(shared_ptr<MessageConductor> cond, int condId) : conductor(std::move(cond)),
                                                                                      condId(condId) {

}

void RemoteClientProxy::sendMessage(const shared_ptr<Message> &command) {
    PLOG(plog::info) << "Sending message: " << command->getClassName();
    conductor->sendMessage(command, condId);
}

bool RemoteClientProxy::syncWorld(const shared_ptr<World> &world) {
    MessagePtr msg(new SyncWorld(world));
    sendMessage(msg);
    return true;
}
