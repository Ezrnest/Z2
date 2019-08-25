/*
 * Created by liyicheng on 2019/7/7.
 */

#include "CommandLineGameGui.h"
#include "../world/World.h"
#include "../core/messages/UnitMove.h"
#include "../core/messages/UnitBuy.h"
#include "../core/messages/UnitAttack.h"
#include "../core/messages/EntityPerform.h"
#include <iostream>
#include <event/StateEvent.h>
#include "plog/Log.h"

using namespace z2;

void CommandLineGameGui::update() {
    if(client->isGameRunning()){
        printWorld();
    }
}

void CommandLineGameGui::onPlayerTurnStarted(int playerId) {
    cout << "Now it's the turn of Player " << playerId << '\n';
    if (playerId != client->getPlayerId()) {
        return;
    }
    cout << "It's your turn!" << endl;
//    runLater([this]() { doPlayerTurn(); });
    doPlayerTurn();
}


void CommandLineGameGui::onPlayerTurnFinished(int playerId) {

}

void CommandLineGameGui::onGameStarted() {
    cout << "Game started!\n";
    printWorld();
}

void CommandLineGameGui::onPlayerWin(int playerId) {
    cout << "Player " << playerId << " has won!\n";
}

void CommandLineGameGui::printWorld() const {
    shared_ptr<World> wPtr = client->getWorld();
    World &w = *wPtr;
    printWorld(w);
}

void CommandLineGameGui::runLater(const CommandLineGameGui::Task &task) {
    lock_guard<mutex> guard(queueMutex);
    taskQueue.push(task);
    queueCV.notify_all();
}


void CommandLineGameGui::mainLoop() {
    while (client->isGameRunning()) {
        Task t;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (taskQueue.empty()) {
                queueCV.wait(lock, [this]() { return !taskQueue.empty(); });
                t = taskQueue.front();
            }
        }

        taskQueue.pop();
        t();
        PLOG(plog::info) << "GUI: Loop";
    }
}

void CommandLineGameGui::doPlayerTurn() {
    while (client->isGameRunning()) {
        printWorld();
        cout << "Please enter the command: Buy, Move, Attack,Perform, End" << endl;
        string cmd;
        cin >> cmd;
        if (strcasecmp(cmd.c_str(), "end") == 0) {
            break;
        } else if (strcasecmp(cmd.c_str(), "move") == 0) {
            makeMove();
        } else if (strcasecmp(cmd.c_str(), "buy") == 0) {
            makeBuy();
        } else if (strcasecmp(cmd.c_str(), "attack") == 0) {
            makeAttack();
        } else if (strcasecmp(cmd.c_str(), "perform") == 0) {
            makePerform();
        } else {
            // do stuff here
            cout << "!" << endl;
        }
    }
//    runLater([this]() { client->sendTurnFinishMessage(); });
    client->sendTurnFinishMessage();
}

void CommandLineGameGui::makeMove() {
    int x, y;
    cin >> x >> y;
    int nx, ny;
    cin >> nx >> ny;
    shared_ptr<UnitMove> msg(new UnitMove(Point(x, y), Point(nx, ny)));
    client->sendMessageToServer(msg);
}

char getTileChar(Tile &t) {
    char c = '?';
    if (!t.hasEntity()) {
        switch (t.getResource()) {
            case Resource::NONE: {
                c = ' ';
                break;
            }
            case Resource::MINE: {
                c = '_';
                break;
            }
            case Resource::GEM: {
                c = '-';
                break;
            }
        }
        switch (t.getTerrain()) {

            case Terrain::PLAIN:
                break;
            case Terrain::MOUNTAIN: {
                c = '#';
                break;
            }
        }
    } else {
        c = t.getEntity()->getEntityName()[0];
    }
    return c;
}

void CommandLineGameGui::printNoPlayerWorld(World &w) {
    Tile **data = w.data;
    for (int j = 0; j < w.width + 1; j++) {
        cout << "--";
    }
    cout << '\n';
    for (int j = w.height - 1; j > -1; j--) {
        cout << "|";
        for (int i = 0; i < w.width; i++) {
            Tile &t = data[i][j];
            char c = getTileChar(t);
            cout << c << ' ';
        }
        cout << "|\n";
    }
    for (int j = 0; j < w.height + 1; j++) {
        cout << "--";
    }
    cout << '\n';
}

void CommandLineGameGui::printWorld(World &w) {
//    cout << "CurrentPlayer:" << w.getCurrentPlayer() << '\n';
    int playerId = w.getCurrentPlayer();
    if (playerId == Player::NO_PLAYER) {
        return;
    }
    Tile **data = w.data;
    for (int j = 0; j < w.width + 1; j++) {
        cout << "--";
    }
    cout << '\n';
    for (int j = w.height - 1; j > -1; j--) {
        cout << "|";
        for (int i = 0; i < w.width; i++) {
            Tile &t = data[i][j];
            char c;
            switch (t.getVisibility(playerId)) {

                case Visibility::DARK: {
                    c = '@';
                    break;
                }
                case Visibility::GREY: {
                    c = '*';
                    break;
                }
                case Visibility::CLEAR: {
                    c = getTileChar(t);
                    break;
                }
            }
            cout << c << ' ';
        }
        cout << "|\n";
    }
    for (int j = 0; j < w.height + 1; j++) {
        cout << "--";
    }
    cout << '\n';
    cout << "Gold: " << w.getPlayer(w.getCurrentPlayer()).getGold() << endl;
}

void CommandLineGameGui::makeBuy() {
    string name;
    cin >> name;
    Point pos = client->getWorld()->searchFor(client->getPlayerId(), "ConstructionBase");
    shared_ptr<UnitBuy> msg(new UnitBuy(name, pos, client->getPlayerId()));
    client->sendMessageToServer(msg);
}

void CommandLineGameGui::makeAttack() {
    Point from, dest;
    from.deserializeData(cin);
    dest.deserializeData(cin);
    shared_ptr<UnitAttack> msg(new UnitAttack(from, dest));
    client->sendMessageToServer(msg);
}

void CommandLineGameGui::makePerform() {
    Point pos;
    pos.deserializeData(cin);
    shared_ptr<EntityPerform> msg(new EntityPerform(pos));
    client->sendMessageToServer(msg);
}

void CommandLineGameGui::onGameStopped() {
    runLater([]() {
    });

}

void CommandLineGameGui::onEvent(const shared_ptr<GameEvent> &event) {
    shared_ptr<GroupEvent> ge = static_pointer_cast<GroupEvent>(event);
    if(ge && ge->getSType() == StateEventType::GroupWon){
        cout << "GROUP " << ge->getGroupId() << " WON !!!" << endl;
        return;
    }
    printWorld();
}


