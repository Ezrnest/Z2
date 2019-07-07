/*
 * Created by liyicheng on 2019/7/7.
 */

#include "GameGui.h"

z2::GameGui::~GameGui() = default;


void z2::GameGui::setControllerAndView(const shared_ptr<z2::Client> &c) {
    client = c;
}
