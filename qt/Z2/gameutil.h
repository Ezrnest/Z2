#ifndef GAMEUTIL_H
#define GAMEUTIL_H
#include "world/World.h"

#include <QWidget>

#include <world/GameMap.h>

using namespace z2;
class GameUtil
{
public:
    GameUtil();

    static void saveGame(QWidget* parent,const shared_ptr<World>& world);

    static shared_ptr<GameMap> loadGame(QWidget* parent);
};

#endif // GAMEUTIL_H
