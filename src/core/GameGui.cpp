/*
 * Created by liyicheng on 2019/7/7.
 */

#include "GameGui.h"
#include "world/World.h"
z2::GameGui::~GameGui() = default;



void z2::GameGui::setControllerAndView(const shared_ptr<z2::Client> &c) {
    client = c;
    auto w = client->getWorld();
    attachListener(w);
}

void z2::GameGui::onWorldLoaded(const shared_ptr<World> &w) {
    attachListener(w);
    update();
}

void z2::GameGui::attachListener(const shared_ptr<World> &w) {
    auto listener = [this](const GameEventPtr& event){
        this->onEvent(event);
    };
    if(w) {
        w->addEventListener(listener);
    }
}

void z2::GameGui::onPlayerQuit(int playerId) {

}

