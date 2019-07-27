//
// Created by liyicheng on 2019/7/27.
//

#ifndef Z2_GAMEMAP_H
#define Z2_GAMEMAP_H


#include "World.h"
#include <memory>
#include <string>
using namespace std;
namespace z2 {

class GameMap {


public:


    shared_ptr<World> buildWorld();

    const string& getDescription();

};

}


#endif //Z2_GAMEMAP_H
