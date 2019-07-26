/*
 * Created by liyicheng on 2019/7/7.
 */

#ifndef Z2_COMMANDLINEGAMEGUI_H
#define Z2_COMMANDLINEGAMEGUI_H

#include "../core/GameGui.h"

#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;
namespace z2 {
class CommandLineGameGui : public GameGui{

private:
    using Task = function<void()>;
    queue<Task> taskQueue;

    condition_variable queueCV;
    mutex queueMutex;
    
    
    void runLater(const Task& task);

    void doPlayerTurn();
    
    static void printNoPlayerWorld(World& w);
public:
    void update() override;

    void onPlayerTurnStarted(int playerId) override;

    void onGameStarted() override;

    void onPlayerWin(int playerId) override;

    void printWorld() const;

    static void printWorld(World& w);

    void mainLoop();

    void makeMove();

    void makeBuy();

    void makeAttack();

    void makePerform();
};
}

#endif //Z2_COMMANDLINEGAMEGUI_H
