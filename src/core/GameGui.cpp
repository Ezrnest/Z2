/*
 * Created by liyicheng on 2019/7/7.
 */

#include "GameGui.h"
#include "world/World.h"
z2::GameGui::~GameGui() = default;


void z2::GameGui::setControllerAndView(const shared_ptr<z2::Client> &c) {
    client = c;
    auto w = client->getWorld();
    auto listener = [this](const GameEventPtr& event){
        this->onEvent(event);
    };
    w->addEventListener(listener);
}
